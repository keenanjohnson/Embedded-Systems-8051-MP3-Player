#include led.h

static unsigned uint8 code state[] = {0x04, 0x03, 0x02, 0x01};
static unsigned uint8 num_states = 4;

void led()
{
	uint32 led_delay=1000;
	unsigned uint8 left_right=1;
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