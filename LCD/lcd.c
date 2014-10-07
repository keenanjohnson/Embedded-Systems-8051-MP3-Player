#include "lcd.h"

void lcd_write_cmd( uint8 cmd )
{
    RS = 0;
    E = 1;
    P0 = cmd;
    E = 0;
    P0 = 0xFF;
}

void lcd_write_data( uint8 data )
{
    RS = 1;
    E = 1;
    P0 = data;
    E = 0;
    P0 = 0xFF;
}

void lcd_init()
{
    // Set mode
    lcd_write_cmd( 0x38 );
    delay( 5 );
    
    // Turns on cursor
    lcd_write_cmd( 0x38 );
    delay( 5 );
    
    // Clears display
    lcd_write_cmd( 0x01 );
    delay( 5 );
    
    // Set entry mode
    lcd_write_cmd( 0x04 );
    delay( 5 );
}

void lcd_print_array( uint8 *array )
{
    uint8 *iterator;
    
    // Get start of array
    iterator = array;
    
    // Check for nul terminator
    while ( *iterator != NUL )
    {
        // Write data at that location
        lcd_write_data( *array );
        
        // Increment the iterator
        iterator++;
    }
}
