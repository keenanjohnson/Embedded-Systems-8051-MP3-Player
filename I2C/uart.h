// uart.h
//
// Contains UART initialization routine
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Sept. 2014

#ifndef _UART_H
#define _UART_H

#include "Main.h"

#define SMOD1 (0)
#define SPD (1)
#define BAUD_RATE (9600UL)
#define BRL_VALUE ((uint8)(256-((1+(5*SPD))*(1+(1*SMOD1))*OSC_FREQ/(32*OSC_PER_INST*BAUD_RATE))))

//////////////////////////
// Initializes UART
//////////////////////////
void uart_init();

#endif
