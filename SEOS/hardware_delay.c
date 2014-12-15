#include "main.h"
#include "PORT.h"
#include "hardware_delay.h"


// ***** Caution: Uses Timer0 *******//
void DELAY_MS_T0(uint16 duration)
{
   // Configure Timer 0 as a 16-bit timer 
   TMOD &= 0xF0;
   TMOD |= 0x01;

	  
   ET0 = 0;  // No interupts
      
   do
   {
     // Values for 1 ms delay
      TH0 = ReloadTH0;  // Timer 0 initial value (High Byte)
      TL0 = ReloadTL0;  // Timer 0 initial value (Low Byte)

      TF0 = 0;          // Clear overflow flag
      TR0 = 1;          // Start timer 0

      while (TF0 == 0); // Loop until Timer 0 overflows (TF0 == 1)

      TR0 = 0;          // Stop Timer 0
	  duration--;
	}while(duration>0);
}




// ***** Caution: Uses Timer1 *******//
void DELAY_40us_T0(void)
{
   // Configure Timer 0 as a 16-bit timer 
   TMOD &= 0xF0;
   TMOD |= 0x01;

	  
   ET0 = 0;  // No interupts
      
   // Values for 40us delay
   TH0 = Reload_40us_TH0;  // Timer 0 initial value (High Byte)
   TL0 = Reload_40us_TL0;  // Timer 0 initial value (Low Byte)

   TF0 = 0;          // Clear overflow flag
   TR0 = 1;          // Start timer 0

   while (TF0 == 0); // Loop until Timer 0 overflows (TF0 == 1)

   TR0 = 0;          // Stop Timer 0

}