#ifndef __USER32_FUNCTIONS_H__
#define __USER32_FUNCTIONS_H__

#include <Windows.h>

typedef int(WINAPI* ptMessageBoxA)(
	_In_opt_ HWND,
	_In_opt_ LPCSTR,
	_In_opt_ LPCSTR,
	_In_     UINT
	);

typedef int (WINAPIV* ptwsprintfW)(
	_Out_ LPWSTR,
	_In_ _Printf_format_string_ LPCWSTR,
	...);

typedef BOOL(WINAPI* ptGetCursorPos)(
	_Out_ LPPOINT lpPoint
	);

#endif //__USER32_FUNCTIONS_H__