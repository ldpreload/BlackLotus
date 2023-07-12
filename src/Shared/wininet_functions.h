#ifndef __WININET_FUNCTIONS_H__
#define __WININET_FUNCTIONS_H__

#include <Windows.h>
#include <Wininet.h>

typedef HINTERNET(WINAPI* ptInternetOpenW)(
	_In_opt_ LPCWSTR lpszAgent,
	_In_ DWORD dwAccessType,
	_In_opt_ LPCWSTR lpszProxy,
	_In_opt_ LPCWSTR lpszProxyBypass,
	_In_ DWORD dwFlags
	);

typedef HINTERNET(WINAPI* ptInternetConnectA)(
	_In_ HINTERNET hInternet,
	_In_ LPCSTR lpszServerName,
	_In_ INTERNET_PORT nServerPort,
	_In_opt_ LPCSTR lpszUserName,
	_In_opt_ LPCSTR lpszPassword,
	_In_ DWORD dwService,
	_In_ DWORD dwFlags,
	_In_opt_ DWORD_PTR dwContext
	);

typedef BOOL(WINAPI* ptHttpSendRequestA)(
	_In_ HINTERNET hRequest,
	_In_reads_opt_(dwHeadersLength) LPCSTR lpszHeaders,
	_In_ DWORD dwHeadersLength,
	_In_reads_bytes_opt_(dwOptionalLength) LPVOID lpOptional,
	_In_ DWORD dwOptionalLength
	);

typedef BOOL(WINAPI* ptHttpSendRequestW)(
	__in HINTERNET hRequest,
	__in_ecount_opt(dwHeadersLength) LPCWSTR lpszHeaders,
	__in DWORD dwHeadersLength,
	__in_bcount_opt(dwOptionalLength) LPVOID lpOptional,
	__in DWORD dwOptionalLength
	);

typedef HINTERNET(WINAPI* ptHttpOpenRequestA)(
	_In_ HINTERNET hConnect,
	_In_opt_ LPCSTR lpszVerb,
	_In_opt_ LPCSTR lpszObjectName,
	_In_opt_ LPCSTR lpszVersion,
	_In_opt_ LPCSTR lpszReferrer,
	_In_opt_z_ LPCSTR FAR * lplpszAcceptTypes,
	_In_ DWORD dwFlags,
	_In_opt_ DWORD_PTR dwContext
	);

typedef BOOL (WINAPI* ptHttpQueryInfoA)(
	IN      HINTERNET hRequest,
	IN      DWORD     dwInfoLevel,
	IN OUT  LPVOID    lpBuffer,
	IN OUT LPDWORD   lpdwBufferLength,
	IN OUT LPDWORD   lpdwIndex
);

typedef BOOL(WINAPI* ptInternetCloseHandle)(
	_In_ HINTERNET hInternet
	);

typedef BOOL(WINAPI* ptInternetReadFile)(
	_In_ HINTERNET hFile,
	_Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(NETWORK) LPVOID lpBuffer,
	_In_ DWORD dwNumberOfBytesToRead,
	_Out_ LPDWORD lpdwNumberOfBytesRead
	);

#endif __WININET_FUNCTIONS_H__