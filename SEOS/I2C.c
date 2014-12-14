#include "main.h"
#include "PORT.H"
#include "I2C.h"
#include <stdio.h>



/***********************************************************************
DESC:    Creates the signals required for transmitting bytes using
         the I2C bus.
RETURNS: Error Flag
CAUTION: 
          
************************************************************************/

uint8 I2C_WRITE(uint8 device_addr,uint8 num_bytes,uint8 idata * send_array)
{
  uint8 send_val, return_val, num_bits, send_bit, sent_bit;

  SDA=1;
  SCL=1;
  return_val=no_errors;
  if((SCL==1) && (SDA==1))
  {
     I2C_clock_start();
     send_val=device_addr<<1;
	 send_val&=0xFE;           // make sure lsb is 0 for write
	 //  send START condition
	 SDA=0;
	 num_bytes++;
	 do
	 {
	    num_bits=8;
        do
		{
		   I2C_clock_delay(Continue);
		   SCL=0;
		   num_bits--;
           send_bit=((send_val>>num_bits)&0x01);
		   SDA=(bit)send_bit;
		   I2C_clock_delay(Continue);
           SCL=1;
		   while(SCL!=1);
           sent_bit=SDA;
		   if(sent_bit!=send_bit) return_val=bus_busy_error;
         }while((num_bits>0)&&(return_val==no_errors));
		 I2C_clock_delay(Continue);
		 SCL=0;
         SDA=1;      // set SDA high so slave can send ACK
         num_bytes--;
		 send_val=*(send_array);
		 send_array++;
		 I2C_clock_delay(Continue);
		 SCL=1;
		 while(SCL!=1);
		 sent_bit=SDA;
		 if(sent_bit!=0) return_val=NACK_error;
      }while((num_bytes>0)&&(return_val==no_errors));
      // send STOP condition
	  I2C_clock_delay(Continue);
      SCL=0;
	  SDA=0;
	  I2C_clock_delay(Continue);
	  SCL=1;
	  while(SCL!=1);
	  I2C_clock_delay(Stop);
	  SDA=1;
  }
  else
  { 
     return_val=bus_busy_error;
  }
  return return_val;
}

/***********************************************************************
DESC:    Creates the signals required for receiving bytes using
         the I2C bus.
RETURNS: Error Flag
CAUTION: 
          
************************************************************************/


uint8 I2C_READ(uint8 device_addr,uint8 num_bytes,uint8 idata * rec_array)
{
  uint8 send_val, return_val, num_bits, send_bit, sent_bit,rcv_val;

  SDA=1;
  SCL=1;
  return_val=no_errors;
  if((SCL==1) && (SDA==1))
  {
     I2C_clock_start();
     send_val=device_addr<<1;
	 send_val|=0x01;           // make sure lsb is 1 for read
	 //  send START condition
	 SDA=0;
	 //  send device address+R
	 num_bits=8;
     do
	 {
		I2C_clock_delay(Continue);
	    SCL=0;
		num_bits--;
        send_bit=((send_val>>num_bits)&0x01);
		SDA=(bit)send_bit;
		I2C_clock_delay(Continue);
        SCL=1;
		while(SCL!=1);
        sent_bit=SDA;
		if(sent_bit!=send_bit) return_val=bus_busy_error;
      }while((num_bits>0)&&(return_val==no_errors));
	  I2C_clock_delay(Continue);
	  SCL=0;
      SDA=1;      // set SDA high so slave can send ACK
	  I2C_clock_delay(Continue);
	  SCL=1;
	  while(SCL!=1);
	  sent_bit=SDA;
	  if(sent_bit!=0) return_val=NACK_error;
      while((num_bytes>0)&&(return_val==no_errors))
	  {
	     num_bits=8;
	     do
		 {
		   I2C_clock_delay(Continue);
		   SCL=0;
		   num_bits--;
		   SDA=1;
		   rcv_val=rcv_val<<1;  // shift value to prepare for next rcvd bit
		   I2C_clock_delay(Continue);
		   SCL=1;
		   while(SCL!=1);
           sent_bit=SDA;
		   rcv_val|=sent_bit;
		 }while(num_bits>0);
		 *rec_array=rcv_val;
		 rec_array++;
		 if(num_bytes==1)
		 {
		    send_bit=1;   // NACK for last byte
		 }
		 else
		 {
		    send_bit=0;   // ACK if more bytes are to be received
		 }
		 I2C_clock_delay(Continue);
		 SCL=0;
		 SDA=send_bit;
		 I2C_clock_delay(Continue);
		 SCL=1;
		 while(SCL!=1);
		 num_bytes--;
	  }
      // send STOP condition
	  I2C_clock_delay(Continue);
      SCL=0;
	  SDA=0;
	  I2C_clock_delay(Continue);
	  SCL=1;
	  while(SCL!=1);
	  I2C_clock_delay(Stop);
	  SDA=1;
  }
  else
  { 
     return_val=bus_busy_error;
  }
  return return_val;
}

/***********************************************************************
DESC:    Initializes and starts Timer0 for a hardware delay for 
         creating a consistant I2C frequency
RETURNS: nothing
CAUTION: 
          
************************************************************************/

void I2C_clock_start(void)
{
// Configure Timer0 as a 16-bit timer (mode1)
   TMOD &=0xF0;
   TMOD |=0x01;

   ET0=0;  //interrupts not used

   TH0=I2C_RELOAD_H;
   TL0=I2C_RELOAD_L;

   TF0=0;   // Clear overflow
   TR0=1;   // Start Timer Running
}


/***********************************************************************
DESC:    Waits for the overflow to be set and restarts Timer0 for a  
         hardware delay for creating a consistant I2C frequency
		 Timer is stopped if stop is sent.
RETURNS: nothing
CAUTION: 
          
************************************************************************/

void I2C_clock_delay(uint8 control)
{
   if(TR0==1)
   { 
     while(TF0==0);
   }

   TR0=0;
   if(control==Continue)
   {
      TH0=I2C_RELOAD_H;
      TL0=I2C_RELOAD_L;

      TF0=0;   // Clear overflow
      TR0=1;   // Start Timer Running
   }
}

/***********************************************************************
DESC:    Initializes the STA013 with the following steps:
         Applies a hardware reset signal, checks the ID register for an 0xAC
		 response and then sends the configuration data.
RETURNS: error flag
CAUTION: 
          
************************************************************************/


uint8 STA013_init(void)
{
   uint8 error_flag, timeout;
   uint8 idata send[5], rcv[5];
   uint8 * p_out;
   uint16 j;

   uint8 code STA_cfg_addr[18]={0x54, 0x55,  6, 11,  82,  81, 101, 100, 80, 97, 0x05, 0x0d, 0x18, 0x0c, 0x46, 0x48, 0x7d, 0x72};
   uint8 code STA_cfg_data[18]={0x07, 0x10,  9,  2, 184,   0,   0,   0,  6,  5, 0xa1, 0x00, 0x04, 0x05, 0x07, 0x07, 0x07, 0x01};

   extern uint8 code CONFIG;
   extern uint8 code CONFIG2;

   STA013_RESET=0;

   send[0]=0x01;   // address of the ID reg.  
   timeout=50;

   STA013_RESET=1;    // make STA013 active
   //P3_4=0;
   do
   {
      error_flag=I2C_WRITE(STA013,1,send);
      timeout--;
      if(timeout==0) AMBERLED=0;
   }while((error_flag!=no_errors)&&(timeout!=0));
   if(timeout!=0)
   {
      timeout=50;
	  do
	  {
        error_flag=I2C_READ(STA013,1,rcv);
        timeout--;
	  }while((error_flag!=no_errors)&&(timeout!=0));
	  if(timeout!=0)
	  {
         printf("Output from ID register: %2.2bX \n\r",rcv[0]);
	  }
   }
   //P3_4=1;



   if(rcv[0]==0xAC)
   {
     send[0]=0x71;
     error_flag=I2C_WRITE(STA013,1,send);
     error_flag=I2C_READ(STA013,1,rcv);
     printf("STA013 SoftVersion: %2.2bX \n\r",rcv[0]); 
     j=0;
     p_out=&CONFIG;  // point to STA013 configuration file
     do
     {
        send[0]=*(p_out+j);
        j++;
        send[1]=*(p_out+j);
        j++;
        if(send[0]!=0xff)
        {
          
           timeout=50;
           do
           {
               error_flag=I2C_WRITE(STA013,2,send);
               timeout--;
               if(timeout==0) AMBERLED=0;
           }while((error_flag!=no_errors)&&(timeout!=0));
        }
     }while((send[0]!=0xff) && (timeout!=0));
     printf("First configuration file sent...\n\r");
     printf("Number of values written: %d \n\r",j>>1);
     j=0;
     p_out=&CONFIG2;  // point to STA013 configuration file
     do
     {
        send[0]=*(p_out+j);
        j++;
        send[1]=*(p_out+j);
        j++;
        if(send[0]!=0xff)
        {
           timeout=50;
           do
           {
               error_flag=I2C_WRITE(STA013,2,send);
               timeout--;
               if(timeout==0) AMBERLED=0;
           }while((error_flag!=no_errors)&&(timeout!=0));
        }
     }while((send[0]!=0xff) && (timeout!=0));
     send[0]=0x71;
     error_flag=I2C_WRITE(STA013,1,send);
     error_flag=I2C_READ(STA013,1,rcv);
     printf("STA013 SoftVersion: %2.2bX \n\r",rcv[0]);            
        
     for(j=0;j<18;j++)
	  {
	    send[0]=STA_cfg_addr[j];
	    send[1]=STA_cfg_data[j];
        timeout=50;
        do
        {
            error_flag=I2C_WRITE(STA013,2,send);
            timeout--;
            if(timeout==0) AMBERLED=0;
        }while((error_flag!=no_errors)&&(timeout!=0));
		printf("Sent to STA013: %2.2bX  %2.2bX \n\r",send[0],send[1]);
        error_flag=I2C_WRITE(STA013,1,send);
        error_flag=I2C_READ(STA013,1,rcv);
        printf("Output from register: %2.2bX  %2.2bX %2.2bX \n\r",send[0],rcv[0],error_flag);
	  }
      printf("Configuration Complete\n\r");
   }      
   return error_flag;
}

