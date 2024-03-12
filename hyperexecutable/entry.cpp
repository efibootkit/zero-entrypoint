#include <Windows.h>
#include <stdio.h>
#include <iostream>

auto main( ) -> void
{
	printf( "test\n" );
	LoadLibrary( L"hypermodule.dll" );
	std::cin.get( );
}