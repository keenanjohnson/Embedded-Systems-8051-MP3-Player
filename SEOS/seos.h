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
#include "hardware_delay.h"

#define DR_ACTIVE (0)
#define DR_INACTIVE (1)

#define interval_ms (11)
#define Timer2_RELOAD (65536-((OSC_FREQ*interval_ms)/(OSC_PER_INST*1000UL)))
#define Timer2_RELOAD_H (Timer2_RELOAD/256)
#define Timer2_RELOAD_L (Timer2_RELOAD%256)

// ENUM for state machine
enum seos_state_t{FIND_CLUS_1, LOAD_BUF_1, DATA_IDLE_1, DATA_SEND_1, FIND_CLUS_2, LOAD_BUF_2, DATA_IDLE_2, DATA_SEND_2};

// ISR for timer2
// runs the state machine
void Timer2_ISR_Init(void);

// Called by seos_run
void seos_init( uint32 First_clus );

// Starts the OS to play a song
//
// First_clus is the first cluster of
// the song to play
void seos_run( uint32 First_clus, uint8 idata *buffer );

// Gets next song cluster
// checks and sets end of file flag
uint32 Find_Cluster_And_Check_EOF( uint32 current_cluster, uint8 xdata *buffer );

void load_buffer( uint8 xdata *buffer );

void send_buffer( uint8 xdata *buffer );

void show_song_title( uint8 idata *buffer );

void update_MP3_Display( uint8 idata *buffer );

