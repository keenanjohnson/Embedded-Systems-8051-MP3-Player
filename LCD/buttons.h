// buttons.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#ifndef _BUTTONS_H
#define _BUTTONS_H

#include "delay.h"
#include "PORT.h"

// Sets button port values to 1, making them inputs
void init_buttons();

// Gets status of button 1
uint8 get_buttons_1(uint16 debounce_delay);

// Gets status of button 2
uint8 get_buttons_2(uint16 debounce_delay);

// Gets status of button 3
uint8 get_buttons_3(uint16 debounce_delay);

// Gets status of button 4
uint8 get_buttons_4(uint16 debounce_delay);

#endif
