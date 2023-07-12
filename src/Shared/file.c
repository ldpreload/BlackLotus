#include <Windows.h>

#include "file.h"
#include "crt.h"
#include "ntdll.h"
#include "nzt.h"
#include "utils.h"

BOOL FileGetInfo(HANDLE FileHandle, PFILE_STANDARD_INFORMATION Info)
{
	IO_STATUS_BLOCK IO;

	MemoryZero(&IO, sizeof(IO_STATUS_BLOCK));
	MemoryZero(Info, sizeof(FILE_STANDARD_INFORMATION));

	if (API(NtQueryInformationFile)(FileHandle, &IO, Info, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation) >= 0)
		return TRUE;

	return FALSE;
}

BOOL FileGetSize(HANDLE FileHandle, PDWORD FileSize)
{
	FILE_STANDARD_INFORMATION Info;

	*FileSize = 0;

	if (!FileGetInfo(FileHandle, &Info))
		return FALSE;

	*FileSize = Info.AllocationSize.LowPart;
	return TRUE;
}

BOOL FileOpen(HANDLE* FileHandle, CONST LPWSTR Path, ACCESS_MASK AccessMask, ULONG CreateDisposition)
{
	NTSTATUS			Status;
	UNICODE_STRING		US;
	OBJECT_ATTRIBUTES	OA;
	IO_STATUS_BLOCK		IO;
	BOOL				bStatus = FALSE;

	*FileHandle = INVALID_HANDLE_VALUE;

	MemoryZero(&IO, sizeof(IO_STATUS_BLOCK));
	MemoryZero(&OA, sizeof(OBJECT_ATTRIBUTES));

	OA.Length = sizeof(OBJECT_ATTRIBUTES);
	API(RtlInitUnicodeString)(&US, Path);
	OA.ObjectName = &US;
	OA.Attributes = OBJ_CASE_INSENSITIVE;

	Status = API(NtCreateFile(FileHandle, AccessMask | SYNCHRONIZE, &OA, &IO, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, CreateDisposition, FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0));

	if (NT_SUCCESS(Status))
		bStatus = TRUE;

	return bStatus;
}

BOOL FileWrite(HANDLE FileHandle, CONST LPVOID Buffer, DWORD Length)
{
	NTSTATUS		Status;
	IO_STATUS_BLOCK IO;
	Status = API(NtWriteFile)(FileHandle, NULL, NULL, NULL, &IO, &Buffer, Length, NULL, NULL);
	if (NT_SUCCESS(Status))
		return TRUE;

	return FALSE;
}

BOOL FileRead(HANDLE FileHandle, LPVOID* Buffer, DWORD Length, PDWORD ReadLength)
{
	IO_STATUS_BLOCK IO;
	LARGE_INTEGER	LI;

	LI.LowPart = 0;
	LI.HighPart = 0;

	if ((*Buffer = Malloc(Length)) == 0)
		return FALSE;

	if ( (FileHandle, 0, 0, 0, &IO, *Buffer, Length, &LI, 0) >= 0)
	{
		*ReadLength = IO.Information;
		return TRUE;
	}

	return FALSE;
}

BOOL FileWriteBuffer(CONST LPWSTR Path, CONST LPVOID Buffer, DWORD Length, BOOL Append)
{
	BOOL	Status = FALSE;
	HANDLE	FileHandle;

	if (!FileOpen(&FileHandle, Path, GENERIC_WRITE, FILE_OPEN_IF))
		return Status;

	Status = FileWrite(FileHandle, Buffer, Length);
	API(NtClose)(FileHandle);

	return Status;
}


BOOL FileReadBuffer(CONST LPWSTR Path, LPVOID* Buffer, PDWORD Length)
{
	BOOL   Status	  = FALSE;
	HANDLE FileHandle;
	DWORD  FileSize;

	if (!FileOpen(&FileHandle, Path, GENERIC_READ, FILE_OPEN))
		return Status;

	if (!FileGetSize(FileHandle, &FileSize))
		return Status;

	Status = FileRead(FileHandle, Buffer, FileSize, Length);
	return Status;
}

BOOL FileCreateDirectory(CONST LPWSTR Path)
{
	NTSTATUS			Status;
	IO_STATUS_BLOCK		IO;
	OBJECT_ATTRIBUTES	OA;
	UNICODE_STRING		US;
	HANDLE				Handle;
	BOOL				bStatus = FALSE;

	MemoryZero(&IO, sizeof(IO));
	MemoryZero(&OA, sizeof(OA));

	OA.Attributes	= OBJ_CASE_INSENSITIVE;
	OA.Length		= sizeof(OA);
	API(RtlInitUnicodeString)(&US, Path);
	OA.ObjectName	= &US;

	Status = API(NtCreateFile)(&Handle, GENERIC_WRITE, &OA, &IO, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE, FILE_DIRECTORY_FILE, NULL, 0);

	if (NT_SUCCESS(Status))
	{
		bStatus = TRUE;
		API(NtClose)(Handle);
	}

	return bStatus;
}

BOOL FileDelete(CONST LPWSTR Path)
{
	BOOL			  Status = FALSE;
	OBJECT_ATTRIBUTES OA;
	UNICODE_STRING    US;

	MemoryZero(&OA, sizeof(OBJECT_ATTRIBUTES));

	OA.Attributes = OBJ_CASE_INSENSITIVE;
	OA.Length = sizeof(OA);
	API(RtlInitUnicodeString)(&US, Path);
	OA.ObjectName = &US;

	if (API(NtDeleteFile)(&OA) >= 0)
		Status = TRUE;

	return Status;
}

BOOL FileCopy(CONST LPWSTR OriginalPath, CONST LPWSTR NewPath, BOOL DeleteOriginal)
{
	BOOL	Status	 = FALSE;
	LPVOID	File;
	DWORD	FileSize;

	if (!FileReadBuffer(OriginalPath, &File, &FileSize))
		return Status;

	if (!FileWriteBuffer(NewPath, File, FileSize, TRUE))
		return Status;

	if (DeleteOriginal)
		FileDelete(OriginalPath);

	Free(File);

	return Status;
}

BOOL IsValidNtPath(const LPWSTR Path)
{
	BOOL	Status = FALSE;
	LPWSTR	Data;

	if ((Data = StringCopyW(Path, 4)) != 0)
	{
		Status = StringCompareW(Path, L"\\??\\");
		Free(Data);
	}

	return Status;
}

BOOL DosPathToNtPath(LPWSTR* Path)
{
	LPWSTR NtPath = NULL;

	if (IsValidNtPath(*Path))
		return TRUE;

	if (StringConcatW(&NtPath, L"\\??\\") && StringConcatW(&NtPath, *Path))
	{
		Free(*Path);
		*Path = NtPath;
		return TRUE;
	}

	if (NtPath != NULL)
		Free(NtPath);

	return FALSE;
}