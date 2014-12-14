#ifndef _UART_H
#define _UART_H

#include "Main.H"

//------- Public Constant definitions --------------------------------
// UART settings
#define SMOD1 (0)
#define SPD (1)
#define BAUD_RATE (9600UL)
#define BRL_value ((uint8) (256-((1+(5*SPD))*(1+(1*SMOD1))*OSC_FREQ)/(32*OSC_PER_INST*BAUD_RATE)))

// ASCII characters
#define space (0x20)
#define CR (0x0D)
#define LF (0x0A)
#define BS (0x08)
#define DEL (0x7F)

// ------ Public function prototypes -------------------------------
void uart_init(void);



#endif