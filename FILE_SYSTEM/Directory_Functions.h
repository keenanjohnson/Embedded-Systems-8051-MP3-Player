#ifndef _Directory_Func_H
#define _Directory_Func_H

#include "Main.h"

//------- Public Constants  -----------------------------------------
#define FAT32 (4)
#define FAT16 (2)
#define FAT32_shift (2)
#define FAT16_shift (1)
#define Disk_Error (0xF0)
#define No_Disk_Error (0)
#define more_entries (0x8000)   
#define no_entry_found (0x80000000)  // msb set to indicate error
#define directory_bit  (0x10000000)  // lsb of first nibble (bit28)

extern uint8 SDTYPE;

// ------ Public function prototypes -------------------------------
uint16 Print_Directory(uint32 Sector_num, uint8 xdata * array_in);

uint32 Read_Dir_Entry(uint32 Sector_num, uint16 Entry, uint8 xdata * array_in);

//uint8 Open_File(uint32 Cluster, uint8 xdata * array_in);

#endif