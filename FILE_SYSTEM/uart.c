// uart.c
//
// Contains UART initialization routine
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Sept. 2014

#include "uart.h"

void uart_init()
{   
    // Power Control Register
    PCON |= (SMOD1 << 7);
    PCON &= ((SMOD1 << 7) | 0x3F);

	// Set up UART in mode 2
    SCON = 0x50;
    
    // Baud Rate Control Register
    BDRCON = 0;
    BRL = BRL_VALUE;
    BDRCON = (0x1C | (SPD << 1));
    
    // Disable serial com interrupt
    ES = 0;
    
    // Set UART to ready
    TI = 1;
}
