#ifndef __SHARED_H__
#define __SHARED_H__

#include "..\Shared\config.c"
#include "..\Shared\crt.c"
#include "..\Shared\api.c"
#include "..\Shared\utils.c"
#include "..\Shared\crypto.c"
#include "..\Shared\guid.c"
#include "..\Shared\file.c"
#include "..\Shared\registry.c"

#ifdef _DEBUG
#else
#pragma function(memset)
void *memset(void *dest, int c, size_t count)
{
	char *bytes = (char *)dest;
	while (count--)
	{
		*bytes++ = (char)c;
	}
	return dest;
}

#pragma function(memcpy)
void *memcpy(void *dest, const void *src, size_t count)
{
	char *dest8 = (char *)dest;
	const char *src8 = (const char *)src;
	while (count--)
	{
		*dest8++ = *src8++;
	}
	return dest;
}
#endif

#endif	//__SHARED_H__