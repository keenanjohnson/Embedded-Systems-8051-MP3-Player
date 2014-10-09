// led_flash.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#ifndef _LED_FLASH_H
#define _LED_FLASH_H

#include "buttons.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "main.h"
#include "PORT.H"

#define debounce_delay 20

// Handles flashing LEDs and displaying on the LCD
// Polls buttons and changes parameters accordingly
void led_flash();

#endif