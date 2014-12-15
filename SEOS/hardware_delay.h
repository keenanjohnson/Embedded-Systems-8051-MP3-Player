#ifndef _Hardware_Delay_H
#define _Hardware_Delay_H

#include "Main.H"

#define one_msec (1)

#define timer0_reload (65536-((OSC_FREQ*one_msec)/(1000*OSC_PER_INST)))  
#define ReloadTH0 (timer0_reload>>8)
#define ReloadTL0 (timer0_reload&0xff)


#define timer0_reload_40us (65536-((OSC_FREQ)/(25000*OSC_PER_INST)))  
#define Reload_40us_TH0 (timer0_reload>>8)
#define Reload_40us_TL0 (timer0_reload&0xff)
// ------ Public function prototypes -------------------------------

void DELAY_MS_T0(uint16 duration);

void DELAY_40us_T0(void);

#endif