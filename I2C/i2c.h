// i2c.h
//
// Contains I2C functions
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Nov. 2014

#ifndef I2C_H
#define I2C_H

#include <stdio.h>

#include "Main.h"
#include "PORT.H"
#include "print.h"

#define I2C_FREQ (10000UL)
#define I2C_RELOAD (65536-((OSC_FREQ)/(OSC_PER_INST*I2C_FREQ*2)))
#define I2C_RELOAD_H (I2C_RELOAD/256)
#define I2C_RELOAD_L (I2C_RELOAD%256)

// Sets up I2C frequency
void set_frequency( uint32 desired_frequency );

//Begins I2C Delay sequence
void I2C_delay( void );

// Sends data out on the I2C bus
//
// device_addr -> the first byte sent
// number_of_bytes -> number of bytes in array_name
// array_name -> the array containing the data bytes 
//
// Returns an error code
//
// Error codes
// 0 -> No Error
// 1 -> Bus Busy Error
// 2 -> Device NACK Error
uint8 I2C_Write(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name);

// Reads data from the I2C bus
//
// device_addr -> the first byte sent
//number_of_bytes -> number of bytes in array_name
//array_name -> the array to which received data is written
//
//////RETURN VALUES//////
// Success = 0
// Bus Busy = 1
// Slave timeout = 2
// Improper ACK = 3
///////////////////////
uint8 I2C_Read(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name);

//Sets SCL and verifies
uint8 set_SCL();

#endif
