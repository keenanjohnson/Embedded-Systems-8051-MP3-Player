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

#include "i2c.h"
#include "Main.h"
#include "PORT.h"
#include "uart.h"

void main(void)
{
	//variables
	uint8 STA013 = 0x43;
	uint16 timeout_val = 600;
	uint16 timeout;
	uint8 error = 0;
	uint8 array_name[2];

	array_name[0] = 0x01;

	// Access more RAM
	AUXR = 0x0C;

	// Uart at 9600 baud
	uart_init();

	while(1)
	{
		// Test code from step 4
		// of the experiment
		timeout=timeout_val;
		do
		{
			error = I2C_Write( STA013, 1, array_name );
			timeout--;
		}while((error!=0)&&(timeout!=0));
		if(timeout!=0)
		{
			timeout=timeout_val;
			do
			{
				error=I2C_Read( STA013, 1, array_name );
				timeout--;
			}while((error!=0)&&(timeout!=0));
			printf("I2C Read: %2.2bX", array_name[0]);
		}
		else
		{
			printf("timeout error\n\r");
		}
	}
}

