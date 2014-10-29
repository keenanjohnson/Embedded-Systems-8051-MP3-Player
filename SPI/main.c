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
	// Variable declaration
	uint32 user_block;

	// Uart at 9600 baud
	uart_init();

	// Setup SPI Hardware
	// at 400kHz
	spi_master_init( 400000 );

	// Init SD card
	initalize_card();

	//Increase SPI to 
	// 25 MHz
	spi_master_init( 25000000 );

	while(1)
	{
    	// Ask user for block number to read
		printf( "Enter block number: " );
		
		// Read in block number
    	user_block = long_serial_input();
		
		// Send command 17

		// Read block
		//uint8 read_block( uint16 number_of_bytes, uint8 *array )
		
		// print Block
		//print_line( uint8 *first_byte )       
	}
}
