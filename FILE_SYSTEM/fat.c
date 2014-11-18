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
	uint16 rsvdSectorCount;
	uint8 numberFATs;
	uint16 rootEntryCount;
	uint16 totalSectors16;
	uint16 fatSize16;
	uint32 totalSectors32;
	uint32 fatSize32;
	uint32 rootCluster;
	uint32 fatSize;
	uint32 totalSectors;
	uint32 dataSec;
	uint32 countOfClusters;
	uint32 relSec;

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
		relSec = read32( 0x01C6, mem_block);
		print_newline();
		printf( "RelSec: %lu", relSec );
		print_newline();
	
		// Load the BPB Sector
		error_code = load_sector( relSec, mem_block );
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
	ROOTDIRECTORYSECTORS = ((rootEntryCount * 32) + (BYTESPERSECTOR - 1)) / BYTESPERSECTOR;

	// Determine FAT Size and Total Sectors
	fatSize = fatSize16;
	if(fatSize == 0)
	{
		fatSize = fatSize32;
	}
	totalSectors = totalSectors16;
	if(totalSectors == 0)
	{
		totalSectors = totalSectors32;
	}

	// Calculate DataSec
	dataSec = totalSectors - (rsvdSectorCount + (numberFATs * fatSize) + ROOTDIRECTORYSECTORS);

	// Calculate CountofClus
	countOfClusters = dataSec / SECTORSPERCLUSTER;

	// Determine FAT type
	if(countOfClusters < 65525)
	{
		// FAT16 unsupported
		FATTYPE = 2;
		error_code = 1;
	}
	else
	{
		// FAT32
		FATTYPE = 4;
	}

	// Calculate globals
	STARTOFFAT = rsvdSectorCount + relSec;
	FIRSTDATASECOR = rsvdSectorCount + (numberFATs * fatSize) + ROOTDIRECTORYSECTORS + relSec;
	FIRSTROOTDIRSEC = rsvdSectorCount + (numberFATs * fatSize) + relSec;

	// Return error code
	return error_code;
}