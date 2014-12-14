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
#include "delay.h"

extern code uint8 CONFIG;
extern code uint8 CONFIG2;
extern code uint8 CONFIG3;
uint8 timer1_reload_high;
uint8 timer1_reload_low;

void main(void)
{
	//variables
	uint8 STA013 = 0x43;
	uint16 timeout_val = 600;
	uint16 timeout;
	uint8 error = 0;
	uint8 I2C_data_array[2];
    uint8 * config_array;
    uint32 index = 0;
	// Access more RAM
	AUXR = 0x0C;

	// Uart at 9600 baud
	uart_init();
    P3_3 = 0;
    // Test code from step 4
    // of the experiment
    timeout=timeout_val;
    do
    {
        I2C_data_array[0] = 0x01;
        error = I2C_Write( STA013, 1, I2C_data_array );

        if (error==1)

        {

            // Bus busy error

            printf("Write - Bus Busy Error");

            print_newline();

        }

        if (error==2)

        {

            // NACK error

            printf("Write - NACK Error");

            print_newline();

        }
        timeout--;
    }while((error!=0)&&(timeout!=0));
    if(timeout!=0)
    {
        timeout=timeout_val;
        do
        {
            error=I2C_Read( STA013, 1, I2C_data_array );
            switch( error )
            {
                case 0:
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
        printf("I2C Read: %2.2bX", I2C_data_array[0]);

        print_newline();
    }
    else
    {
        printf("timeout error\n\r");
    }
	
    //Sending patch file CONFIG to STA013
    config_array = &CONFIG;
    printf("Sending CONFIG... ");
    while ( I2C_data_array[0] != 0xFF )
	{
        error = I2C_Write( STA013, 2, I2C_data_array );
        if ( error != 0 )
        {
            printf("I2C Write Error %bu", error);
            print_newline();
        }
        I2C_data_array[0] = config_array[index];
        index++;
        I2C_data_array[1] = config_array[index];
        index++;
    } 
    delay(1);
    printf("Complete");
    print_newline();
    
    index = 0;
    //Sending patch file CONFIG2
    config_array = &CONFIG2;
    printf("Sending CONFIG2... ");
    while ( I2C_data_array[0] != 0xFF )
	{
        error = I2C_Write( STA013, 2, I2C_data_array );
        if ( error != 0 )
        {
            printf("I2C Write Error %bu", error);
            print_newline();
        }
        I2C_data_array[0] = config_array[index];
        index++;
        I2C_data_array[1] = config_array[index];
        index++;       
    } 
    printf("Complete");
    print_newline();
    
    index = 0;
    //Sending patch file CONFIG3
    config_array = &CONFIG3;
    printf("Sending CONFIG3... ");
    while ( I2C_data_array[0] != 0xFF )
	{
        error = I2C_Write( STA013, 2, I2C_data_array );
        if ( error != 0 )
        {
            printf("I2C Write Error %bu", error);
            print_newline();
        }
        I2C_data_array[0] = config_array[index];
        index++;
        I2C_data_array[1] = config_array[index];
        index++;       
    } 
    printf("Complete");
    print_newline();
    P3_3 = 1;
    while (1)
    {
    
    }
}

