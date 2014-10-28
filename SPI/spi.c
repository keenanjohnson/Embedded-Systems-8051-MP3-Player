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
    uint8 divider;
    uint8 error_status = 0;
    
    divider = (uint8)(((OSC_FREQ / OSC_PER_INST) * 6) / clock_rate);
    
    SPCON |= 0x70;
    SPCON &= 0xF3;
    
    if(divider > 128)
    {
        // Clock rate is out of range
        error_status = 1;
    }
    else if(divider <= 128 && divider > 64)
    {
        SPCON |= 0x82;
        SPCON &= 0xFE;
    }
    else if(divider <=64 && divider > 32)
    {
        SPCON |= 0x81;
        SPCON &= 0xFD;
    }
    else if(divider <=32 && divider > 16)
    {
        SPCON |= 0x80;
        SPCON &= 0xFC;
    }
    else if(divider <= 16 && divider > 8)
    {
        SPCON |= 0x03;
        SPCON &= 0x7F;
    }
    else if(divider <= 8 && divider > 4)
    {
        SPCON |= 0x02;
        SPCON &= 0x7E;
    }
    else if(divider <= 4 && divider > 2)
    {
        SPCON |= 0x01;
        SPCON &= 0x7D;
    }
    else
    {
        SPCON &= 0x7C;
    }
    
    // Return value
    return error_status;
}

uint16 spi_transter( uint8 send_value )
{
    // Variable Init
    uint8 test;
    uint16 data_output, timeout;
    timeout = 0;
    
    // Write Byte to be sent
    SPDAT = send_value;
    
    // Check for completed transfer
    do
    {
        test = SPSTA;
        timeout++;
    } while((test & 0x80) != 0x80 && timeout != 0);
    
    if(timeout != 0)
    {
        data_output = (SPSTA & 0x70) << 8;
        data_output |= SPDAT;
    }
    else // Timed out
    {
        data_output = 1;	
    }
    
    // Return r
    return data_output;
}

