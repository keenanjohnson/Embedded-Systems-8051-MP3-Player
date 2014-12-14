// delay.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#include "delay.h"

void delay( uint32 ms_to_delay )
{
    uint32 millisec;
    
    // Set up TMOD
    TMOD &= 0xF0;
    TMOD |= 0x01;
    
    ET0 = 0; // Disable timer 0 interrupt
    
    // Delay
    for(millisec = 0; millisec < ms_to_delay; millisec++)
    {
        TH0 = PRELOADH1; // Timer values
        TL0 = PRELOADL1;
        
        TF0 = 0; // clears overflow flag   
        TR0 = 1; // starts the timer
        
        while(TF0 == 0); // Check for overflow
        
        TR0 = 0; // Stop the timer
    }
}
