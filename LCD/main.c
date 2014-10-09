// main.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "PORT.h"

void main(void)
{

	lcd_init();

	while(1)
	{
		REDLED = 0;
		delay( 500 );
		REDLED = 1;
		delay( 500 );
	    
		lcd_write_data( 0x41 );
	}
}
