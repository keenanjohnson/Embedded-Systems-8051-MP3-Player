#include "Timer2_ISR.h"

void Timer2_ISR (void) interrupt Timer_2_Overflow
{
    // Clear flag
    TF2=0; 

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



