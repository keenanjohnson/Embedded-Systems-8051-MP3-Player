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
	init_switch(SW1);
	init_switch(SW2);
	init_switch(SW3);
	init_switch(SW4);
	while (1)
	{
		if (get_switch(SW1))
		{
			left_right=1;
		}
		else if (get_switch (SW2))
		{
			if (led_delay < 2000)
				led_delay+=100;
		}
		else if (get_switch (SW3))
		{
			if (led_delay > 100)
				led_delay-=100;
		}
		else if (get_switch (SW4))
		{
			left_right=0;
		}
		i=led(left_right, i);
		delay(led_delay);
	}
}