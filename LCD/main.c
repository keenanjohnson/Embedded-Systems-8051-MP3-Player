// main.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// Submission for experiment 2
//
// CpE 4160
// FALL 2014

#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "led_flash.h"
#include "PORT.h"

void main(void)
{
	// Configures LCD
	lcd_init();

	while(1)
	{
		// Handles all input and output
	    led_flash();
	}
}
