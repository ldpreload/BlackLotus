#ifndef _CRYPTO_H_
#define _CRYPTO_H_


DWORD Crc32Hash(CONST PVOID Data, DWORD Size);
VOID CryptRC4(PCHAR pKey, DWORD Key, PVOID Destination, PVOID Source, DWORD Length);
VOID __stdcall CryptXor(
	PCHAR	Buffer,		// data buffer
	ULONG	Size,		// size of the buffer in bytes
	ULONG	Key,		// key value
	BOOL	SkipZero	// TRUE to skip zero dwords
);
VOID __stdcall XorDecryptBuffer(
	PCHAR	Buffer,		// buffer containing encrypted data
	ULONG	Size,		// size of the buffer in bytes
	ULONG	Key,		// key value
	BOOL	SkipZero	// TRUE to skip zero dwords
);

#endif