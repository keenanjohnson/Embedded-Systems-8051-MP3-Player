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

////////////////////
// Global Variables
////////////////////
uint16 BYTESPERSECTOR;
uint8 SECTORSPERCLUSTER;
uint32 ROOTDIRECTORYSECTORS;
uint8 SECTORSPERCLUSTER;
uint8 FATTYPE;
uint32 STARTOFFAT;
uint32 FIRSTDATASECTOR;
uint32 FIRSTROOTDIRSEC;
uint16 rootEntryCount;

void main(void)
{
	//variables
	xdata uint8 mem_block[512];
	uint32 user_input;

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

	Print_Directory(FIRSTROOTDIRSEC, mem_block);
	
	while(1)
	{
		// Get user input
		printf( "Please select an entry to read:" );
		print_newline();
		user_input = long_serial_input();
		
		if (user_input > rootEntryCount)
		{
			printf( "Invalid Entry" );
			print_newline();
		}
		else
		{
			// Print Directory
			Read_Dir_Entry( , user_input, mem_block);
		}
	}
}

