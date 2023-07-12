#include <Windows.h>

#include "antidebug.h"
#include "nzt.h"
#include "utils.h"

BOOL IsBeingDebuggedAlt()
{
	return GetPeb()->BeingDebugged;
}

WINERROR IsBeingDebugged()
{
	WINERROR Status = NO_ERROR;

	if (IsBeingDebuggedAlt())
		return ERROR_UNSUCCESSFULL;

	return Status;
}