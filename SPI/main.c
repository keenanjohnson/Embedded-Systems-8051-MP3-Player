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
	uint16 spi_error =0;

	// Uart at 9600 baud
	uart_init();

	// Setup SPI Hardware
	// at 400kHz
	spi_master_init( 400000 );

	while(1)
	{
		spi_error =	spi_transfer( 0x55 ); 
   	    printf("SPI Value: %u", spi_error);
		print_newline();
	}
}
