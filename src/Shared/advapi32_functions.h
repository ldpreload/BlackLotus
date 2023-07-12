#ifndef __ADVAPI32_FUNCTIONS_H__
#define __ADVAPI32_FUNCTIONS_H__

#include <Windows.h>

typedef BOOL(WINAPI* ptGetUserNameA)(
	_Out_writes_to_opt_(*pcbBuffer, *pcbBuffer) LPSTR lpBuffer,
	_Inout_ LPDWORD pcbBuffer
	);

#endif //__ADVAPI32_FUNCTIONS_H__