#include "main.h"
#include "PORT.H"
#include "SPI.h"


/***********************************************************************
DESC:    Sets up the SPI to master mode with the clock as close
         to the input parameter as possible.
         clock=32-bit 
RETURNS: Error Flag
CAUTION: Sets the CPHA to 0 and CPOL to 0
         Disables SS and sets master mode 
************************************************************************/

uint8 SPI_Master_Init(uint32 clock_rate)
{
  uint8 divider,return_val;
  return_val=init_okay;
  divider=(uint8)(((OSC_FREQ/OSC_PER_INST)*6)/clock_rate);
  if(divider<=2)
  {
     SPCON=0x70;
  }
  else if(divider>2||divider<=4)
  {
     SPCON=0x71;
  }
  else if(divider>4||divider<=8)
  {
     SPCON=0x72;
  }
  else if(divider>8||divider<=16)
  {
     SPCON=0x73;
  } 
  else if(divider>16||divider<=32)
  {
     SPCON=0xF0;
  }
  else if(divider>32||divider<=64)
  {
     SPCON=0xF1;
  }
  else if(divider>64||divider<=128)
  {
     SPCON=0xF2;
  }
  else  // if the SPI clock rate is too slow, a divider cannot be found
  {
    return_val=illegal_clockrate;
  }
  return return_val;
}

/***********************************************************************
DESC:    Sends one byte using the SPI port and returns the received byte
          
RETURNS: SPI Error Flags | received byte
         or a timeout error
CAUTION: Waits for the SPI transfer to be complete
************************************************************************/


uint16 SPI_Transfer(uint8 data_input)
{
   uint8 test;
   uint16 data_output, timeout;
   timeout=0;
   SPDAT=data_input;
   do
   {
      test=SPSTA;
	  timeout++;
   }while(((test&0x80)!=0x80)&&(timeout!=0));
   if(timeout!=0)
   {
     data_output=(SPSTA&0x70)<<8;
     data_output|=SPDAT;
   }
   else
   {
     data_output=TIMEOUT_ERROR;
   }
   return data_output;
}
 

