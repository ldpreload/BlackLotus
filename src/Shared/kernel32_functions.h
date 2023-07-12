#ifndef __KERNEL32_FUNCTIONS_H__
#define __KERNEL32_FUNCTIONS_H__

#include <Windows.h>
#include <TlHelp32.h>

typedef LPVOID(WINAPI* ptVirtualAlloc)(
	_In_opt_ LPVOID lpAddress,
	_In_     SIZE_T dwSize,
	_In_     DWORD  flAllocationType,
	_In_     DWORD  flProtect
	);

typedef HANDLE(WINAPI* ptOpenProcess)(
	_In_ DWORD dwDesiredAccess,
	_In_ BOOL  bInheritHandle,
	_In_ DWORD dwProcessId
	);

typedef BOOL(WINAPI* ptProcess32FirstW)(
	_In_  HANDLE           hSnapshot,
	_Out_ LPPROCESSENTRY32W lppe
	);

typedef BOOL(WINAPI* ptProcess32NextW)(
	_In_  HANDLE           hSnapshot,
	_Out_ LPPROCESSENTRY32W lppe
	);

typedef BOOL(WINAPI* ptWriteProcessMemory)(
	_In_  HANDLE  hProcess,
	_In_  LPVOID  lpBaseAddress,
	_In_  LPCVOID lpBuffer,
	_In_  SIZE_T  nSize,
	_Out_ SIZE_T  *lpNumberOfBytesWritten
	);

typedef LPVOID(WINAPI* ptVirtualAllocEx)(
	_In_     HANDLE hProcess,
	_In_opt_ LPVOID lpAddress,
	_In_     SIZE_T dwSize,
	_In_     DWORD  flAllocationType,
	_In_     DWORD  flProtect
	);

typedef BOOL(WINAPI* ptVirtualFree)(
	_In_ LPVOID lpAddress,
	_In_ SIZE_T dwSize,
	_In_ DWORD  dwFreeType
	);

typedef BOOL(WINAPI* ptVirtualFreeEx)(
	_In_ HANDLE hProcess,
	_In_ LPVOID lpAddress,
	_In_ SIZE_T dwSize,
	_In_ DWORD  dwFreeType
	);

typedef BOOL(WINAPI* ptSetThreadContext)(
	_In_       HANDLE  hThread,
	_In_ const CONTEXT *lpContext
	);

typedef HANDLE(WINAPI* ptCreateRemoteThread)(
	_In_  HANDLE                 hProcess,
	_In_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
	_In_  SIZE_T                 dwStackSize,
	_In_  LPTHREAD_START_ROUTINE lpStartAddress,
	_In_  LPVOID                 lpParameter,
	_In_  DWORD                  dwCreationFlags,
	_Out_ LPDWORD                lpThreadId
	);

typedef BOOL(WINAPI* ptCloseHandle)(
	_In_ HANDLE hObject
	);

typedef HANDLE(WINAPI* ptCreateToolhelp32Snapshot)(
	_In_ DWORD dwFlags,
	_In_ DWORD th32ProcessID
	);

typedef BOOL(WINAPI* ptCreateProcessW)(
	_In_opt_ LPCWSTR lpApplicationName,
	_Inout_opt_ LPWSTR lpCommandLine,
	_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ BOOL bInheritHandles,
	_In_ DWORD dwCreationFlags,
	_In_opt_ LPVOID lpEnvironment,
	_In_opt_ LPCWSTR lpCurrentDirectory,
	_In_ LPSTARTUPINFOW lpStartupInfo,
	_Out_ LPPROCESS_INFORMATION lpProcessInformation
	);

typedef BOOL(WINAPI* ptVirtualProtect)(
	_In_  LPVOID lpAddress,
	_In_  SIZE_T dwSize,
	_In_  DWORD  flNewProtect,
	_Out_ PDWORD lpflOldProtect
	);

typedef VOID(WINAPI* ptExitProcess)(
	_In_ UINT uExitCode
	);

typedef DWORD(WINAPI* ptGetModuleFileNameW)(
	_In_opt_ HMODULE hModule,
	_Out_writes_to_(nSize, ((return < nSize) ? (return +1) : nSize)) LPWSTR lpFilename,
	_In_ DWORD nSize
	);

typedef BOOL(WINAPI* ptDeleteFileW)(
	_In_ LPCWSTR lpFileName
	);

typedef VOID(WINAPI* ptSleep)(
	_In_ DWORD dwMilliseconds
	);

typedef HMODULE(WINAPI* ptLoadLibraryW)(
	_In_ LPCWSTR lpLibFileName
	);

typedef BOOL(WINAPI* ptIsWow64Process)(
	_In_  HANDLE hProcess,
	_Out_ PBOOL  Wow64Process
	);

typedef DWORD(WINAPI* ptGetCurrentProcessId)(VOID);

typedef UINT(WINAPI* ptGetWindowsDirectoryW)(
	_Out_writes_to_opt_(uSize, return +1) LPWSTR lpBuffer,
	_In_ UINT uSize
	);

typedef DWORD(WINAPI* ptResumeThread)(
	_In_ HANDLE hThread
	);

typedef DWORD(WINAPI* ptQueueUserAPC)(
	_In_ PAPCFUNC pfnAPC,
	_In_ HANDLE hThread,
	_In_ ULONG_PTR dwData
	);

typedef UINT(WINAPI* ptGetSystemDirectoryW)(
	_Out_writes_to_opt_(uSize, return +1) LPWSTR lpBuffer,
	_In_ UINT uSize
	);

typedef HANDLE(WINAPI* ptFindFirstFileW)(
	_In_ LPCWSTR lpFileName,
	_Out_ LPWIN32_FIND_DATAW lpFindFileData
	);

typedef BOOL(WINAPI* ptFindNextFileW)(
	_In_ HANDLE hFindFile,
	_Out_ LPWIN32_FIND_DATAW lpFindFileData
	);

typedef HANDLE(WINAPI* ptCreateThread)(
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ SIZE_T dwStackSize,
	_In_ LPTHREAD_START_ROUTINE lpStartAddress,
	_In_opt_ __drv_aliasesMem LPVOID lpParameter,
	_In_ DWORD dwCreationFlags,
	_Out_opt_ LPDWORD lpThreadId
	);

typedef HANDLE(WINAPI* ptCreateFileW)(
	_In_ LPCWSTR lpFileName,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwShareMode,
	_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	_In_ DWORD dwCreationDisposition,
	_In_ DWORD dwFlagsAndAttributes,
	_In_opt_ HANDLE hTemplateFile
	);

typedef BOOL(WINAPI* ptWriteFile)(
	_In_ HANDLE hFile,
	_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
	_In_ DWORD nNumberOfBytesToWrite,
	_Out_opt_ LPDWORD lpNumberOfBytesWritten,
	_Inout_opt_ LPOVERLAPPED lpOverlapped
	);

typedef BOOL(WINAPI* ptReadFile)(
	_In_ HANDLE hFile,
	_Out_writes_bytes_to_opt_(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
	_In_ DWORD nNumberOfBytesToRead,
	_Out_opt_ LPDWORD lpNumberOfBytesRead,
	_Inout_opt_ LPOVERLAPPED lpOverlapped
	);

typedef DWORD(WINAPI* ptGetFileSize)(
	_In_      HANDLE  hFile,
	_Out_opt_ LPDWORD lpFileSizeHigh
	);

typedef BOOL(WINAPI* ptGetVersionExW)(
	_Inout_ LPOSVERSIONINFOW lpVersionInformation
	);

typedef HANDLE(WINAPI* ptFindFirstVolumeW)(
	_Out_writes_(cchBufferLength) LPWSTR lpszVolumeName,
	_In_ DWORD cchBufferLength
	);

typedef BOOL(WINAPI* ptGetVolumeInformationW)(
	_In_opt_ LPCWSTR lpRootPathName,
	_Out_writes_opt_(nVolumeNameSize) LPWSTR lpVolumeNameBuffer,
	_In_ DWORD nVolumeNameSize,
	_Out_opt_ LPDWORD lpVolumeSerialNumber,
	_Out_opt_ LPDWORD lpMaximumComponentLength,
	_Out_opt_ LPDWORD lpFileSystemFlags,
	_Out_writes_opt_(nFileSystemNameSize) LPWSTR lpFileSystemNameBuffer,
	_In_ DWORD nFileSystemNameSize
	);

typedef BOOL(WINAPI* ptFindVolumeClose)(
	_In_ HANDLE hFindVolume
	);

typedef int (WINAPI* ptwsprintfA)(
	_Out_ LPSTR,
	_In_ _Printf_format_string_ LPCSTR,
	...);

typedef int (WINAPI* ptMultiByteToWideChar)(
	_In_      UINT   CodePage,
	_In_      DWORD  dwFlags,
	_In_      LPCSTR lpMultiByteStr,
	_In_      int    cbMultiByte,
	_Out_opt_ LPWSTR lpWideCharStr,
	_In_      int    cchWideChar
	);

typedef HMODULE(WINAPI* ptGetModuleHandleW)(
	_In_opt_ LPCWSTR lpModuleName
	);

typedef BOOL(WINAPI* ptFlushInstructionCache)(
	__in HANDLE hProcess,
	__in_bcount_opt(dwSize) LPCVOID lpBaseAddress,
	__in SIZE_T dwSize
	);

typedef HANDLE(WINAPI* ptGetProcessHeap)(VOID);
typedef LPVOID(WINAPI* ptHeapAlloc)(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
	);
typedef BOOL(WINAPI* ptHeapFree)(
	_Inout_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	LPVOID lpMem
	);

typedef HANDLE(WINAPI* ptGetCurrentProcess)(
	VOID
	);

typedef BOOL(WINAPI* ptThread32First)(
	HANDLE hSnapshot,
	LPTHREADENTRY32 lpte
	);

typedef BOOL(WINAPI* ptThread32Next)(
	HANDLE hSnapshot,
	LPTHREADENTRY32 lpte
	);

typedef HANDLE(WINAPI* ptOpenMutexW)(
	_In_ DWORD dwDesiredAccess,
	_In_ BOOL bInheritHandle,
	_In_ LPCWSTR lpName
	);

typedef BOOL(WINAPI* ptReleaseMutex)(
	_In_ HANDLE hMutex
	);

typedef HANDLE(WINAPI* ptCreateMutexW)(
	_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
	_In_ BOOL bInitialOwner,
	_In_opt_ LPCWSTR lpName
	);

typedef SIZE_T(WINAPI* ptVirtualQuery)(
	__in_opt LPCVOID lpAddress,
	__out_bcount_part(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer,
	__in     SIZE_T dwLength
	);

typedef HANDLE(WINAPI* ptCreateFileMappingW)(
	_In_ HANDLE hFile,
	_In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
	_In_ DWORD flProtect,
	_In_ DWORD dwMaximumSizeHigh,
	_In_ DWORD dwMaximumSizeLow,
	_In_opt_ LPCWSTR lpName
	);

typedef LPVOID(WINAPI* ptMapViewOfFile)(
	_In_ HANDLE hFileMappingObject,
	_In_ DWORD dwDesiredAccess,
	_In_ DWORD dwFileOffsetHigh,
	_In_ DWORD dwFileOffsetLow,
	_In_ SIZE_T dwNumberOfBytesToMap
	);

typedef BOOL(WINAPI* ptUnmapViewOfFile)(
	_In_ LPCVOID lpBaseAddress
	);

typedef BOOL(WINAPI* ptDuplicateHandle)(
	_In_ HANDLE hSourceProcessHandle,
	_In_ HANDLE hSourceHandle,
	_In_ HANDLE hTargetProcessHandle,
	_Outptr_ LPHANDLE lpTargetHandle,
	_In_ DWORD dwDesiredAccess,
	_In_ BOOL bInheritHandle,
	_In_ DWORD dwOptions
	);

typedef HANDLE(WINAPI* ptGetCurrentThread)(VOID);
typedef BOOL(WINAPI* ptFlushFileBuffers)(HANDLE hFile);
typedef BOOL(WINAPI* ptDisconnectNamedPipe)(HANDLE hNamedPipe);
typedef FARPROC(WINAPI* ptGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
typedef void (WINAPI* ptRtlInitializeCriticalSection)(RTL_CRITICAL_SECTION* lpCriticalSection);
typedef void (WINAPI* ptRtlEnterCriticalSection)(RTL_CRITICAL_SECTION* lpCriticalSection);
typedef void (WINAPI* ptRtlLeaveCriticalSection)(RTL_CRITICAL_SECTION* lpCriticalSection);

typedef int (WINAPI* ptWideCharToMultiByte)(
	UINT     CodePage,
	DWORD    dwFlags,
	LPCWSTR  lpWideCharStr,
	int      cchWideChar,
	LPSTR   lpMultiByteStr,
	int      cbMultiByte,
	LPCSTR   lpDefaultChar,
	LPBOOL  lpUsedDefaultChar);

typedef BOOL(WINAPI* ptTerminateThread)(
	_Inout_ HANDLE hThread,
	_In_    DWORD  dwExitCode
);

typedef DWORD(WINAPI* ptGetTickCount)(VOID);

typedef void (WINAPI* ptOutputDebugStringA)(
	LPCSTR lpOutputString
);

typedef void (WINAPI* ptOutputDebugStringW)(
	LPCWSTR lpOutputString
);

typedef DWORD(WINAPI* ptGetLastError)();

typedef HANDLE(WINAPI* ptCreateEventA)(
	_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
	_In_     BOOL				   bManualReset,
	_In_	 BOOL				   bInitialState,
	_In_opt_ LPCSTR				   lpName
	);

typedef HANDLE(WINAPI* ptCreateEventW)(
	_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
	_In_     BOOL				   bManualReset,
	_In_	 BOOL				   bInitialState,
	_In_opt_ LPCWSTR           	   lpName
	);

typedef BOOL(WINAPI* ptSetEvent)(
	_In_ HANDLE hEvent
);

typedef HANDLE(WINAPI* ptOpenEventA)(
	_In_ DWORD	dwDesiredAccess,
	_In_ BOOL	bInheritHandle,
	_In_ LPCSTR lpName
);

typedef HANDLE(WINAPI* ptOpenEventW)(
	_In_ DWORD	 dwDesiredAccess,
	_In_ BOOL	 bInheritHandle,
	_In_ LPCWSTR lpName
);



#endif __KERNEL32_FUNCTIONS_H__