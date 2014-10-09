// led.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "led.h"

static uint8 code states[] = {0x80, 0x40, 0x20, 0x10};
static uint8 code num_states = 4;

uint8 led(uint8 left_right, uint8 index)
{
	if ( left_right == 1 )
	{
		P2=~states[index];
		index++;
		if ( index == num_states )
		{
			index=0;
		}
		return index;
	}
	P2=~states[index];
	index--;
	if ( index < 0 )
	{
		index=num_states-1;
	}
	return index;
}