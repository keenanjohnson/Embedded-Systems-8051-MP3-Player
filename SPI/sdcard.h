// sdcard.h
//
// Handles interfacing with an SD card
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#ifndef SDCARD_H
#define SDCARD_H

#include <stdio.h>

#include "PORT.H"
#include "print.h"
#include "spi.h"

//Initialized SD card
uint8 initialize_card();

// Sends a command to the SD card
uint8 send_command( uint8 command, uint32 argument );

// Receives a response from the SD card after sending a command
//
// Return value of 0 is no error
// Return value of 1 is timeout error
// Return value of 2 is spi error
uint8 receive_response( uint8 number_of_bytes, uint8 *response_array );

// Reads a data block of specified size
//
// CS should be set external to this function
uint8 read_sd_block( uint16 number_of_bytes, uint8 *array );

#endif //SDCARD_H

