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
#include "print.h"

uint8 timer1_reload_high;
uint8 timer1_reload_low;

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

	// Set I2C Frequency
	set_frequency( 100000 );

	while(1)
	{
		// Test code from step 4
		// of the experiment
		timeout=timeout_val;
		do
		{
			printf("Writing to STA013");
			print_newline();
			error = I2C_Write( STA013, 1, array_name );
			timeout--;
		}while((error!=0)&&(timeout!=0));
		if(timeout!=0)
		{
			timeout=timeout_val;
			do
			{
				printf("Reading from STA013");
				print_newline();
				error=I2C_Read( STA013, 1, array_name );
				switch( error )
				{
					case 0:
					printf("Read successful");
					print_newline();
					break;
					case 1:
					printf("I2C Read Error: Bus Busy");
					print_newline();
					break;
					case 2:
					printf("I2C Read Error: Slave Device timeout");
					print_newline();
					break;
					case 3:
					printf("I2C Read Error: Improper ACK");
					print_newline();
					break;
					default:
					printf("Unspecified Error");
					print_newline();
					break;
				}
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

