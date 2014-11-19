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
   uint8 * input;
   uint8 index, dat;
   input = first_byte;
   printf("%p ",input);
   for (index=0;index<16;index++)
   {
      dat=*(input+index);
      printf("%2.2bX ",dat);
   }
   for (index=0;index<16;index++)
   { 
      dat=*(input+index);
      if (dat<32 || dat>127) dat=46;
      putchar(dat);
   }
   putchar(CR);
   putchar(LF);
}

void print_mem_block( uint8 *start_addr, uint16 num_bytes )
{
   idata uint8 * input;
   idata uint16 i;
   input = start_addr;
   i = (uint16) input;
   i &= 0x000f;
   input = input - i;
   printf("Addr.   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n\r");
   do
   {
      print_line(input);
      input+=16;
   }while(input<(start_addr+num_bytes));
}

void print_newline()
{
	// Add carriage return and linefeed
	putchar( CR );
    putchar( LF );
}

