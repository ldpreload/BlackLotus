#include <Windows.h>
#include <stdio.h>

#include "..\Shared\crt.c"
#include "..\Shared\api.c"
#include "..\Shared\utils.c"
#include "..\Shared\crypto.c"
#include "..\Shared\registry.c"

NzT_T NzT;

int main(int argc, char **argv)
{
	CHAR  Buffer[256] = { 0 };
	DWORD Hash = 0;

	if (argc < 2)
	{
		printf("Usage: Encryptor.exe [-a,-s,-x]\n -a [function name]\n\t-Get Crc32 hash of API function\n -s [string]\n\t-Encrypt string with RC4");
	}

	MemoryZero(&NzT, sizeof(NzT_T));
	MemoryZero(&Buffer, sizeof(Buffer));

	if (!ApiInitialize())
		return -1;

	if (!strcmp(argv[1], "-a"))
	{
		if (argv[2] == NULL)
			return -1;

		Hash = Crc32Hash(argv[2], strlen(argv[2]));
		wsprintfA(Buffer, "%s 0x%lX", CharUpperA(argv[2]), Hash);
	}

	printf("%s\n", Buffer);
	return 0;
}
