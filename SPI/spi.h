// spi_transfer.h
//
// SPI transfer function
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#ifndef SPI_H
#define SPI_H

#include "Main.h"
#include "PORT.H"

// Sets up SPI hardware as master
uint8 spi_master_init( uint32 clock_rate );

// Writes a byte specified by send_value
// Out of the SPI bus
uint16 spi_transfer( uint8 send_value );

#endif // SPI_H
