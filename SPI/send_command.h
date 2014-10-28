// send_command.h
//
// function to send a command to the SD card
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#ifndef SEND_COMMAND_H
#define SEND_COMMAND_H

#include "PORT.H"
#include "Main.h"

uint8 send_command(uint8 command, uint32 argument);

#endif