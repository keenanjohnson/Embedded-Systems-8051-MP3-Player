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
#include "LCD_routines.h"

// Globals
xdata uint8 buf1[512];
xdata uint8 buf2[512];

uint16 idata Buffer1Position_g;
uint16 idata Buffer2Position_g;
uint8 idata ClusterEmpty_g;
uint32 idata Sector_g;
uint32 idata SectorOffset_g;
uint8 idata EndOfSong_g;
uint32 idata CurrentCluster_g;
extern uint32 idata FirstRootDirSec_g;

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

   // Init LCD
   LCD_Init();
   LCD_Print(10, "Sys. Init.");

   // UART Init
   uart_init();

   // I2C Test
   printf("I2C Test Program\n\r\n\n");
   error_flag=SPI_Master_Init(400000UL);
   if(error_flag!=no_errors)
   {
      REDLED=ON;
      while(1);
   }

   // SD Card Init
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
             Current_directory=First_Sector( Entry_clus );
		   }
 	        else
		    {
                // File Selected                
                // Start OS
                seos_run( Entry_clus );
		    }	  
	   }
	   else
	   {
	      printf("Invalid Selection\n\r");
	   }  
   }
} 
