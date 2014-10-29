// print.c
//
// Contains memory print functions
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Sept. 2014

#include "print.h"

void print_line( uint8 *first_byte )
{
    uint8 index;
    uint8 value;
    
    // Print address
    printf( "%p ", first_byte );
    
    // Print hexadecimal values
    for( index=0; index<16; index++ )
    {
        // Add offset to start addr
        value = *( first_byte + index );
        
        // Print value
        printf( "%2.2bX ", value );
    }
    
    // Print ASCII
    for( index=0; index<16; index++ )
    {
        // Add offset to start addr
        value = *(first_byte + index);
        
        // Check that char is printable
        if ( (value < 0x20) || (value > 0x7E) )
        {
            // Value is out of range
            // Replace with a "."
            value = 0x2E;
        }
        
        // Print character
        putchar( value );
    }
}

void print_mem_block( uint8 *start_addr, uint16 num_bytes )
{
    uint16 index;
    
    // Print heading
    printf("Addr.  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    
    // Add carriage return and linefeed
    putchar( CR );
    putchar( LF );

    // Adjust start addr to nearest 16 byte boundary
    index = (uint16) start_addr;
    index = index & 0x000F;
    start_addr = start_addr - index;
    index = 0;
    
    // Print lines
    while( index < num_bytes )
    {
        // Print
        print_line( start_addr + index );
        
        // Increment index
        index += 16;
        
        // Add carriage return and linefeed
        putchar( CR );
        putchar( LF );
    }
}
