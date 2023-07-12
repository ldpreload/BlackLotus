#ifndef __SHELL32_FUNCTIONS_H__
#define __SHELL32_FUNCTIONS_H__

#include <Windows.h>
#include <ShlObj.h>

typedef HRESULT(WINAPI* ptSHGetFolderPathW)(_Reserved_ HWND hwnd, _In_ int csidl, _In_opt_ HANDLE hToken, _In_ DWORD dwFlags, _Out_writes_(MAX_PATH) LPWSTR pszPath);

#endif //__SHELL32_FUNCTIONS_H__