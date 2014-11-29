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

#include "AT89C51RC2.h"
#include "delay.h"
#include "Main.h"
#include "PORT.H"

uint8 I2C_Write(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name);
uint8 I2C_Read(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name);

#endif
