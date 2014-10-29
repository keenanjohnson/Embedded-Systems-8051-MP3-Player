// print.h
//
// Contains memory print functions
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Sept. 2014

#ifndef _PRINT_H
#define _PRINT_H

#include <stdio.h>

#include "Main.h"

#define CR (0x0D)
#define LF (0x0A)

/////////////////////////////////////////////////
// Prints a line of 16 byte values
//
// Each hexadecimal value will be space delimited.
// Then each value will be printed again as an ASCII
// character. Non printable characters will be
// displayed as a "."
//
// @PARAMETERS
//
// first_byte - generic pointer to first memory address
/////////////////////////////////////////////////

void print_line( uint8 *first_byte );

/////////////////////////////////////////////////
// Prints a block of memory
//
// @PARAMETERS
//
// start_addr - Pointer to first memory address
// num_bytes - The number of bytes to display
/////////////////////////////////////////////////
void print_mem_block( uint8 *start_addr, uint16 num_bytes );

#endif
