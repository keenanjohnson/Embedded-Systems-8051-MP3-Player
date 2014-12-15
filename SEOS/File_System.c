#include "AT89C51RC2.h"
#include <stdio.h>
#include "main.h"
#include "PORT.H"
#include "UART.h"
#include "SPI.h"
#include "SDCard.h"
#include "File_System.h"
#include "print_bytes.h"
#include "Directory_Functions.h"

extern uint32 idata FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
extern uint16 idata BytesPerSec_g;
extern uint8 idata SDtype_g, SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;


// Read Value Functions

/***********************************************************************
DESC: Reads a specified byte from a block of memory 
INPUT: Offset to the byte, Starting address of the block of memory
RETURNS: Uint8 specifed byte
CAUTION: 
************************************************************************/
uint8 read8(uint16 offset, uint8 * array_name)
{
   uint8 idata return_val;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=*(array_name+offset);
   return return_val;
}

/***********************************************************************
DESC: Reads a specified integer (stored little endian) from a block of memory 
INPUT: Offset to the LSB of the integer, Starting address of the block of memory
RETURNS: Uint16 specfied integer
CAUTION: 
************************************************************************/

uint16 read16(uint16 offset, uint8 * array_name)
{
   uint16 idata return_val,temp;
   offset&=0x1FF;  // limit offset to 0 to 511
   return_val=0;
   return_val=*(array_name+offset+1);
   return_val=return_val<<8;
   temp=*(array_name+offset);
   return_val|=temp;
   return return_val;
}

/***********************************************************************
DESC: Reads a specified word (stored little endian) from a block of memory 
INPUT: Offset to the LSB of the word, Starting address of the block of memory
RETURNS: Uint32 specfied word
CAUTION: 
************************************************************************/

uint32 read32(uint16 offset, uint8 * array_name)
{
   uint32 idata return_val;
   uint8 idata temp, i;
   return_val=0;
   offset&=0x1FF;  // limit offset to 0 to 511
   for(i=0;i<4;i++)
   {   
       temp=*(array_name+offset+(3-i));
       return_val=return_val<<8;
       return_val|=temp;
   }
   return return_val;
}

/***********************************************************************
DESC: Determines FAT type, Start of the FAT, Root Directory Sector
      and Start of Data for an SD card
INPUT: Start of a block of memory in xdata that can be used 
       to read blocks from the SD card
RETURNS: Uint8 error code    Creates globals in idata for: 
idata Uint32 FATSz, RelSec, RootClus, FirstDataSec_g, StartofFAT_g;
idata Uint16 BytesPerSec_g, BytesPerSecM, RootEntCnt;
idata Uint8 SecPerClus, FATtype, SecShift;

CAUTION: Does not work with FAT16 yet!
************************************************************************/

uint8 Mount_Drive(uint8 xdata * array_in)
{
  uint32 TotSec, DataSec, CntClus,MBR_RelSec, FATSz, RootClus;
  uint16 RsvdSecCnt, RootEntCnt, temp16;
  uint8 NumFAT, temp8;
  uint8 * values;

   

   values=array_in;
   SDtype_g=Return_SD_Card_Type();
   printf("Locating Boot Sector...\n\r");
   nCS0=0;
   SEND_COMMAND(17,0);
   read_block(512,values);
   nCS0=1;
   temp8=read8(0,values);
   if((temp8!=0xEB)&&(temp8!=0xE9))
   {
      MBR_RelSec=read32(MBR_RelSectors,values);
	  nCS0=0;
	  SEND_COMMAND(17,(MBR_RelSec<<SDtype_g));
	  read_block(512,values);
	  nCS0=1;
	  printf("Relative Sectors = %ld\n\r",MBR_RelSec);
	  temp8=read8(0,values);
   }
   if((temp8!=0xEB)&&(temp8!=0xE9))
   {
      printf("Disk Error!\n\r");
	  return Disk_Error;
   }
   BytesPerSec_g=read16(BPB_BytesPerSector,values);
   BytesPerSecShift_g=0;
   temp16=BytesPerSec_g;
   while(temp16!=0x01)
   {
      BytesPerSecShift_g++;
      temp16=temp16>>1;
   }
   SecPerClus_g=read8(BPB_SectorsPerCluster,values);
   printf("Sectors Per Cluster = %Bu\n\r",SecPerClus_g);
   RsvdSecCnt=read16(BPB_RsvdSectorCount,values);
   NumFAT=read8(BPB_NumberFATs,values);
   RootEntCnt=read16(BPB_RootEntryCount,values);
   TotSec=read16(BPB_TotalSectors16,values);
   if(TotSec==0) TotSec=read32(BPB_TotalSectors32,values);
   FATSz=read16(BPB_FATsize16,values);
   if(FATSz==0)
   {
	  FATSz=read32(BPB_FATsize32,values);
      RootClus=read32(BPB_RootCluster,values);
   }
   RootDirSecs_g=((RootEntCnt * 32) + (BytesPerSec_g - 1))/BytesPerSec_g;
   DataSec=TotSec-(RsvdSecCnt+(NumFAT*FATSz)+RootDirSecs_g);
   CntClus=DataSec/SecPerClus_g;
   StartofFAT_g=RsvdSecCnt+MBR_RelSec;
   FirstDataSec_g=StartofFAT_g+(NumFAT*FATSz)+RootDirSecs_g;
   if(CntClus<65525)
   {
     FATtype_g=FAT16;
	 FATshift_g=FAT16_shift;
	 printf("FAT16 Detected...\n\r");
	 FirstRootDirSec_g=StartofFAT_g+(NumFAT*FATSz);

   }
   else
   {
     FATtype_g=FAT32;
	 FATshift_g=FAT32_shift;
	 printf("FAT32 Detected...\n\r");
     FirstRootDirSec_g=((RootClus-2)*SecPerClus_g)+FirstDataSec_g;
   }
   return No_Disk_Error;
}


/***********************************************************************
DESC: Calculates the First Sector of a given Cluster 
INPUT: uint32 Cluster number
RETURNS: uint32 sector number 
CAUTION: 
************************************************************************/
uint32 First_Sector (uint32 Cluster_num)
{
   uint32 Sector_num;
   if(Cluster_num==0) 
   {
       Sector_num=FirstRootDirSec_g;
   }
   else
   {
       Sector_num=((Cluster_num-2)*SecPerClus_g)+FirstDataSec_g;
   }
   return Sector_num;
}

/***********************************************************************
DESC: Finds the next cluster number of a file in the FAT with an input
      parameter of the current cluster number.
INPUT: Current cluster number.
RETURNS: Next Cluster number from the FAT
CAUTION: 
************************************************************************/


uint32 Find_Next_Clus(uint32 Cluster_num, uint8 xdata * array_name)
{
   uint32 Sector, return_clus;
   uint16 FAToffset;
   uint8 * values;
   values=array_name;
   Sector=(Cluster_num>>(BytesPerSecShift_g-FATshift_g))+StartofFAT_g;
   nCS0=0;
   SEND_COMMAND(17,(Sector<<SDtype_g));
   read_block(512,values);
   nCS0=1;
   FAToffset=(uint16)((Cluster_num<<FATshift_g)&(BytesPerSec_g-1));
   if(FATtype_g==FAT32)    // FAT32
   {
       return_clus=(read32(FAToffset,values)&0x0FFFFFFF);
   }
   else if(FATtype_g==FAT16)    // FAT16
   {
       return_clus=(uint32)(read16(FAToffset,values));
   }
   return return_clus;
}
   



/***********************************************************************
DESC: Prints the sectors of a file until the user hits X
      Prints the Cluster Number and Sector number of each sector as well
INPUT: Starting Cluster of the file and the start of a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: Uint8 0
CAUTION: 
************************************************************************/


uint8 Open_File(uint32 Cluster_num, uint8 xdata * array_in)
{
   uint32 Sector, SecOffset;
   uint16 entries;
   uint8 temp8;
   uint8 * values;

   values=array_in;
   entries=0;
   SecOffset=0; 
   
   do
   {
      if(SecOffset==0) Sector=((Cluster_num-2)*SecPerClus_g)+FirstDataSec_g;
      printf("Cluster#: %9lu,  Sector#: %9lu,  SecOffset: %lu\n",Cluster_num,(Sector+SecOffset),SecOffset);
      nCS0=0;
      SEND_COMMAND(17,((Sector+SecOffset)<<SDtype_g));
      read_block(512,values);
      nCS0=1;
      print_memory(512,values);
      SecOffset++;
      if(SecOffset==SecPerClus_g)
      {
         Cluster_num=Find_Next_Clus(Cluster_num,values);
         SecOffset=0;
      }
      printf("Press Space to Continue or X to exit\n");
      do
      {
        temp8=getchar();
      }while((temp8!=0x20)&&(temp8!=0x58));
   }while(temp8==0x20);
   putchar(0x0d);
   putchar(0x0a);
return 0;
}




