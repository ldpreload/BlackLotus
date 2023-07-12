#ifndef _CRT_H_
#define _CRT_H_

#include "ntdll.h"

/* memory */
void MemoryCopy(void* Destination, const void* Source, DWORD Size);
VOID	MemoryZero(PVOID Destination, SIZE_T Size);
DWORD	MemorySize(LPVOID Address);

LPVOID	Malloc(DWORD Size);
VOID	Free(PVOID Data);

BOOL	ReallocEx(PVOID Old, DWORD Size);
LPVOID	Realloc(PVOID Old, SIZE_T Size);

/* strings */
INT		IntToString(CONST PCHAR String);

DWORD	StringLengthA(CONST LPSTR String);
DWORD	StringLengthW(CONST LPWSTR String);

LPSTR	StringCopyA(CONST LPSTR Input, DWORD Length);
LPWSTR	StringCopyW(CONST LPWSTR Input, DWORD Length);

BOOL	StringConcatExA(LPSTR* ppszData, DWORD dwCurrentLength, const LPSTR pszSource, DWORD dwLength);
BOOL	StringConcatA(LPSTR* ppszData, const LPSTR pszSource);
BOOL	StringConcatExW(LPWSTR* ppwzData, DWORD dwCurrentLength, const LPWSTR pwzSource, DWORD dwLength);
BOOL	StringConcatW(LPWSTR* ppwzData, const LPWSTR pwzSource);

INT		StringCompareA(LPCSTR String1, LPCSTR String2);
BOOL	StringCompareW(CONST LPWSTR Input, CONST LPWSTR Data);

PCHAR	StringTokenizeA(PCHAR String, CONST PCHAR Delim);
PWCHAR	StringTokenizeW(PWCHAR String, CONST PWCHAR Delim);

PWCHAR  StringToLowerW(CONST PWCHAR Input, DWORD Length);
BOOL	StringEndsWithSlashW(CONST PWCHAR String);

char*	StringChr(s, c);

BOOL	StringToUnicode(PUNICODE_STRING String, CONST LPWSTR Buffer);
void	FreeUnicodeString(PUNICODE_STRING String);

#endif