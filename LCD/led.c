// led.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "led.h"

static uint8 code states[] = {0x80, 0x40, 0x20, 0x10};
static uint8 code num_states = 4;

uint8 led(uint8 right_left, int index)
{
	if ( right_left == 0 )
	{
		// Writes value to port
		P2=~states[index];

		// Increments index
		index++;

		// Check for rollover
		if ( index == num_states )
		{
			// Reset Index
			index = 0;
		}
	}
	else
	{
		// Write values to port
		P2=~states[index];

		// Decrements index
		index--;
	
		// Check for rollover
		if ( index < 0 )
		{
			// Reset index
			index=num_states-1;
		}
	}

	return index;
}