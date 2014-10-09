// lcd.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#ifndef _LCD_H
#define _LCD_H

#include "delay.h"
#include "Main.h"
#include "PORT.h"

#define NUL (0x00)

// Writes a command to the LCD screen
void lcd_write_cmd( uint8 cmd );

// Writes data to the LCD screen
void lcd_write_data( uint8 dat );

// Initliazes the LCD screen
void lcd_init();

// Prints a nul terminated character array to LCD screen
void lcd_print_array( uint8 *array );

// Clears the LCD screen
void lcd_clear();

#endif