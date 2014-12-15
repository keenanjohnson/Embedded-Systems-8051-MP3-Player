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

enum seos_state_t STATE;

void seos_init( void )
{
    Timer2_ISR_Init();
}

void seos_run( uint32 First_clus )
{
    // Init
    seos_init();
    
    // ADD TIMEOUT
    while(1)
	{
		// Basic state machine
		switch( STATE )
		{
			case FIND_CLUS_1:
				// Find Cluster
				STATE = DATA_IDLE_2;
			break;
			case LOAD_BUF_1:
				// Load Buffer
				STATE = DATA_IDLE_2;
			break;
			case DATA_IDLE_1:
			break;
			case DATA_SEND_1:
			break;
			case FIND_CLUS_2:
			break;
			case LOAD_BUF_2:
			break;
			case DATA_IDLE_2:
			break;
			case DATA_SEND_2:
			break;
		}
	}
}

void Timer2_ISR (void) interrupt Timer_2_Overflow
{
    // Clear flag
    TF2=0; 

	YELLOWLED=ON;
}

void Timer2_ISR_Init( void ) 
{
  T2CON=0;
  TH2=Timer2_RELOAD_H;
  RCAP2H=Timer2_RELOAD_H;
  TL2=Timer2_RELOAD_L;
  RCAP2L=Timer2_RELOAD_L;

  ET2=1;
  TR2=1;
  EA=1;
}

