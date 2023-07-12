#include <windows.h>

#include "crt.h"
#include "nzt.h"
#include "ntdll.h"
#include "utils.h"

void MemoryCopy(void* Destination, const void* Source, DWORD Size)
{
	DWORD i;
	for (i = 0; i < Size; i++)
	{
		((LPBYTE)Destination)[i] = ((LPBYTE)Source)[i];
	}
}

//MemoryCopy pad by 8 bytes for relative JMP (0xE9)
VOID SafeMemoryCopy_p(
	LPVOID	Destination,
	LPVOID	Source,
	DWORD	Size
)
{
	BYTE Buffer[8];

	if (Size > 8)
		return;

	//8 bytes padding
	MemoryCopy(Buffer, Destination, 8);
	MemoryCopy(Buffer, Source, Size);

	if (Is64Bit())
	{
		_InterlockedCompareExchange64((LONGLONG *)Destination, *(LONGLONG*)Buffer, *(LONGLONG *)Destination);
		return;
	}

	__asm
	{
		lea esi, Buffer;
		mov edi, Destination;

		mov eax, [edi];
		mov edx, [edi + 4];
		mov ebx, [esi];
		mov ecx, [esi + 4];

		lock cmpxchg8b[edi];
	}
}

VOID MemoryZero(
	PVOID	Destination,
	SIZE_T	Size
)
{
	PCHAR Data = NULL;

	if ((Data = (char*)Destination) == NULL)
		return;

	for (DWORD i = 0; i < Size; i++)
		Data[i] = 0x00;
}

DWORD MemorySize(LPVOID Address)
{
	MEMORY_BASIC_INFORMATION Memory;

	if (!Address)
		return 0;

	NzT.Api.pVirtualQuery(Address, &Memory, sizeof(MEMORY_BASIC_INFORMATION));

	return Memory.RegionSize;

}

LPVOID Malloc(DWORD Size)
{
	LPVOID Ptr = 0;

	if (API(NtAllocateVirtualMemory(CURRENT_PROCESS, &Ptr, 0, &Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) >= 0)
		MemoryZero(Ptr, Size);

	return Ptr;
}

VOID Free(PVOID Data)
{
	DWORD f = 0;

	if (Data)
		API(NtFreeVirtualMemory(CURRENT_PROCESS, &Data, &f, MEM_RELEASE));
}

LPVOID Realloc(
	PVOID	Old,
	SIZE_T	Size
)
{
	LPVOID NewAddress = NULL;
	DWORD  OldLength  = 0;

	if (Old)
		OldLength = MemorySize(Old);

	NewAddress = NULL;
	if (Size > 0)
	{
		NewAddress = Malloc(Size);
		if (Old && NewAddress && OldLength)
		{
			if (Size < OldLength)
				OldLength = Size;
			MemoryCopy(NewAddress, Old, OldLength);
		}
	}

	if (Old != NULL)
		Free(Old);

	return NewAddress;
}

BOOL ReallocEx(
	PVOID Old,
	DWORD Size
)
{
	if (Size == 0)
	{
		Free(*(LPBYTE *)Old);
		*(LPBYTE *)Old = NULL;
	}
	else
	{
		register PVOID p = Realloc(*(LPBYTE *)Old, Size);
		if (p == NULL) return FALSE;
		*(LPBYTE *)Old = (LPBYTE)p;
	}

	return TRUE;
}

INT IntToString(CONST PCHAR String)
{
	INT Number = 0;
	INT i = 0;

	// run till the end of the string is reached, or the
	// current character is non-numeric
	while (String[i] && (String[i] >= '0' && String[i] <= '9'))
	{
		Number = Number * 10 + (String[i] - '0');
		i++;
	}

	return Number;
}


DWORD StringLengthA(CONST LPSTR String)
{
	DWORD Size = 0;

	if (String == NULL)
		return 0;

	do
	{
		if (String[Size] == 0)
			break;

		Size++;
	} while (TRUE);

	return Size;
}

DWORD StringLengthW(CONST LPWSTR String)
{
	DWORD Size = 0;

	if (String == NULL)
		return 0;

	do
	{
		if (String[Size] == 0)
			break;

		Size++;
	} while (TRUE);

	return Size;
}

LPSTR StringCopyA(
	CONST LPSTR Input,
	DWORD Length
)
{
	LPSTR Data;

	if ((Data = (LPSTR)Malloc(Length + 1)) == NULL)
		return NULL;

	MemoryCopy(Data, Input, Length);

	return Data;
}

LPWSTR StringCopyW(
	CONST LPWSTR Input,
	DWORD Length
)
{
	LPWSTR Data;
	DWORD NewStringLength;

	NewStringLength = Length * sizeof(wchar_t);

	if ((Data = (LPWSTR)Malloc(NewStringLength + sizeof(wchar_t))) == NULL)
		return NULL;

	MemoryCopy(Data, Input, NewStringLength);

	return Data;
}

BOOL StringConcatExA(
	LPSTR*		ppszData,
	DWORD		dwCurrentLength,
	const LPSTR pszSource,
	DWORD		dwLength
)
{
	if (ReallocEx(ppszData, dwCurrentLength + dwLength + 1))
	{
		MemoryCopy((*ppszData) + dwCurrentLength, pszSource, dwLength);
		return TRUE;
	}

	return FALSE;
}

BOOL StringConcatA(
	LPSTR*		ppszData,
	const LPSTR pszSource
)
{
	return StringConcatExA(ppszData, StringLengthA(*ppszData), pszSource, StringLengthA(pszSource));
}

BOOL StringConcatExW(
	LPWSTR*			ppwzData,
	DWORD			dwCurrentLength,
	const LPWSTR	pwzSource,
	DWORD			dwLength
)
{
	if (ReallocEx(ppwzData, (dwCurrentLength + dwLength + 1) * sizeof(wchar_t)))
	{
		MemoryCopy((*ppwzData) + dwCurrentLength, pwzSource, dwLength * sizeof(wchar_t));
		return TRUE;
	}

	return FALSE;
}

BOOL StringConcatW(
	LPWSTR*		 ppwzData,
	const LPWSTR pwzSource)
{
	return StringConcatExW(ppwzData, StringLengthW(*ppwzData), pwzSource, StringLengthW(pwzSource));
}

INT StringCompareA(
	LPCSTR String1,
	LPCSTR String2
)
{
	for (; *String1 == *String2; String1++, String2++)
	{
		if (*String1 == '\0')
			return 0;
	}

	return ((*(LPCSTR)String1 < *(LPCSTR)String2) ? -1 : +1);
}

BOOL StringCompareW(
	CONST LPWSTR Input,
	CONST LPWSTR Data
)
{
	DWORD InputLength, DataLength, i;

	InputLength = StringLengthW(Input);
	DataLength	= StringLengthW(Data);

	if (DataLength != InputLength)
		return FALSE;

	for (i = 0; i < DataLength; i++)
	{
		if (Input[i] != Data[i])
			return FALSE;
	}

	return TRUE;
}

PCHAR StringTokenizeA(
	PCHAR str,
	CONST PCHAR Delimiter
)
{
	static int pos;
	static char *s;
	int i = 0, start = pos;

	// Copying the string for further calls of strtok
	if (str != NULL)
		s = str;

	i = 0;
	int j = 0;
	//While not end of string
	while (s[pos] != '\0')
	{
		j = 0;
		//Comparing of one of the delimiter matches the character in the string
		while (Delimiter[j] != '\0')
		{
			//Pos point to the next location in the string that we have to read
			if (s[pos] == Delimiter[j])
			{
				//Replace the delimter by \0 to break the string
				s[pos] = '\0';
				pos = pos + 1;
				//Checking for the case where there is no relevant string before the delimeter.
				//start specifies the location from where we have to start reading the next character
				if (s[start] != '\0')
					return (&s[start]);
				else
				{
					// Move to the next string after the delimiter
					start = pos;
					// Decrementing as it will be incremented at the end of the while loop
					pos--;
					break;
				}
			}
			j++;
		}
		pos++;
	}//End of Outer while
	s[pos] = '\0';
	if (s[start] == '\0')
		return NULL;
	else
		return &s[start];
}

PWCHAR StringTokenizeW(
	PWCHAR		 String,
	CONST PWCHAR Delim
)
{
	PWCHAR Last = NULL;
	PWCHAR SpanP = NULL, Token = NULL;
	INT C = 0, SC = 0;

	if (String == NULL)
		return NULL;

CONTINUE:

	C = *String++;

	for (SpanP = (PWCHAR)Delim; (SC = *SpanP++) != ERROR_SUCCESS;)
	{
		if (C == SC)
			goto CONTINUE;
	}

	if (C == ERROR_SUCCESS) { Last = NULL; return NULL; }

	Token = String - 1;

	for (;;)
	{
		C = *String++;
		SpanP = (PWCHAR)Delim;

		do {
			if ((SC = *SpanP++) == C)
			{
				if (C == ERROR_SUCCESS)
					String = NULL;
				else
					String[-1] = '\0';

				Last = String;
				return Token;
			}
		} while (SC != ERROR_SUCCESS);
	}

	return NULL;
}

PWCHAR StringToLowerW(
	CONST PWCHAR Input,
	DWORD		 Length
)
{
	int c = 0;
	static WCHAR Lower[255];

	if (Input == NULL)
		return NULL;

	MemoryZero(&Lower, sizeof(Lower));

	for (DWORD i = 0; i < Length; i++)
	{
		c = Input[i];

		if (c >= 65 && c <= 90)
			c += 32;

		Lower[i] = c;
	}

	return Lower;
}

BOOL StringEndsWithSlashW(CONST PWCHAR String)
{
	DWORD Length;
	WCHAR Data;

	if ((Length = StringLengthW(String)) == 0)
		return FALSE;

	Data = String[Length];

	if (Data == L'\\') return TRUE;

	return FALSE;
}

char *StringChr(s, c)
register const char *s;
int c;
{
	char *rtnval = 0;

	do {
		if (*s == c)
			rtnval = (char*)s;
	} while (*s++);
	return (rtnval);
}

BOOL StringToUnicode(
	PUNICODE_STRING String,
	CONST LPWSTR	Buffer
)
{
	DWORD Length;

	if (String == 0)
		return FALSE;

	Length = StringLengthW(Buffer);

	if ((String->Buffer = StringCopyW(Buffer, Length)) == 0)
		return FALSE;

	String->Length			= Length * 2;
	String->MaximumLength	= Length * 2;

	return TRUE;
}

void FreeUnicodeString(PUNICODE_STRING String)
{
	if (String == 0)
		return;

	if (String->Buffer != 0)
	{
		Free(String->Buffer);
		MemoryZero(String, sizeof(UNICODE_STRING));
	}
}