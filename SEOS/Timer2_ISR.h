#ifndef _Timer2_ISR_H
#define _Timer2_ISR_H

#include "Main.h"
#include "PORT.h"
#include "seos.h"

#define interval_ms (11)
#define Timer2_RELOAD (65536-((OSC_FREQ*interval_ms)/(OSC_PER_INST*1000UL)))
#define Timer2_RELOAD_H (Timer2_RELOAD/256)
#define Timer2_RELOAD_L (Timer2_RELOAD%256)
 
// ------ Public function prototypes -------------------------------
void Timer2_ISR_Init(void);

#endif