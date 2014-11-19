// main.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// Submission for experiment 4
//
// File System
//
// CpE 4160
// FALL 2014

#include <stdio.h>

#include "Main.h"
#include "PORT.h"
#include "uart.h"
#include "offset_read.h"
#include "print.h"
#include "sdcard.h"
#include "serial_ln.h"
#include "spi.h"
#include "fat.h"
#include "Directory_Functions.h"

////////////////////
// Global Variables
////////////////////
idata uint16 BYTESPERSECTOR;
idata uint8 SECTORSPERCLUSTER;
idata uint32 ROOTDIRECTORYSECTORS;
idata uint8 SECTORSPERCLUSTER;
idata uint8 FATTYPE;
idata uint32 STARTOFFAT;
idata uint32 FIRSTDATASECTOR;
idata uint32 FIRSTROOTDIRSEC;
idata uint16 ROOTENTRYCOUNT;
idata uint8 SDTYPE = 0;

void main(void)
{
	//variables
	xdata uint8 mem_block[512];
	idata uint32 user_input;
	idata uint32 current_directory = FIRSTROOTDIRSEC;
	idata uint32 directory_read;
	
	//0x80000000 if directory, 1 if file
	uint8 entry_type;

	// Access 1kB of RAM
	AUXR = 0x0C;

	// Uart at 9600 baud
	uart_init();

	// Setup SPI Hardware
	// at 400kHz
	spi_master_init( 400000 );

	// Init SD Card
	initialize_card();

	//Set SPI interface to max speed
	spi_master_init( 8000000 );

	// Mount Drive
	mount_drive();

	// Print first sector of root directory
	Print_Directory(FIRSTROOTDIRSEC, mem_block);
	
	while(1)
	{
		// Get user input
		printf( "Please select an entry to read:" );
		print_newline();
		user_input = long_serial_input();
		
		// Check that user input is < number of entries in
		// the root directory
		if (user_input > ROOTENTRYCOUNT)
		{
			printf( "Invalid Entry" );
			print_newline();
		}
		else
		{
			// Print Directory
			directory_read = Read_Dir_Entry(current_directory , user_input, mem_block);
			entry_type = directory_read & 0x10000000;
			if ( (directory_read & 0x80000000) == 0x80000000 )
			{
				printf("Directory_Read: Fatal Read Error");
				print_newline();
			}
			else
			{
				// Check if entry is a file or directory
				if ( entry_type == 0x80 )
				{
					// This is a directory 
					// Find the first sector
					current_directory = First_Sector(directory_read);
				}
				else if ( entry_type == 0x00 )
				{
					// This is a file
				}
			}
		}
	}
}

