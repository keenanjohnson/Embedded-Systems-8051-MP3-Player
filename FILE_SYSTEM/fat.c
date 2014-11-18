// fat.c
//
// Library for interacting with
// the FAT file system
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014
#include "fat.h"

uint8 mount_drive(void)
{
	// Variable declaration
	uint8 error_code = 0;
	xdata uint8 mem_block[512];
	uint8 value_8;
	uint32 value_32;

	// Print header
	print_newline();
	printf( "Mounting File System");
	print_newline();

	// Load sector zero
	error_code = load_sector( 0, mem_block );
	if ( error_code != 0 )
	{
		return error_code;
	}

	// Determine if this is the 
	// Bios Parameter Block
	// or the 
	// Master Boot Record
	value_8 = read8( 0, mem_block);
	if ( value_8 != 0xEB && value_8 != 0xE9 ) 
	{
		// This is the MBR
		// Read the relative sectors value
		value_32 = read32( 0x01C6, mem_block);
		print_newline();
		printf( "RelSec: %lu", value_32 );
		print_newline();
	
		// Load the BPB Sector
		error_code = load_sector( value_32, mem_block );
		if ( error_code != 0 )
		{
			return error_code;
		}
	}

	// Determine if this is the BPB sector
	// if offset 0 is not 0xEB or 0xE9
	// Then this is not the BPB
	value_8 = read8( 0, mem_block);
	if ( value_8 != 0xEB && value_8 != 0xE9 )
	{
		// This is not the BPB
		error_code = 2;
		printf("First byte of BPB is not 0xEB or 0xE9. Value: %2.2bx", value_8);
		print_newline();
		return error_code;
	}

	// Return error code
	return error_code;
}
