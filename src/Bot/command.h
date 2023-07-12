#ifndef __COMMAND_H__
#define __COMMAND_H__

typedef enum
{
	COMMAND_DL_EXEC	    = 0x992c01,
	COMMAND_UPDATE		= 0xba1af8,
	COMMAND_LOAD_PLUGIN	= 0xbfc330,
	COMMAND_KILL		= 0x2fe86c,
	COMMAND_UNINSTALL	= 0xa2327f
} COMMANDS;

VOID CommandExecute(
	COMMANDS Command,
	PCHAR*	 Parameter
);


#endif //__COMMAND_H__