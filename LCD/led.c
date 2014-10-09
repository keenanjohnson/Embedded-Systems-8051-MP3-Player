#include "led.h"

static uint8 code states[] = {0x80, 0x40, 0x20, 0x10};
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