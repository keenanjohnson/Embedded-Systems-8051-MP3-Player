// main.c
//
// main file for experiment #1
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Sept. 2014

#include "Main.h"
#include "memory_test.h"
#include "PORT.h"
#include "print.h"
#include "uart.h"

void main(void)
{
    // Declare generic pointer
    uint8 *pointer;
	
    // Prepare UART
    uart_init();
    
	// Infinite Display Loops
	while(1)
	{
	    // Print Code Memory
	    pointer = code_memory_init();
	    print_mem_block( pointer, 64 );
	    
	    // Print XDATA Memory
	    pointer = xdata_memory_init();
	    print_mem_block( pointer, 64 );
	}
}
