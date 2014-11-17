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

#include <stdio.h>

#include "Main.h"

// Each of these functions reads a specfic datatype
// from a given 512 block of XRAM data.
// 
// offset - this is the offset of the data to read
// in the XRAM block. This value must be less than 
// 512. If this value is greater than 511, the function
// will return 0 and print an error message to the 
// serial port
//
// array_name - this is the block of data from which
// to read

// Reads a UINT8
uint8 read8(uint16 offset, uint8 *array_name);

// Reads a UINT16
uint16 read16(uint16 offset, uint8 *array_name);

// Reads a UINT32
uint32 read32(uint16 offset, uint8 *array_name);

#endif //OFFSET_READ_H

