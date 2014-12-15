// main.c
//
// Keenan Johnson
// Franco Santa-Maria
//
// Submission for experiment 6
//
// Simple Embedded Operating System
//
// CpE 4160
// FALL 2014

#include "stdio.h"
#include "main.h"
#include "PORT.H"
#include "UART.h"
#include "print_bytes.h"
#include "SPI.h"
#include "SDcard.h"
#include "Serial_In.h"
#include "Directory_Functions.h"
#include "File_System.h"
#include "I2C.h"
#include "seos.h"

// Globals
xdata uint8 buf1[512];
xdata uint8 buf2[512];

extern uint32 idata FirstRootDirSec_g;

// Private Function Prototypes
void Play_Song(uint32 Start_Cluster);

main()
{
   uint32 idata Current_directory, Entry_clus;
   uint16 idata i, num_entries, entry_num;
   uint8 idata error_flag;

   /////////////////////
   // INIT
   /////////////////////
   AMBERLED=OFF;
   YELLOWLED=OFF;
   GREENLED=OFF;
   REDLED = ON;
   STA013_RESET=0;
   i=0;
   while(i<=60000) i++;
   REDLED = OFF;
   AUXR=0x0c;   // make all of XRAM available
   if(OSC_PER_INST==6)
   {
      CKCON0=0x01;  // set X2 clock mode
   }
   else if(OSC_PER_INST==12)
   {
      CKCON0=0x00;  // set standard clock mode
   } 
   uart_init();
   printf("I2C Test Program\n\r\n\n");
   error_flag=SPI_Master_Init(400000UL);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   printf("SD Card Initialization ... \n\r");
   error_flag=SD_card_init();
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   error_flag=SPI_Master_Init(20000000UL);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   for(i=0;i<512;i++)
   {
      buf1[i]=0xff;  // erase valout for debug
      buf2[i]=0xff;
   }
   error_flag=Mount_Drive(buf1);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }
   Current_directory=FirstRootDirSec_g;
   
    // Init MP3 Decoder
    STA013_init(); 
   
    //////////////////
    // Main Loop
	//////////////////
    while(1)
    {
        printf("Directory Sector = %lu\n\r",Current_directory);
        num_entries=Print_Directory(Current_directory, buf1);
	    printf("Enter Selection = ");
  	    entry_num=(uint16)long_serial_input();
	    if(entry_num<=num_entries)
	    {
	       Entry_clus=Read_Dir_Entry(Current_directory, entry_num, buf1);
		   if(Entry_clus&directory_bit)
		   {
		     Entry_clus&=0x0FFFFFFF;
             Current_directory=First_Sector(Entry_clus);
		   }
 	        else
		    {
                // File Selected
                //Open_File(Entry_clus, buf1);
                
                // Start OS
                seos_run();
		    }	  
	   }
	   else
	   {
	      printf("Invalid Selection\n\r");
	   }  
   }
} 

/*
//-------------- Private Functions --------------------------------------------------
void Play_Song(uint32 Start_Cluster)
{
   uint16 index1, index2;
   uint8 buffer1, buffer2;
   uint32 sector, sector_offset;
printf("Starting Cluster = %lu\n\r",Start_Cluster);

sector=First_Sector(Start_Cluster);
printf("Starting Sector = %lu\n\r",sector);
sector_offset=0;
buffer1=1;
buffer2=0;
//P3_2=ON;
YELLOWLED=ON;
nCS0=0;
SEND_COMMAND(17,sector+sector_offset);
read_block(512,buf1);
index1=0;
sector_offset++;
nCS0=1;
YELLOWLED=OFF;
AMBERLED=ON;
nCS0=0;
SEND_COMMAND(17,sector+sector_offset);
read_block(512,buf2);
index2=0;
sector_offset++;
nCS0=1;
AMBERLED=OFF;


do
{      
 do
 {
   if(DATA_REQ==0)
   {
       GREENLED=ON;
       BIT_EN=1;
       while((SPSTA&0x80)!=0x80);
       SPDAT=buf1[index1];
	   GREENLED=OFF;
	   index1++;
       if(index1>511)
       {
          if(index2>511)
          {
              while((SPSTA&0x80)!=0x80);
              BIT_EN=0;              
              AMBERLED=ON;
              nCS0=0;
              SEND_COMMAND(17,sector+sector_offset);
              read_block(512,buf2);
              index2=0;
              sector_offset++;
              nCS0=1;
              AMBERLED=OFF;
          }
          buffer1=0;
          buffer2=1;

       }
    }
    else
    {
       if(index2>511)
       {
           while((SPSTA&0x80)!=0x80);
           BIT_EN=0;
           AMBERLED=ON;
           nCS0=0;
           SEND_COMMAND(17,sector+sector_offset);
           read_block(512,buf2);
           index2=0;
           sector_offset++;
           nCS0=1;
           AMBERLED=OFF;
       }
       else
       {
           if(index1>511)
           {
               buffer1=0;
               buffer2=1;
           }
        }
     }
  }while(buffer1==1);
 do
 {
   if(DATA_REQ==0)
   {
       REDLED=ON;
       BIT_EN=1;
       while((SPSTA&0x80)!=0x80);
       SPDAT=buf2[index2];
       REDLED=OFF;
       index2++;
       if(index2>511)
       {
          if(index1>511)
          {
              while((SPSTA&0x80)!=0x80);
              BIT_EN=0; 
              YELLOWLED=ON;
              nCS0=0;
              SEND_COMMAND(17,sector+sector_offset);
              read_block(512,buf1);
              index1=0;
              sector_offset++;
              nCS0=1;
              YELLOWLED=OFF;
          }
          buffer2=0;
          buffer1=1;
         
       }
    }
    else
    {
       if(index1>511)
       {
           while((SPSTA&0x80)!=0x80);
           BIT_EN=0; 
           YELLOWLED=ON;
           nCS0=0;
           SEND_COMMAND(17,sector+sector_offset);
           read_block(512,buf1);
           index1=0;
           sector_offset++;
           nCS0=1;
           YELLOWLED=OFF;
       }
       else
       {
           if(index2>511)
           {
               buffer2=0;
               buffer1=1;
           }
        }
     }
  }while(buffer2==1);
}while(sector_offset<512);
//P3_2=OFF;
} 

/*
void Play_Song2(uint32 Start_Cluster)
{
   uint16 index1;
   uint8 buff_emp, * p_out;
   uint32 sector, sector_offset;
printf("Starting Cluster = %lu\n\r",Start_Cluster);
sector=First_Sector(Start_Cluster);
printf("Starting Sector = %lu\n\r",sector);
//P3_2=ON;
sector_offset=0;
YELLOWLED=ON;
nCS0=0;
SEND_COMMAND(17,sector+sector_offset);
read_block(512,buf1);
index1=0;
sector_offset++;
nCS0=1;
YELLOWLED=OFF;
//AMBERLED=ON;
//nCS0=0;
//SEND_COMMAND(17,sector+sector_offset);
//read_block(buf2,512);
//sector_offset++;
//nCS0=1;
//AMBERLED=OFF;


   p_out=buf1;
   buff_emp=0;
   do
   {
      if(DATA_REQ==0)
      {
         GREENLED=ON;
         BIT_EN=1;
         while((SPSTA&0x80)!=0x80);
         SPDAT=*(buf1+index1);
         index1++;
         if(index1==512)
         {
            buff_emp|=1;           
         }
		 if(index1==1024)
		 {
		    index1=0;
			buff_emp|=2;
	     }
         if(index1==768)
         {
           BIT_EN=0;
           GREENLED=OFF;
           if((buff_emp & 0x01)==0x01)
           {
              YELLOWLED=ON;
              nCS0=0;
              SEND_COMMAND(17,sector+sector_offset);
              read_block(512,buf1);
              nCS0=1;             
              YELLOWLED=OFF;
              buff_emp &= 0xFE;
              sector_offset++;            
           }
         }
         if(index1==256)
         {
           BIT_EN=0;
           GREENLED=OFF;
           if((buff_emp & 0x02)==0x02)
           {
              AMBERLED=ON;
              nCS0=0;
              SEND_COMMAND(17,sector+sector_offset);
              read_block(512,buf2);
              nCS0=1;             
              AMBERLED=OFF;
              buff_emp &= 0xFD;
              sector_offset++;            
           }
         }                        
       }
       else
       {
          GREENLED=OFF;
          BIT_EN=0;
          if((buff_emp & 0x01)==0x01)
          {
             YELLOWLED=ON;
             nCS0=0;
             SEND_COMMAND(17,sector+sector_offset);
             read_block(512,buf1);
             nCS0=1;             
             YELLOWLED=OFF;
             buff_emp &= 0xFE;
             sector_offset++;
 //            print_hex(1);
 //            print_hex(i);
            
          }
          else if((buff_emp & 0x02)==0x02)
          {
             AMBERLED=ON;
             nCS0=0;
             SEND_COMMAND(17,sector+sector_offset);
             read_block(512,buf2);
             nCS0=1;             
             AMBERLED=OFF;
             buff_emp &= 0xFD;
             sector_offset++;
          }
       }
   }while(sector_offset<128);   
   GREENLED=1;
  }  
*/
   