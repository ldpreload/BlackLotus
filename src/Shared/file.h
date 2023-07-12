#ifndef __FILE_H__
#define __FILE_H__

BOOL FileGetInfo(HANDLE FileHandle, PFILE_STANDARD_INFORMATION Info);
BOOL FileGetSize(HANDLE FileHandle, PDWORD FileSize);
BOOL FileOpen(HANDLE* FileHandle, CONST LPWSTR Path, ACCESS_MASK AccessMask, ULONG CreateDisposition);
BOOL FileWrite(HANDLE FileHandle, CONST LPVOID Buffer, DWORD Length);
BOOL FileRead(HANDLE FileHandle, LPVOID* Buffer, DWORD Length, PDWORD ReadLength);
BOOL FileWriteBuffer(CONST LPWSTR Path, CONST LPVOID Buffer, DWORD Length, BOOL Append);
BOOL FileReadBuffer(CONST LPWSTR Path, LPVOID* Buffer, PDWORD Length);
BOOL FileCreateDirectory(const LPWSTR Path);
BOOL FileDelete(CONST LPWSTR Path);
BOOL FileCopy(CONST LPWSTR OriginalPath, CONST LPWSTR NewPath, BOOL DeleteOriginal);
BOOL DosPathToNtPath(LPWSTR* Path);

#endif