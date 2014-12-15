#include "main.h"
#include "LCD_Routines.h"
#include "PORT.H"
#include "hardware_delay.h"


/***********************************************************************
DESC:    Writes data to the LCD with proper order for RS and E
INPUT: Uint8 for RS and Data to be written
       RS=0 instruction, RS=1 data
RETURNS: Nothing
CAUTION: None
************************************************************************/
void LCD_Write(bit RegSelect, uint8 LCD_Data)
{
  RS=RegSelect;  // Set or Clear RS before E is Set
  E=1;           // Set Enable before data written
  P0=LCD_Data;   
  E=0;
  P0=0xFF;
  RS=1;
}





/***********************************************************************
DESC:    Initializes LCD
INPUT: Nothing
RETURNS: Nothing
CAUTION: Uses timer 0 in delay functions
************************************************************************/
void LCD_Init(void)
{
  LCD_Write(COMMAND,function_set_8bit);   // 8-bit interface, 2 lines, 5x8 font
  DELAY_MS_T0(10);
  LCD_Write(COMMAND,function_set_8bit);
  DELAY_MS_T0(5);
  LCD_Write(COMMAND,function_set_8bit);
  DELAY_MS_T0(1);
  LCD_Write(COMMAND,function_set_8bit|two_line|set_5x8);
  DELAY_MS_T0(1);
  LCD_Write(COMMAND,display_control|display_on|cursor_off|blink_off);   // Display on, Cursor On and Not blinking
  DELAY_MS_T0(1);
  LCD_Write(COMMAND,clear_display);   // Clear Display
  DELAY_MS_T0(5);         // Must wait until this is complete
  LCD_Write(COMMAND,entry_mode|shift_right|cursor);   // DDRAM address increments with no scrolling
  DELAY_MS_T0(1);

}


void LCD_Print(uint8 num_bytes, uint8 *array_name)
{
   uint8 idata index,character;
   if(num_bytes==0)
   {
      index=0;
      character=*(array_name+index);
      while(character!=0)
      {
         LCD_Write(DDRAM,character);
         DELAY_40us_T0();
         index++;
         character=*(array_name+index);
      }
    }
    else
    {
       for(index=0;index<num_bytes;index++)
       {
          character=*(array_name+index);
          LCD_Write(DDRAM,character);
          DELAY_40us_T0();
       }
    }     
}

