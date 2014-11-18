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
	uint16 rsvdSectorCount;
	uint8 numberFATs;
	uint16 rootEntryCount;
	uint16 totalSectors16;
	uint16 fatSize16;
	uint32 totalSectors32;
	uint32 fatSize32;
	uint32 rootCluster;

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

	// Read the values from BIOS parameter block
	BYTESPERSECTOR = read16(0x0B, mem_block);
	SECTORSPERCLUSTER = read8(0x0D, mem_block);
	rsvdSectorCount = read16(0x0E, mem_block);
	numberFATs = read8(0x10, mem_block);
	rootEntryCount = read16(0x11, mem_block);
	totalSectors16 = read16(0x13, mem_block);
	fatSize16 = read16(0x16, mem_block);
	totalSectors32 = read32(0x20, mem_block);
	fatSize32 = read32(0x24, mem_block);
	rootCluster = read32(0x2C, mem_block);

	// Determine the count of sectors occupied by the root directory
	// Calculate RootDirSectors
	ROOTDIRECTORYSECTORS = ((RootEntCnt * 32) + (BytesPerSec_g - 1)) / BytesPerSec_g;

	// Determine FATSz and TotSec
	FATSz = FATSz16;
	if(FATSz == 0)
	{
		FATSz = FATSz32;
	}
	TotSec = TotSec16;
	if(TotSec == 0)
	{
		TotSec = TotSec32;
	}

	// Calculate DataSec
	DataSec = TotSec - (RsvdSecCnt + (NumFATs * FATSz) + RootDirSecs_g);

	// Calculate CountofClus
	CountofClus = DataSec / SecPerClus_g;

	// Determine FAT type
	if(CountofClus < 65525)
	{
		FATtype_g = FAT16;
		error_code = 1; // FAT16 unsupported
	}
	else
	{
		FATtype_g = FAT32;
	}

	// Assign globals
	StartofFAT_g = RsvdSecCnt + RelSec;
	FirstDataSec_g = RsvdSecCnt + (NumFATs * FATSz) + RootDirSecs_g + RelSec;
	FirstRootDirSec_g = RsvdSecCnt + (NumFATs * FATSz) + RelSec;

	// Return error code
	return error_code;
}
