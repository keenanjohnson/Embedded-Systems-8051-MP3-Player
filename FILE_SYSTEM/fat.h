// fat.h
//
// Library for interacting with
// the FAT file system
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Nov. 2014
#ifndef FAT_H
#define FAT_H

#include "Main.h"
#include "offset_read.h"
#include "PORT.h"
#include "sdcard.h"
//#include "Directory_Function.h"

// Globals
extern idata uint16 BYTESPERSECTOR;
extern idata uint8 SECTORSPERCLUSTER;
extern idata uint32 ROOTDIRECTORYSECTORS;
extern idata uint8 SECTORSPERCLUSTER;
extern idata uint8 FATTYPE;
extern idata uint32 STARTOFFAT;
extern idata uint32 FIRSTDATASECTOR;
extern idata uint32 FIRSTROOTDIRSEC;
extern idata uint16 ROOTENTRYCOUNT;

// Initializes the file system
uint8 mount_drive(void);

// Calculates the starting sector of a cluster
uint32 First_Sector(uint32 clusterNum);

// Finds the next cluster in a file given the previous
uint32 Find_Next_Clus(uint32 cluster_num, uint8 xdata * array);

// Prints a file sector by sector
uint8 Open_File(uint32 Cluster, uint8 xdata *array_in);

#endif // _FAT_H
