// seos.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// Submission for experiment 6
//
// Simple Embedded Operating System
//
// CpE 4160
// FALL 2014

#include "seos.h"

void seos_init( void )
{
    Timer2_ISR_Init();
}

void seos_run( void )
{
    // Init
    seos_init();
    
    // Loop forever
    while(1);
}
