// led_flash.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "led_flash.h"

void led_flash()
{
	uint8 i = 0;
	uint32 led_delay=1000;
	uint8 left_right=1;
	while (1)
	{
		i=led(left_right, i);
		delay(led_delay);
	}
}