// fat.c
//
// Library for interacting with
// the FAT file system
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Nov. 2014
#include "fat.h"

uint8 mount_drive(void)
{
	// Variable declaration
	idata uint8 error_code = 0;
	xdata uint8 mem_block[512];
	idata uint8 value_8;
	idata uint16 rsvdSectorCount;
	idata uint8 numberFATs;
	idata uint16 totalSectors16;
	idata uint16 fatSize16;
	idata uint32 totalSectors32;
	idata uint32 fatSize32;
	idata uint32 rootCluster;
	idata uint32 fatSize;
	idata uint32 totalSectors;
	idata uint32 dataSec;
	idata uint32 countOfClusters;
	idata uint32 relSec;

	// Print header
	print_newline();
	printf( "Mounting File System");
	print_newline();

	// Load sector zero
	error_code = load_sector( 0, 512, mem_block );
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
		error_code = load_sector( relSec, 512, mem_block );
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
	ROOTENTRYCOUNT = read16(0x11, mem_block);
	totalSectors16 = read16(0x13, mem_block);
	fatSize16 = read16(0x16, mem_block);
	totalSectors32 = read32(0x20, mem_block);
	fatSize32 = read32(0x24, mem_block);
	rootCluster = read32(0x2C, mem_block);

	// Determine the count of sectors occupied by the root directory
	// Calculate RootDirSectors
	ROOTDIRECTORYSECTORS = ((ROOTENTRYCOUNT * 32) + (BYTESPERSECTOR - 1)) / BYTESPERSECTOR;

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
		printf( "FAT16" );
		print_newline();
		error_code = 1;
	}
	else
	{
		// FAT32
		printf( "FAT32" );
		print_newline();
		FATTYPE = 4;
	}

	// Calculate globals
	STARTOFFAT = rsvdSectorCount + relSec;
	FIRSTDATASECTOR = rsvdSectorCount + (numberFATs * fatSize) + ROOTDIRECTORYSECTORS + relSec;
	FIRSTROOTDIRSEC = rsvdSectorCount + (numberFATs * fatSize) + relSec;

	// Print Globals
	printf( "Start of FAT: %lu", STARTOFFAT );
    print_newline();
    printf( "First Data Sector: %lu", FIRSTDATASECTOR );
	print_newline();
	printf( "First Root Directory Sector: %lu", FIRSTROOTDIRSEC );
	print_newline();

	// Return error code
	return error_code;
}

uint32 First_Sector(uint32 clusterNum)
{	
	// Declare return value
	uint32 firstSector;

	
	if( clusterNum == 0 )
	{
		firstSector = FIRSTROOTDIRSEC;
	}
	else
	{
		firstSector = ((clusterNum - 2) * SECTORSPERCLUSTER) + FIRSTDATASECTOR;
	}

	// Return
	return firstSector;	
}
 uint32 Find_Next_Clus(uint32 cluster_num, uint8 xdata * array)
{
	// Variable declaration
	uint8 error_code = 0;
	uint32 offset;
	uint32 current_sector;
	uint32 current_offset;
	uint32 value32;
	//uint16 value16;
	
	// Calculate the sector number in which the cluster number entry can be found
	offset = cluster_num * 4;

	current_sector = STARTOFFAT + (offset / BYTESPERSECTOR);
	printf("Current Sector = %lu",current_sector);
	print_newline();
	
	// Read the 512 block of the current sector
	error_code = load_sector( current_sector, BYTESPERSECTOR, array );
	if ( error_code != 0 )
	{
		printf( "Error loading current sector in Find Next Cluster" );
		print_newline();
		return 0;
	}

	// Calculate the offset address for the cluster number entry
	current_offset = offset&(BYTESPERSECTOR-1);
	printf("Current Offset = %lu",current_offset);
	print_newline();

	// Read the next cluster value
	value32 = read32(current_offset, array);
	printf("Next Cluster32 Number = %lu", value32);
	print_newline();

	// Return
	return ( value32 & 0x0FFFFFFF );
}

uint8 Open_File(uint32 Cluster, uint8 xdata *array_in)
{
    // Variable declaration
    uint8 error_code = 0;
    uint32 currentCluster;
	uint8 userStop = 0;
	uint8 sectorOffset = 0;
	uint8 input;
    
    // Get first cluster of file
    currentCluster =  First_Sector(Cluster);

	printf("First Sector: %lu", currentCluster);
	print_newline();
    
	// Print file segment and poll user
	while( ( error_code == 0 ) && ( userStop == 0 ) )
    {
		// Load sector
		error_code = load_sector( (currentCluster + sectorOffset), BYTESPERSECTOR, array_in );

		// No error
		if ( error_code == 0 )
		{
			// Print sector
			//print_mem_block( array_in, BYTESPERSECTOR );

			// Prompt user to continue or stop
			//printf("Press Esc to stop, anything else to continue...");
			//print_newline();

			// Get user value
			//scanf("%c", &input);
			input = 0x50;
			
			// Check for escape character
			if(input == 0x1B)
			{
				// Escape Key
				userStop = 1;
			}
			else // Not escape key
			{
				// Increment offset
				sectorOffset++;

				// Check for end of cluster
				if ( sectorOffset == (SECTORSPERCLUSTER-1) )
				{
					// Set sector offset back to 0
					sectorOffset = 0;

					// Set cluster to the next cluster
					currentCluster = Find_Next_Clus(currentCluster, array_in);
					
					printf("Sector Cluster End Reached. Press Any key to continue.");
					print_newline();
					printf("Current Cluster: %lu",currentCluster);
					print_newline();
					scanf("%c", &input);
					
					// Check for End of File
					if ( currentCluster == 0xFFFFFFFF ) 
					{
						// End of file detected
						// Stop the loop
						userStop = 1;
						printf( "End of File" );
						print_newline();
					}
				}
			}
		}
		// Error condition
		else
		{
			printf( "Error loading sector in OpenFile" );
			print_newline();
		}
	}
    
	// Return
    return error_code;
}

