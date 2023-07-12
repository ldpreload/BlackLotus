#ifndef _GUID_H_
#define _GUID_H_

/* Defines */
#define	GUID_STR_LENGTH		16*2+4+2
#define GUID_STR_FORMAT		("%04X%04X%04X%04X%08X%04X")

/* Structures */
typedef union _GUID_EX
{
	GUID Guid;
	struct
	{
		DWORD Data1;
		DWORD Data2;
		DWORD Data3;
		DWORD Data4;
	};
}GUID_EX, *PGUID_EX;

/* Functions */
VOID GuidGenerate(
	GUID*  Guid,
	PDWORD Seed
);

VOID GuidFillName(
	PDWORD Seed,
	LPTSTR GuidName
);

ULONG GuidToBuffer(
	GUID*	Guid,
	LPTSTR	Buffer
);

LPTSTR GuidToString(GUID *Guid);

LPTSTR GetBotGuid();


#endif