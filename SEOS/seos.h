// seos.h
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

#include "Main.h"
#include "PORT.H"
#include "SDCard.h"
#include "File_System.h"
#include "SPI.h"
#include "LCD_Routines.h"

#define DR_ACTIVE (0)
#define DR_INACTIVE (1)

#define interval_ms (11)
#define Timer2_RELOAD (65536-((OSC_FREQ*interval_ms)/(OSC_PER_INST*1000UL)))
#define Timer2_RELOAD_H (Timer2_RELOAD/256)
#define Timer2_RELOAD_L (Timer2_RELOAD%256)

enum seos_state_t{FIND_CLUS_1, LOAD_BUF_1, DATA_IDLE_1, DATA_SEND_1, FIND_CLUS_2, LOAD_BUF_2, DATA_IDLE_2, DATA_SEND_2};

void Timer2_ISR_Init(void);

void seos_init( uint32 First_clus );

void seos_run( uint32 First_clus );

uint32 Find_Cluster_And_Check_EOF( uint32 current_cluster, uint8 xdata *buffer );

void load_buffer( uint8 xdata *buffer );

void send_buffer( uint8 xdata *buffer );

void update_MP3_Display( void );

