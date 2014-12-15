#include "AT89C51RC2.h"
#include <stdio.h>
#include "main.h"
#include "print_bytes.h"




/***********************************************************************
DESC:    Prints 8 bytes in Hexadecimal and then ASCII
INPUT: Pointer to an array of eight bytes
RETURNS: nothing
CAUTION: 
************************************************************************/
void print_16bytes(uint8 * array_in)
{
   uint8 * input;
   //uint16 i;
   uint8 idata index, dat;
   input = array_in;
   //i= (uint16) input;
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


/***********************************************************************
DESC: Prints an array from memory in Hexadecimal and then ASCII
INPUT: Pointer to an array, number of bytes to print
RETURNS: nothing
CAUTION: 
************************************************************************/
void print_memory(uint16 number_of_bytes, uint8 * array_in)
{
   uint8 * input;
   uint16 idata i;
   input = array_in;
   i = (uint16) input;
   i &= 0x000f;
   input = input - i;
   printf("Addr.   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n\r");
   do
   {
      print_16bytes(input);
      input+=16;
   }while(input<(array_in+number_of_bytes));
}
		