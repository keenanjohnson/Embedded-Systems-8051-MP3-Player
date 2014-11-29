// i2c.c
//
// Contains I2C functions
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Nov. 2014

#include "i2c.h"

uint8 I2C_Write(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name)
{
	// Variable declaration
	uint8 error_status = 0;

	// Set SCL & SDA to 1
	SCL = 1;
	SDA = 1;

	// Check for bus busy condition
	if( SCL != 1 || SDA != 1 )
	{
		// Bus Busy Error
		error_status = 1;
		return error_status;
	}

	// Delay and set SCL to 0
	delay( ClockDelay );
	SCL = 0;

	// Return error status
	return error_status;
}

uint8 I2C_Read(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name)
{
	// Variable declaration
	uint8 error_status = 0;

	// Return error status
	return error_status;
}
