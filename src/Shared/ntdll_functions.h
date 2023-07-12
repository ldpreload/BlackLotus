#ifndef _NT_FUNCTIONS_H_
#define _NT_FUNCTIONS_H_

#include <Windows.h>
#include <NTSecAPI.h>
#include "ntdll.h"

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef ULONG(WINAPI* ptRtlRandomEx)(
	_Inout_ PULONG Seed
	);

typedef NTSTATUS(WINAPI *ptRtlGetVersion)(
	PRTL_OSVERSIONINFOW lpVersionInformation
	);

typedef NTSTATUS(WINAPI* ptNtCreateUserProcess)(PHANDLE ProcessHandle, PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, ULONG ProcessFlags, ULONG ThreadFlags, PVOID ProcessParameters, PVOID CreateInfo, PVOID AttributeList);

typedef NTSTATUS(WINAPI* ptNtCreateThread)(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ProcessHandle, LPVOID ClientId, PCONTEXT ThreadContext, LPVOID InitialTeb, BOOLEAN CreateSuspended);

typedef NTSTATUS(WINAPI* ptNtQueryInformationProcess)(
	_In_      HANDLE           ProcessHandle,
	_In_      int ProcessInformationClass,
	_Out_     PVOID            ProcessInformation,
	_In_      ULONG            ProcessInformationLength,
	_Out_opt_ PULONG           ReturnLength
	);

#define STATUS_SUCCESS                    ((NTSTATUS)0x00000000)
#define RVATOVA( base, offset ) ( (DWORD)base + (DWORD)offset )

typedef NTSYSAPI NTSTATUS(NTAPI *ptNtMapViewOfSection)(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect);

typedef NTSTATUS(WINAPI* ptLdrLoadDll)(PWCHAR pathToFile, ULONG flags, PUNICODE_STRING moduleFileName, PHANDLE moduleHandle);

typedef NTSTATUS(WINAPI* ptLdrGetDllHandle)(
	IN PWORD                pwPath OPTIONAL,
	IN PVOID                Unused OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             pHModule);

typedef NTSTATUS(WINAPI* ptNtWriteVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN PVOID                Buffer,
	IN ULONG                NumberOfBytesToWrite,
	OUT PULONG              NumberOfBytesWritten OPTIONAL);

typedef NTSTATUS(WINAPI* ptNtAllocateVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN ULONG                ZeroBits,
	IN OUT PULONG           RegionSize,
	IN ULONG                AllocationType,
	IN ULONG                Protect);

typedef NTSTATUS(WINAPI* ptNtProtectVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN OUT PULONG           NumberOfBytesToProtect,
	IN ULONG                NewAccessProtection,
	OUT PULONG              OldAccessProtection);

typedef NTSTATUS(WINAPI* ptNtDeviceIoControlFile)(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN ULONG                IoControlCode,
	IN PVOID                InputBuffer OPTIONAL,
	IN ULONG                InputBufferLength,
	OUT PVOID               OutputBuffer OPTIONAL,
	IN ULONG                OutputBufferLength);

typedef NTSTATUS(WINAPI* ptNtSetContextThread)(
	IN HANDLE               ThreadHandle,
	IN PCONTEXT             Context);

typedef NTSTATUS(WINAPI* ptNtOpenProcess)(
	OUT PHANDLE             ProcessHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId);

typedef NTSTATUS(WINAPI* ptNtClose)(
	_In_ HANDLE Handle
	);

typedef NTSTATUS(WINAPI* ptNtCreateFile)(
	_Out_    PHANDLE            FileHandle,
	_In_     ACCESS_MASK        DesiredAccess,
	_In_     POBJECT_ATTRIBUTES ObjectAttributes,
	_Out_    PIO_STATUS_BLOCK   IoStatusBlock,
	_In_opt_ PLARGE_INTEGER     AllocationSize,
	_In_     ULONG              FileAttributes,
	_In_     ULONG              ShareAccess,
	_In_     ULONG              CreateDisposition,
	_In_     ULONG              CreateOptions,
	_In_     PVOID              EaBuffer,
	_In_     ULONG              EaLength
	);

typedef NTSTATUS(WINAPI* ptNtOpenFile)(
	_Out_ PHANDLE            FileHandle,
	_In_  ACCESS_MASK        DesiredAccess,
	_In_  POBJECT_ATTRIBUTES ObjectAttributes,
	_Out_ PIO_STATUS_BLOCK   IoStatusBlock,
	_In_  ULONG              ShareAccess,
	_In_  ULONG              OpenOptions
	);

typedef NTSTATUS(WINAPI* ptNtDeleteFile)(IN POBJECT_ATTRIBUTES ObjectAttributes);
typedef NTSTATUS(WINAPI* ptNtQueueApcThread)(HANDLE ThreadHandle, PIO_APC_ROUTINE ApcRoutine, PVOID ApcRoutineContext, PIO_STATUS_BLOCK ApcStatusBlock, ULONG ApcReserved);
typedef NTSTATUS(WINAPI* ptNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesReaded);
typedef NTSTATUS(WINAPI* ptNtQueryVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID Buffer, ULONG Length, PULONG ResultLength);
typedef NTSTATUS(WINAPI* ptNtOpenThread)(
	_Out_ PHANDLE            ThreadHandle,
	_In_  ACCESS_MASK        DesiredAccess,
	_In_  POBJECT_ATTRIBUTES ObjectAttributes,
	_In_  PCLIENT_ID         ClientId
	);
typedef NTSTATUS(WINAPI* ptNtQueryInformationThread)(
	HANDLE          ThreadHandle,
	int ThreadInformationClass,
	PVOID           ThreadInformation,
	ULONG           ThreadInformationLength,
	PULONG          ReturnLength
	);

typedef NTSTATUS(WINAPI* ptNtCreateSection)(
	_Out_    PHANDLE            SectionHandle,
	_In_     ACCESS_MASK        DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PLARGE_INTEGER     MaximumSize,
	_In_     ULONG              SectionPageProtection,
	_In_     ULONG              AllocationAttributes,
	_In_opt_ HANDLE             FileHandle
	);
typedef NTSTATUS(WINAPI* ptNtResumeThread)(
	IN HANDLE ThreadHandle,
	OUT PULONG PreviousSuspendCount
	);

typedef NTSTATUS(WINAPI* ptLdrInitializeThunk)(
	DWORD Unknown1,
	DWORD Unknown2,
	DWORD Unknown3
	);

typedef NTSTATUS(WINAPI* ptNtFreeVirtualMemory)(IN HANDLE ProcessHandle, IN PVOID *BaseAddress, IN OUT PULONG RegionSize, IN ULONG FreeType);
typedef NTSTATUS(WINAPI* ptNtFlushInstructionCache)(HANDLE hProcess, LPCVOID lpBaseAddress, SIZE_T dwSize);
typedef NTSTATUS(WINAPI* ptNtSetInformationThread)(IN HANDLE ThreadHandle, IN int ThreadInformationClass, IN PVOID ThreadInformation, IN ULONG ThreadInformationLength);
typedef NTSTATUS(WINAPI* ptNtQuerySystemInformation)(IN SYSTEM_INFORMATION_CLASS SystemInformationClass, OUT PVOID SystemInformation, IN ULONG SystemInformationLength, OUT PULONG ReturnLength OPTIONAL);
typedef NTSTATUS(WINAPI* ptLdrQueryProcessModuleInformation)(OUT PRTL_PROCESS_MODULES SystemModuleInformationBuffer, IN ULONG BufferSize, OUT PULONG RequiredSize OPTIONAL);
typedef VOID(WINAPI* ptRtlInitUnicodeString)(
	_Out_    PUNICODE_STRING DestinationString,
	_In_opt_ PCWSTR          SourceString
	);

typedef NTSTATUS(WINAPI* ptNtWriteFile)(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	IN PVOID                Buffer,
	IN ULONG                Length,
	IN PLARGE_INTEGER       ByteOffset OPTIONAL,
	IN PULONG               Key OPTIONAL);

typedef NTSTATUS(WINAPI* ptNtReadFile)(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               Buffer,
	IN ULONG                Length,
	IN PLARGE_INTEGER       ByteOffset OPTIONAL,
	IN PULONG               Key OPTIONAL);

typedef NTSTATUS(WINAPI* ptRtlStringCbPrintfA)(
	_Out_ LPTSTR  pszDest,
	_In_  size_t  cbDest,
	_In_  LPCTSTR pszFormat,
	...
	);

typedef NTSTATUS(WINAPI* ptNtDelayExecution)(
	IN BOOLEAN              Alertable,
	IN PLARGE_INTEGER       DelayInterval);

typedef NTSTATUS(NTAPI* ptNtOpenKey)(
	OUT PHANDLE             pKeyHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

typedef NTSTATUS(NTAPI* ptNtSetValueKey)(
	IN HANDLE               KeyHandle,
	IN PUNICODE_STRING      ValueName,
	IN ULONG                TitleIndex OPTIONAL,
	IN ULONG                Type,
	IN PVOID                Data,
	IN ULONG                DataSize);

typedef NTSTATUS(NTAPI* ptNtQueryValueKey)(
	IN HANDLE               KeyHandle,
	IN PUNICODE_STRING      ValueName,
	IN KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	OUT PVOID               KeyValueInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength);


typedef NTSTATUS(WINAPI* ptRtlFormatCurrentUserKeyPath)(
	_Out_ PUNICODE_STRING CurrentUserKeyPath
	);

typedef NTSTATUS(WINAPI* ptNtQueryInformationFile)(
	_In_  HANDLE                 FileHandle,
	_Out_ PIO_STATUS_BLOCK       IoStatusBlock,
	_Out_ PVOID                  FileInformation,
	_In_  ULONG                  Length,
	_In_  FILE_INFORMATION_CLASS FileInformationClass
	);


#endif // _NT_H_