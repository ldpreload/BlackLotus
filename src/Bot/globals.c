#include <Windows.h>
#include <synchapi.h>
#ifdef _DEBUG
#include <stdio.h>
#endif

#include "nzt.h"
#include "crt.h"
#include "globals.h"
#include "guid.h"
#include "utils.h"
#include "config.h"

DWORD	 g_CurrentProcessId		= 0;
LPWSTR	 g_CurrentProcessPath	= 0;
LPTSTR   g_CurrentProcessName	= 0;
HMODULE  g_CurrentProcessModule	= 0;
HMODULE  g_CurrentModule		= 0;

LPTSTR   g_BotGuid				= 0;
DWORD	 g_BotOS				= 0;
DWORD	 g_BotArch				= 0;
LPTSTR   g_BotUsername			= 0;
DWORD    g_BotGroupId			= 0;
LPWSTR   g_BotInstallPath		= 0;

HANDLE	 g_ShutdownEvent		= 0;
HANDLE   g_MainMutex			= 0;

VOID GlobalsRelease()
{
	if (g_CurrentProcessPath)
		Free(g_CurrentProcessPath);

	if (g_ShutdownEvent)
		API(CloseHandle(g_ShutdownEvent));
}

BOOL GlobalsInitialize(HMODULE CurrentModule, ULONG Flags)
{
	BOOL  Status				= TRUE;

	g_CurrentModule			= CurrentModule;
	g_CurrentProcessModule	= API(GetModuleHandleW)(NULL);
	g_CurrentProcessId		= API(GetCurrentProcessId)();

	g_BotGuid				= GetBotGuid();
	g_BotGroupId			= 0;
	g_BotUsername			= GetUsername();
	g_BotInstallPath		= NULL;

	do
	{
		/*if (Flags & G_SHUTDOWN_EVENT)
		{
			if (!(g_ShutdownEvent = CreateEvent(NULL, TRUE, FALSE, 0)))
			{
				Status = GetLastError();
				DebugPrint("Globals: Initialized ShutdownEvent failed with status: %u", Status);
				break;
			} // G_SHUTDOWN_EVENT
		}*/

		if (Flags & G_SYSTEM_VERSION)
		{
			g_BotOS = GetOperatingSystem();
			g_BotArch = Is64Bit();
		}

		if (Flags & G_CURRENT_PROCESS_ID)
			g_CurrentProcessId = API(GetCurrentProcessId)();

		if (Flags & G_CURRENT_PROCESS_PATH)
		{
			if (!GetModulePath(API(GetModuleHandleW)(NULL), &g_CurrentProcessPath, FALSE))
			{
				Status = FALSE;
				DebugPrint("NzT: Failed to initialize current process path!");
			}
		}
	} while (FALSE);

	if (Status != TRUE)
		GlobalsRelease();

	return Status;
}