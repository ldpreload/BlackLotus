#include "nzt.h"
#include "crypto.h"

#define RtlOffsetToPointer(B,O) ((PCHAR)(((PCHAR)(B)) + ((ULONG_PTR)(O))))

DWORD Crc32Hash(CONST PVOID Data, DWORD Size)
{
	DWORD i, j, crc, cc;

	if (NzT.Crc.Initialized == FALSE)
	{
		for (i = 0; i < 256; i++)
		{
			crc = i;
			for (j = 8; j > 0; j--)
			{
				if (crc & 0x1)crc = (crc >> 1) ^ 0xEDB88320L;
				else crc >>= 1;
			}
			NzT.Crc.Table[i] = crc;
		}

		NzT.Crc.Initialized = TRUE;
	}
	cc = 0xFFFFFFFF;
	for (i = 0; i < Size; i++)cc = (cc >> 8) ^ NzT.Crc.Table[(((LPBYTE)Data)[i] ^ cc) & 0xFF];
	return ~cc;
}

VOID CryptRC4(PCHAR pKey, DWORD Key, PVOID Destination, PVOID Source, DWORD Length)
{
	DWORD i = 0, j = 0, k = 0;
	UCHAR ucKey[256] = { 0 };
	UCHAR ucTemp = 0;

	for (i = 0; i < sizeof(ucKey); i++) 
		ucKey[i] = (CHAR)i;

	for (i = j = 0; i < sizeof(ucKey); i++)
	{
		j = (j + pKey[i % Key] + ucKey[i]) % 256;

		ucTemp		= ucKey[i];
		ucKey[i]	= ucKey[j];
		ucKey[j]	= ucTemp;
	}

	for (i = j = 0, k = 0; k < Length; k++)
	{
		i = (i + 1) % 256;
		j = (j + ucKey[i]) % 256;

		ucTemp = ucKey[i];
		ucKey[i] = ucKey[j];
		ucKey[j] = ucTemp;

		*RtlOffsetToPointer(Destination, k) = *RtlOffsetToPointer(Source, k) ^ ucKey[(ucKey[i] + ucKey[j]) % 256];
	}
}

//
//	Encrypts the specified memory buffer by XORing it's data with the specified key value in CBC manner.
//
VOID __stdcall CryptXor(
	PCHAR	Buffer,		// data buffer
	ULONG	Size,		// size of the buffer in bytes
	ULONG	Key,		// key value
	BOOL	SkipZero	// TRUE to skip zero dwords
)
{
	PULONG	pDwords = (PULONG)Buffer;
	ULONG	uDword, uVector = 0, Count = 0;

	if (Size /= sizeof(ULONG))
	{
		do
		{
			uDword = *pDwords;

			if (SkipZero && uDword == 0 && Size > 1 && pDwords[1] == 0)
				break;

			uDword = _rotl(uDword, Count += 1);
			uDword ^= uVector;
			uDword ^= Key;
			uVector = uDword;

			*pDwords = uDword;
			pDwords += 1;
		} while (Size -= 1);
	}	// if (Size /= sizeof(ULONG))
}

VOID __stdcall XorDecryptBuffer(
	PCHAR	Buffer,	// buffer containing encrypted data
	ULONG	Size,		// size of the buffer in bytes
	ULONG	Key,		// key value
	BOOL	SkipZero	// TRUE to skip zero dwords
)
{
	PULONG	pDwords = (PULONG)Buffer;
	ULONG	uDword, uLast, uVector = 0, Count = 0;

	if (Size /= sizeof(ULONG))
	{
		do
		{
			uLast = uDword = *pDwords;
			if (SkipZero && uDword == 0)
				break;

			uDword ^= Key;
			uDword ^= uVector;
			uDword = _rotr(uDword, Count += 1);
			uVector = uLast;

			*pDwords = uDword;
			pDwords += 1;
		} while (Size -= 1);
	}	// if (Size /= sizeof(ULONG))
}
