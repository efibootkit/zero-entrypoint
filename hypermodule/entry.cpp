#include "structures.h"

typedef struct internal_struct
{
	uintptr_t RtlAcquirePebLock_addr;
	uintptr_t RtlReleasePebLock_addr;
	uintptr_t PEBAddr;
	PPEB_LDR_DATA PEBLdrAddr;
};

auto unlink_module( const char* mod_name ) -> void
{
	AllocConsole( );
	freopen( "CONOUT$", "w", stdout );
	printf( "[+] Console Allocated\n" );

	internal_struct PEBStructLoc{ };

	PEBStructLoc.RtlAcquirePebLock_addr = reinterpret_cast< uintptr_t >( GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "RtlAcquirePebLock" ) );
	PEBStructLoc.RtlReleasePebLock_addr = reinterpret_cast< uintptr_t >( GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "RtlReleasePebLock" ) );

	printf( "[+] RtlAcquirePebLock -> %p\n", reinterpret_cast< void* >( PEBStructLoc.RtlAcquirePebLock_addr ) );
	printf( "[+] RtlReleasePebLock -> %p\n", reinterpret_cast< void* >( PEBStructLoc.RtlReleasePebLock_addr ) );

	PEBStructLoc.PEBAddr = __readgsword( 0x60 );
	cout << "[+] PEBAddr -> " << PEBStructLoc.PEBAddr << endl;

	reinterpret_cast< void( __stdcall* )( void ) >( PEBStructLoc.RtlAcquirePebLock_addr )( );
	printf( "[+] Called RtlAcquirePebLock@ntdll.dll successfully \n" );

	PEBStructLoc.PEBLdrAddr = reinterpret_cast< PPEB_LDR_DATA >( reinterpret_cast< PPEB >( PEBStructLoc.PEBAddr )->Ldr );
	printf( "[+] Imported PEB->Ldr to PEBLdr \n" );

	if ( PEBStructLoc.PEBAddr != 0 && PEBStructLoc.RtlAcquirePebLock_addr != 0 && PEBStructLoc.RtlReleasePebLock_addr != 0 )
	{
		reinterpret_cast< void( __stdcall* )( void ) >( PEBStructLoc.RtlAcquirePebLock_addr )( );
		printf( "[+] RtlAcquirePebLock re-call  \n" );

		// error is caused by the following line for some reasons
		PEBStructLoc.PEBLdrAddr = reinterpret_cast< PPEB_LDR_DATA >( reinterpret_cast< PPEB >( PEBStructLoc.PEBAddr )->Ldr );
		PLIST_ENTRY current_mod_entry = PEBStructLoc.PEBLdrAddr->InMemoryOrderModuleList.Flink;

		printf( "[+] Looping PEBLdrAddr->InLoadOrderModuleList  \n" );
		while ( current_mod_entry != &PEBStructLoc.PEBLdrAddr->InLoadOrderModuleList )
		{
			printf( "[+] Loop:PEBLdrAddr->InLoadOrderModuleList:Success  \n" );

			PLDR_MODULE mod_entry = reinterpret_cast< PLDR_MODULE >( current_mod_entry );

			int buffer_size = WideCharToMultiByte( CP_ACP, 0, mod_entry->BaseDllName.Buffer, mod_entry->BaseDllName.Length / sizeof( wchar_t ), nullptr, 0, nullptr, nullptr );
			char* narrow_string = new char[ buffer_size + 1 ];
			WideCharToMultiByte( CP_ACP, 0, mod_entry->BaseDllName.Buffer, mod_entry->BaseDllName.Length / sizeof( wchar_t ), narrow_string, buffer_size, nullptr, nullptr );
			narrow_string[ buffer_size ] = '\0';

			printf( "[+] wow \n" );

			cout << narrow_string << endl;
			if ( strcmp( narrow_string, mod_name ) == 0 )
			{
				cout << "->modified" << endl;

				ZeroMemory( &mod_entry->BaseAddress, sizeof( mod_entry->BaseAddress ) );
				ZeroMemory( &mod_entry->EntryPoint, sizeof( mod_entry->EntryPoint ) );

				mod_entry->HashTableEntry.Blink->Flink = mod_entry->HashTableEntry.Flink;
				mod_entry->HashTableEntry.Flink->Blink = mod_entry->HashTableEntry.Blink;

				current_mod_entry->Blink->Flink = current_mod_entry->Flink;
				current_mod_entry->Flink->Blink = current_mod_entry->Blink;
			}

			cout << "ModuleEntry:Blink -> " << hex << reinterpret_cast< int >( mod_entry->HashTableEntry.Blink ) << endl;
			cout << "ModuleEntry:Flink -> " << hex << reinterpret_cast< int >( mod_entry->HashTableEntry.Flink ) << endl;
			cout << "ModuleEntry:bAddr -> " << hex << reinterpret_cast< int >( mod_entry->BaseAddress ) << endl;
			cout << "ModuleEntry:eAddr -> " << hex << reinterpret_cast< int >( mod_entry->EntryPoint ) << endl;
			cout << endl;

			current_mod_entry = mod_entry->InLoadOrderModuleList.Flink;
		}

		reinterpret_cast< void( __stdcall* )( void ) >( PEBStructLoc.RtlReleasePebLock_addr )( );
	}
}

auto DllMain( HMODULE hmod, DWORD call, LPVOID reserved ) -> BOOL APIENTRY
{
	if ( call == DLL_PROCESS_ATTACH )
	{
		unlink_module( "hypermodule.dll" );
	}

	return TRUE;
}