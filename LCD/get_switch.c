// get_switch.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "get_switch.h"

uint8 get_switch(sbit name, uint32 delay_debounce)
{
	uint8 active=0;
	if (name == 0)
	{
		delay(delay_debounce);
		if (name == 0)
		{
			active=1;
		}
	}
	return active;
}