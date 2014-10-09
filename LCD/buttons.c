// buttons.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "buttons.h"

void init_buttons()
{
	SW1 = 1;
	SW2 = 1;
	SW3 = 1;
	SW4 = 1;
}

uint8 get_buttons_1(uint16 debounce_delay)
{
	uint8 active=0;
	if (SW1 == 0)
	{
		delay(debounce_delay);
		if (SW1 == 0)
		{
			active=1;
		}
	}
	return active;
}

uint8 get_buttons_2(uint16 debounce_delay)
{
	uint8 active=0;
	if (SW2 == 0)
	{
		delay(debounce_delay);
		if (SW2 == 0)
		{
			active=1;
		}
	}
	return active;
}

uint8 get_buttons_3(uint16 debounce_delay)
{
	uint8 active=0;
	if (SW3 == 0)
	{
		delay(debounce_delay);
		if (SW3 == 0)
		{
			active=1;
		}
	}
	return active;
}

uint8 get_buttons_4(uint16 debounce_delay)
{
	uint8 active=0;
	if (SW4 == 0)
	{
		delay(debounce_delay);
		if (SW4 == 0)
		{
			active=1;
		}
	}
	return active;
}
