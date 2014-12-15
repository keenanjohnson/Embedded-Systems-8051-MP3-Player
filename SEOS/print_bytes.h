#ifndef _print_bytes_H
#define _print_bytes_H

#include "main.h"
#include "UART.h"



// ------ Public function prototypes -------------------------------

void print_hex(uint8 in);

void print_16bytes(uint8 * array_in);

//void print_memory(uint16 number_of_bytes, uint8 * array_in);


#endif