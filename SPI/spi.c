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
	// Variables
    uint8 divider;
    uint8 error_status = 0;
    
	// Calculate divisor
    divider = (uint8)(((OSC_FREQ / OSC_PER_INST) * 6) / clock_rate);
    
	// Set SPI Control Register
    if(divider > 128)
    {
        // Clock rate is out of range
        error_status = 1;
    }
    else if(divider <= 128 && divider > 64)
    {
		// Divider is 128
        SPCON = 0xF2;
    }
    else if(divider <=64 && divider > 32)
    {
		// Divider is 64
        SPCON = 0xF1;
    }
    else if(divider <=32 && divider > 16)
    {
		// Divider is 32
        SPCON = 0xF0;
    }
    else if(divider <= 16 && divider > 8)
    {
		// Divider is 16
        SPCON = 0x73;
    }
    else if(divider <= 8 && divider > 4)
    {
		// Divider is 8
        SPCON = 0x72;
    }
    else if(divider <= 4 && divider > 2)
    {
		// Divider is 4
        SPCON = 0x71;
    }
    else
    {
		// Divisor is 2
        SPCON = 0x70;
    }
    
	// Set CS to high
	SPI_nCS0 = 1;

    // Return error status
    return error_status;
}

uint16 spi_transfer( uint8 send_value )
{
    // Variable Init
    uint8 spi_status;
    uint16 data_output;
	uint16 timeout;
    timeout = 0;
    
    // Write Byte to be sent
    SPDAT = send_value;
    
    // Check for completed transfer
    do
    {
        spi_status = SPSTA;
        timeout++;
    } while( (spi_status & 0x80 ) != 0x80 && timeout != 0);
    
    if(timeout != 0)
    {
		// Check for error condition
        data_output = (SPSTA & 0x70) << 8;
        data_output |= SPDAT;
    }
    else // Timed out
    {
        data_output = 1;	
    }
	
    // Return 
    return data_output;
}
