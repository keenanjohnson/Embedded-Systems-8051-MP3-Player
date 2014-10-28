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

#include "Main.h"
#include "PORT.h"
#include "uart.h"
//#include "sdcard.h"
#include "serial_ln.h"
#include "spi.h"

void main(void)
{
	// Uart at 9600 baud
	uart_init();

	// Setup SPI Hardware
	spi_master_init( 400000 );

	while(1)
	{
        spi_transfer( 0x55 );    
	}
}
