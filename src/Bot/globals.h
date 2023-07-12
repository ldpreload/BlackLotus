#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define G_SYSTEM_VERSION		1		// OS version
#define	G_CURRENT_PROCESS_ID	2		// Current process ID
#define G_CURRENT_MODULE_PATH	4		// Current module full path for DLL (equal to G_PROCESS_MODULE_PATH for EXE)
#define G_CURRENT_PROCESS_PATH	8		// Current process module full path (for both DLL and EXE)
#define G_SHUTDOWN_EVENT	    0x10	// Application shutdown event


extern	DWORD	g_CurrentProcessId;
extern	LPWSTR	g_CurrentProcessPath;
extern	LPTSTR	g_CurrentProcessName;

extern	LPTSTR	g_BotGuid;
extern	DWORD	g_BotOS;
extern	DWORD	g_BotArch;
extern	LPTSTR	g_BotUsername;
extern	LPWSTR  g_BotInstallPath;

extern	HANDLE	g_ShutdownEvent;
extern  HANDLE  g_MainMutex;

VOID	GlobalsRelease();
BOOL GlobalsInitialize(HMODULE CurrentModule, ULONG Flags);

#endif //__GLOBALS_H__