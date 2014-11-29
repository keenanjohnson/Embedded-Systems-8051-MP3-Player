// main.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// Submission for experiment 5
//
// I2C and MP3 Decoder
//
// CpE 4160
// FALL 2014

#include <stdio.h>

#include "Main.h"
#include "PORT.h"
#include "uart.h"

void main(void)
{
	//variables

	// Access more RAM
	AUXR = 0x0C;

	// Uart at 9600 baud
	uart_init();

	while(1)
	{
	}
}

