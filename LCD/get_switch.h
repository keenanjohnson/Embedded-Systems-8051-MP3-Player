// get_switch.h
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#ifndef GET_SWITCH_H
#define GET_SWITCH_H

#include "main.h"
#include "PORT.H"
#include "delay.h"

uint8 get_switch(sbit name, uint32 debounce_delay);

#endif