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
	SPI_nCS0 = 0;
	send_command( 17 , 0 );
	read_sd_block( 512, mem_block );
	SPI_nCS0 = 1;

	// Determine if this is the 
	// Bios Parameter Block
	// or the 
	// Master Boot Record
	value_8 = read8( 0, mem_block);

	// Print the value
	printf( "MBR?: %2.2bx", value_8 );
	print_newline();

	// Read the relative sectors value
	value_32 = read32( 0x01C6, mem_block);

	printf( "RelSec: %lu", value_32 );
	print_newline();

	// Return error code
	return error_code;

}
