#ifndef __HOOK_H__
#define __HOOK_H__


typedef struct
{
	CHAR*	DLL;
	CHAR*	Name;
	LPVOID	Proxy;
	LPVOID	Original;
	DWORD	Length;
} HOOKS, PHOOKS;

typedef int (WINAPI *pOldMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCTSTR lpCaption, UINT uType);

int HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCTSTR lpCaption, UINT uType);

BOOL HookFunction(PCHAR DLL, PCHAR Name, LPVOID Proxy, LPVOID Original, PDWORD Length);
BOOL UnhookFunction(PCHAR DLL, PCHAR Name, LPVOID Proxy, LPVOID Original, PDWORD Length);
#endif

