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

	while(1)
	{
		// Get user input
		printf( "Please enter a block to read:" );
		print_newline();
		user_input = long_serial_input();

		// Set chip select low
		SPI_nCS0 = 0;

		// Send CMD 17
		send_command( 17 , user_input );

		// Receive data block
		read_sd_block( 512, mem_block );

		// Set Chip Select High
		SPI_nCS0 = 1;

		// Print Receive block
		print_mem_block( mem_block, 512 );
	}
}

