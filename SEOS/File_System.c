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
   uint8 return_val;
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
   uint16 return_val,temp;
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
   uint32 return_val;
   uint8 temp, i;
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
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: Uint16 number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/

/*

uint16  Print_Directory(uint32 Sector_num, uint8 xdata * array_in)
{ 
   uint32 Sector, max_sectors;
   uint16 i, entries;
   uint8 temp8, j, attr, out_val, error_flag;
   uint8 * values;

   values=array_in;
   entries=0;
   i=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   AMBERLED=0;
   nCS0=0;
   error_flag=SEND_COMMAND(17,(Sector<<SDtype_g));
   if(error_flag==no_errors) error_flag=read_block(values,512);
   nCS0=1;
   AMBERLED=1;
   if(error_flag==no_errors)
   {
          YELLOWLED=0;
     do
     {
 
	    temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   	YELLOWLED=1;
		   if((attr&0x0E)==0)   // if hidden, system or Vol_ID bit is set do not print
		   {
		      entries++;
			  printf("%5d. ",entries);  // print entry number with a fixed width specifier
		      for(j=0;j<8;j++)
			  {
			     out_val=read8(i+j,values);   // print the 8 byte name
			     putchar(out_val);
			  }
              if((attr&0x10)==0x10)  // indicates directory
			  {
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        putchar(out_val);
			     }
			     printf("[DIR]\n");
			  }
			  else       // print a period and the three byte extension for a file
			  {
			     putchar(0x2E);       
			     for(j=8;j<11;j++)
			     {
			        out_val=read8(i+j,values);
			        putchar(out_val);
			     }
			     putchar(0x0d);
                 putchar(0x0a);
			  }
		    }

		}
		i=i+32;  // next entry

		if(i>510)
		{
		  Sector++;
          if((Sector-Sector_num)<max_sectors)
		  {
              nCS0=0;
              error_flag=SEND_COMMAND(17,(Sector<<SDtype_g));
              if(error_flag==no_errors) error_flag=read_block(values,512);
			  if(error_flag!=no_errors)
			    {
			      entries=0;   // no entries found indicates disk read error
				  temp8=0;     // forces a function exit
			    }
                nCS0=1;
			    i=0;
		  }
		  else
		  {
			  entries=entries|more_entries;  // set msb to indicate more entries in another cluster
			  temp8=0;                       // forces a function exit
		  }
		}
       
	  }while(temp8!=0);
	}
	else
	{
	   entries=0;    // no entries found indicates disk read error
	}
    return entries;
 }

 */

/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster  
INPUT: Starting Sector of the directory, an entry number and a pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint32 with cluster in lower 28 bits.  Bit 28 set if this is 
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION: 
************************************************************************/

/*

uint32 Read_Dir_Entry(uint32 Sector_num, uint16 Entry, uint8 xdata * array_in)
{ 
   uint32 Sector, max_sectors, return_clus;
   uint16 i, entries;
   uint8 temp8, attr, error_flag;
   uint8 * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   nCS0=0;
   error_flag=SEND_COMMAND(17,(Sector<<SDtype_g));
   if(error_flag==no_errors)  error_flag=read_block(values,512);
   nCS0=1;
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read8(0x0b+i,values);
		   if((attr&0x0E)==0)    // if hidden do not print
		   {
		      entries++;
              if(entries==Entry)
              {
			    if(FATtype_g==FAT32)
                {
                   return_clus=read8(21+i,values);
                   return_clus&=0x0F;            // makes sure upper four bits are clear
				   return_clus=return_clus<<8;
                   return_clus|=read8(20+i,values);
                   return_clus=return_clus<<8;
                }
                return_clus|=read8(27+i,values);
			    return_clus=return_clus<<8;
                return_clus|=read8(26+i,values);
			    attr=read8(0x0b+i,values);
			    if(attr&0x10) return_clus|=directory_bit;
                temp8=0;    // forces a function exit
              }
              
		   }
        }
		i=i+32;  // next entry
		if(i>510)
		{
		   Sector++;
		   if((Sector-Sector_num)<max_sectors)
		   {
              nCS0=0;
              error_flag=SEND_COMMAND(17,(Sector<<SDtype_g));
              if(error_flag==no_errors)  error_flag=read_block(values,512);
              nCS0=1;
			  if(error_flag!=no_errors)
			  {
			     return_clus=no_entry_found;
                 temp8=0; 
			  }
			  i=0;
		   }
		   else
		   {
			  temp8=0;                       // forces a function exit
		   }
		}
        
	 }while(temp8!=0);
   }
   else
   {
	 return_clus=no_entry_found;
   }
   if(return_clus==0) return_clus=no_entry_found;
   return return_clus;
}

*/


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




