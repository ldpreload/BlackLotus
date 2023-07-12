#pragma once

typedef struct __attribute__(( packed ))
{
	// ExitBootServicesHook / OslArchTransferToKernelHook
	EFI_EXIT_BOOT_SERVICES		ExitBootServices;
	PVOID				OslArchTransferToKernelGate;

	// DrvMain
	PVOID				KernelBuf;
	ULONG				KernelLen;
	PVOID				KernelBase;
	PVOID				TgtDrvImgSect;
	PVOID				TgtDrvImgBase;
	PVOID				TgtDrvLdrEntry;
	ULONG				TgtDrvAddressOfEntrypoint;
} EFTBL, *PEFTBL;
