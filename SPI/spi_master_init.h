// spi_master_init.h
//
// SPI initialization function
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#ifndef SPI_INIT_H
#define SPI_INIT_H

#include "PORT.H"
#include "Main.h"

uint8 spi_master_init(uint32 clock_rate);

#endif