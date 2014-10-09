// led_flash.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "led_flash.h"

void led_flash()
{
	int i = 0;
	uint16 led_delay=1000;
	uint8 right_left=0;
	
	// Prep Buttons
	init_buttons();

	// Print status
	lcd_print_led_status( right_left, led_delay );

	while (1)
	{
		// Button 1
		if ( get_buttons_1( debounce_delay ) )
		{
			// Sets LEDs to move left to right
			right_left=1;

			// Print status
			lcd_print_led_status( right_left, led_delay );
		}
		// Button 2
		else if ( get_buttons_2( debounce_delay ) )
		{
			// Increases time increment
			if (led_delay < 2000)
				led_delay+=100;

			// Print status
			lcd_print_led_status( right_left, led_delay );
		}
		// Button 3
		else if ( get_buttons_3( debounce_delay ) )
		{
			// Decreases time increment
			if (led_delay > 100)
				led_delay-=100;

			// Print status
			lcd_print_led_status( right_left, led_delay );
		}
		// Button 4
		else if (get_buttons_4( debounce_delay ) )
		{
			// Sets LEDs to move right to left
			right_left=0;

			// Print status
			lcd_print_led_status( right_left, led_delay );
		}

		// Lights LEDs
		i=led(right_left, i);

		// Delay for time increment
		delay(led_delay);
	}
}