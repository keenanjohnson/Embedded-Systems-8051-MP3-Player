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

#include "PORT.H"
#include "Main.h"
#include "spi.h"

//Initialized SD card
uint8 initialize_card();

// Sends a command to the SD card
uint8 send_command( uint8 command, uint32 argument );

// Receives a response from the SD card after sending a command
uint8 receive_response( uint8 num_bytes, uint8 * response );

#endif //SDCARD_H
