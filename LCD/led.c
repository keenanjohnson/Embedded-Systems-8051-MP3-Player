// led.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "led.h"

static uint8 code states[] = {0x8F, 0x4F, 0x2F, 0x1F};
static uint8 num_states = 4;

void led()
{
	uint8 i = 0;
	uint32 led_delay=1000;
	uint8 left_right=1;

	while (left_right == 1)
	{
		if ( i == num_states)
			i=0;

		P2=~states[i];
		i++;
		delay(led_delay);
	}

	while (left_right != 0)
	{
		if ( i < 0 )
			i= num_states-1;
		P2=~states[i];
		i--;
		delay(led_delay);
	}
}