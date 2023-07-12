#include "registry.h"
#include "nzt.h"
#include "utils.h"
#include "crt.h"

static LPWSTR GetRegistryStartPath(INT Hive)
{
	LPWSTR		   Path = NULL;
	UNICODE_STRING US;

	if (Hive == HIVE_HKEY_LOCAL_MACHINE)
	{
		if (!StringConcatW(&Path, L"\\Registry\\Machine\\"))
			return NULL;
	}
	else
	{
		MemoryZero(&US, sizeof(UNICODE_STRING));

		if (API(RtlFormatCurrentUserKeyPath(&US)) >= 0)
		{
			if (!StringConcatW(&Path, US.Buffer))
				return NULL;
		}
	}

	if (!StringEndsWithSlashW(Path))
	{
		if (!StringConcatW(&Path, L"\\"))
		{
			Free(Path);
			Path = NULL;
		}
	}

	return Path;
}

BOOL RegistryOpenKeyEx(CONST LPWSTR KeyPath, HANDLE RegistryHandle, ACCESS_MASK AccessMask)
{
	OBJECT_ATTRIBUTES OJ;
	UNICODE_STRING	  US;
	BOOL			  Status = FALSE;

	if (!StringToUnicode(&US, KeyPath))
		return FALSE;

	MemoryZero(&OJ, sizeof(OBJECT_ATTRIBUTES));

	OJ.Length	  = sizeof(OBJECT_ATTRIBUTES);
	OJ.Attributes = OBJ_CASE_INSENSITIVE;
	OJ.ObjectName = &US;

	if (API(NtOpenKey)(RegistryHandle, AccessMask, &OJ) >= 0)
		Status = TRUE;

	return TRUE;
}

BOOL RegistryReadValueEx(CONST LPWSTR KeyPath, CONST LPWSTR Name, LPWSTR* Value)
{
	HANDLE						   Key;
	UNICODE_STRING				   US;
	KEY_VALUE_PARTIAL_INFORMATION* KVPI;
	KEY_VALUE_PARTIAL_INFORMATION  KV;
	DWORD						   Size   = 0;
	BOOL						   Status = FALSE;

	if (!StringToUnicode(&US, Name))
		return FALSE;

	if (!RegistryOpenKeyEx(KeyPath, &Key, KEY_READ))
		return FALSE;

	MemoryZero(&KV, sizeof(KEY_VALUE_PARTIAL_INFORMATION));

	API(NtQueryValueKey)(Key, &US, KeyValuePartialInformation, &KV, sizeof(KEY_VALUE_PARTIAL_INFORMATION), &Size);

	if (Size != 0)
	{
		if ((KVPI = Malloc(Size)) != 0)
		{
			if (API(NtQueryValueKey)(Key, &US, KeyValuePartialInformation, KVPI, Size, &Size) >= 0)
			{
				if ((*Value = Malloc(KVPI->DataLength + 2)) != 0)
				{
					MemoryCopy(*Value, KVPI->Data, KVPI->DataLength);
					Status = TRUE;
				}
			}

			Free(KVPI);
		}

		API(NtClose)(Key);
	}

	return Status;
}

BOOL RegistryReadValue(INT Hive, CONST LPWSTR Path, CONST LPWSTR Name, LPWSTR* Value)
{
	LPWSTR RegistryPath = NULL;
	BOOL Status			= FALSE;

	if ((RegistryPath = GetRegistryStartPath(Hive)) == 0)
		return FALSE;

	if (StringConcatW(&RegistryPath, Path))
		Status = RegistryReadValueEx(RegistryPath, Name, Value);

	Free(RegistryPath);

	return Status;

}
/*
WINERROR RegistryReadValue(
	LPTSTR ValueName,
	PCHAR* Buffer,
	PULONG BufferSize
)
{
	WINERROR Status = NO_ERROR;
	HKEY	 SubKey;
	ULONG	 DataType = 0;
	PCHAR	 pBuffer;

	if ((Status = RegOpenKey(HKEY_CURRENT_USER, "", &SubKey)) == NO_ERROR)
	{
		if ((Status = RegQueryValueEx(SubKey, ValueName, 0, &DataType, NULL, BufferSize)) == NO_ERROR)
		{
			if (pBuffer == Malloc(*BufferSize))
			{
				if ((Status = RegQueryValueEx(SubKey, ValueName, 0, &DataType, pBuffer, BufferSize)) == NO_ERROR)
					Buffer = pBuffer;
				else
					Free(pBuffer);
			}	//if (pBuffer == Malloc(*BufferSize))
			else
				Status = ERROR_NOT_ENOUGH_MEMORY;
		}	// if ((Status = RegQueryValueEx(SubKey, ValueName, 0, &DataType, NULL, BufferSize)) == NO_ERROR)
			RegCloseKey(SubKey);
	}	// if ((Status = RegOpenKey(HKEY_CURRENT_USER, "", &SubKey)) == NO_ERROR)
	
	return Status;
}

WINERROR RegistryWriteValue(
	LPTSTR ValueName,
	PCHAR Buffer,
	ULONG BufferSize,
	ULONG Type
)
{
	BOOL	Status = NO_ERROR;
	HKEY	SubKey;
	ULONG	DataType = 0;

	if ((Status = RegOpenKey(HKEY_CURRENT_USER, "", &SubKey)) == NO_ERROR)
	{
		if (Buffer)
			Status = RegSetValueEx(SubKey, ValueName, 0, Type, Buffer, BufferSize);
		else
			Status = RegDeleteValue(SubKey, ValueName);

		RegCloseKey(SubKey);
	}

	return Status;
}*/