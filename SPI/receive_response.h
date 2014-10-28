// receive_response.h
//
// function to receive a response from the SD card
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#ifndef RECEIVE_RESPONSE_H
#define RECEIVE_RESPONSE_H

#include "PORT.H"
#include "Main.h"

uint8 receive_response(uint8 num_bytes, uint8 * response);

#endif