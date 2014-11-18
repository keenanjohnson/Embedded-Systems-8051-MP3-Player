// fat.h
//
// Library for interacting with
// the FAT file system
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014
#ifndef FAT_H
#define FAT_H

#include "Main.h"
#include "offset_read.h"
#include "PORT.h"
#include "sdcard.h"

// Globals
extern uint16 BYTESPERSECTOR;
extern uint8 SECTORSPERCLUSTER;
extern uint32 ROOTDIRECTORYSECTORS;
extern uint8 SECTORSPERCLUSTER;
extern uint8 FATTYPE;
extern uint32 STARTOFFAT;
extern uint32 FIRSTDATASECOR;
extern uint32 FIRSTROOTDIRSEC;

// Initializes the file system
uint8 mount_drive(void);

#endif // _FAT_H
