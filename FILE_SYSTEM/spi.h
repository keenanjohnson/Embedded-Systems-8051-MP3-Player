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

#define TIMEOUT_ERR (0x8000)

// Sets up SPI hardware as master
// 
// Returns 0 for no error
// Returns 1 for error condition
uint8 spi_master_init( uint32 clock_rate );

// Writes a byte specified by send_value
// Out of the SPI bus
// Chip select must be toggled outside of
// This function
//
// High Byte is error status
// Low Byte is value returned
uint16 spi_transfer( uint8 send_value );

#endif // SPI_H
