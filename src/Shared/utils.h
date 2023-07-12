#ifndef _UTILS_H_
#define _UTILS_H_

#include "crypto.h"

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#define	htonS(x)  _byteswap_ushort(x)
#define	htonL(x)  _byteswap_ulong(x)

#ifdef DEBUG
#define  DebugPrint(args, ...) \
		{ char* buff = (char*)Malloc(128); \
		  API(wsprintfA)(buff, args, __VA_ARGS__); \
		  API(OutputDebugStringA)((LPCSTR)buff); \
		  Free(buff); }

#define  DebugPrintW(args, ...) \
		{ wchar_t* buff = (wchar_t*)Malloc(128); \
		  API(wsprintfW)(buff, args, __VA_ARGS__); \
		  API(OutputDebugStringW)((LPCWSTR)buff); \
		  Free(buff); } 
#else
#define DebugPrint(x, ...) 
#define DebugPrintW(x, ...) 
#endif

/* Enum */
enum OS_VERSION
{
	WINDOWS_2000			= 0,
	WINDOWS_XP				= 1,
	WINDOWS_VISTA			= 2,
	WINDOWS_7				= 3,
	WINDOWS_8				= 4,
	WINDOWS_8_1				= 5,
	WINDOWS_10				= 6,
	WINDOWS_11				= 7,
	WINDOWS_SERVER_2000		= 8,
	WINDOWS_SERVER_2003		= 9,
	WINDOWS_SERVER_2008		= 10,
	WINDOWS_SERVER_2012		= 11,
	WINDOWS_SERVER_2016		= 12
};

enum OS_ARCHITECTURE
{
	X86 = 0,
	X64 = 1
};

enum OS_PATH
{
	PATH_APPDATA = 0,
	PATH_DESKTOP,
	PATH_STARTUP
};

/* Functions */
DWORD GenerateSeed(DWORD Seed);
DWORD RandomNumber(DWORD Seed);
DWORD GetRandomNumber();

BOOL GetModulePath(
	HMODULE		Module,		// a handle of the module within current process
	PVOID*		ModulePath,	// returned module path
	BOOL		bUnicode
);


BOOL	DownloadFile(CONST PCHAR URL, BOOL Execute);
DWORD	GetOperatingSystem();
PPEB	GetPeb(VOID);
BOOL	Is64Bit();
BOOL	IsAdmin();
PDWORD	GetSerialNumber();
LPSTR	GetUsername();
DWORD	GetProcessIdByHandle(HANDLE Process);
DWORD	GetProcessIdByHash(DWORD Hash);
HANDLE	CreateMutexOfProcess(DWORD ProcessID);
LPWSTR  GetSystem32();
LPWSTR	GetDirectoryPath(int PathIndex);
VOID	_Sleep(DWORD Miliseconds);

#endif