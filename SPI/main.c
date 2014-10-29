// main.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// Submission for experiment 3
//
// SD Card
//
// CpE 4160
// FALL 2014

#include <stdio.h>

#include "Main.h"
#include "PORT.h"
#include "delay.h"
#include "uart.h"
#include "print.h"
#include "sdcard.h"
#include "serial_ln.h"
#include "spi.h"

void main(void)
{
	//variables
	xdata uint8 mem_block[512];
	uint32 user_input;

	// Access more RAM
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

		printf( "User input: %2.2bX", user_input );

		//read a block of memory from SD card
		//read_block();
	}
}

