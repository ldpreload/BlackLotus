#include <Windows.h>

#include "config.h"

LPTSTR			g_Hosts[]		= { "127.0.0.1", "10.0.0.65", NULL};
UCHAR			g_NumberHosts	= (UCHAR)(sizeof(g_Hosts) / sizeof(LPTSTR));