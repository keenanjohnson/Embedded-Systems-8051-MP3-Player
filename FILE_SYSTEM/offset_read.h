// offset_read.h
//
// Contains functions to read values 
// from a 512 byte block of data
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Nov. 2014

#ifndef _OFFSET_READ_H
#define _OFFSET_READ_H

#include "Main.h"

uint8 read8(uint16 offset, uint8 *array_name);

uint16 read16(uint16 offset, uint8 *array_name);

uint32 read32(uint16 offset, uint8 *array_name);

#endif //OFFSET_READ_H

