// spi.c
//
// Handles SPI Communication
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#include "spi.h"

uint8 spi_master_init( uint32 clock_rate )
{
    
}

uint16 spi_transter( uint8 send_value )
{
    // Set SPI pins to input
    P1 |= 0xF0;

    // Set SCON register
    // Inits control reg
    SPCON = 0x70;
}

