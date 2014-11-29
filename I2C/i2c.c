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
	uint8 errorStatus = 0;
	uint8 outputByte;
	uint8 index;
	uint8 byteIndex;
	bit outputBit;

	////////////////
	// IDLE
	////////////////

	// Set SCL & SDA to 1
	SCL = 1;
	SDA = 1;

	// Check for bus busy condition
	if( SCL != 1 || SDA != 1 )
	{
		// Bus Busy Error
		errorStatus = 1;
		return errorStatus;
	}

	//////////////////
	// Send Address
	//////////////////

	// Set R/W bit to 0
	outputByte = (device_addr << 1);

	// Send address on i2c bus
	for( index = 0; index < 8; index++ )
	{
		// Delay and set SCL to 0
		delay( ClockDelay );
		SCL = 0;

	   	// Calculate bit to send
		outputBit = ((outputByte >> (7 - index)) & 0x01);

		// Send bit
		SDA = outputBit;

		// Set SCL to 1 after delay
		delay( ClockDelay );
		SCL = 1;

		// Wait for other devices to be 
		// ready
		while(SCL != 1);

		// Check for bus busy
		if( SDA != outputBit )
		{
			// Bus busy error
			errorStatus = 1;
			return errorStatus;
		}
	}

	///////////////////
	// Check for NACK
	///////////////////

	// After clock delay
	// Set SCL to 0 &
	// Set SDA to 1
	// This allows slave device to respond
	// with ACK
	delay( ClockDelay );
	SCL = 0;
	SDA = 1;

	// After clock delay,
	// Set SCL to 1 and wait
	// until SCL = 1
	delay( ClockDelay );
	SCL = 1;
	while( SCL != 1 );

	// Read SDA value
	// to check for NACK
	if ( SDA == 1 )
	{
		// NACK Received
		errorStatus = 2;
		return errorStatus;
	}

	///////////////////////////
	// Send all data bytes
	///////////////////////////
	byteIndex = 0;

	// Loop until all bytes sent
	while( byteIndex < number_of_bytes )
	{
		// Get current byte
		outputByte = array_name[index];
	
		// Send data byte on i2c bus
		for( index = 0; index < 8; index++ )
		{
			// Delay and set SCL to 0
			delay( ClockDelay );
			SCL = 0;
	
		   	// Calculate bit to send
			outputBit = ((outputByte >> (7 - index)) & 0x01);
	
			// Send bit
			SDA = outputBit;
	
			// Set SCL to 1 after delay
			delay( ClockDelay );
			SCL = 1;
	
			// Wait for other devices to be 
			// ready
			while(SCL != 1);
	
			// Check for bus busy
			if( SDA != outputBit )
			{
				// Bus busy error
				errorStatus = 1;
				return errorStatus;
			}
		}
	
		///////////////////
		// Check for NACK
		///////////////////
	
		// After clock delay
		// Set SCL to 0 &
		// Set SDA to 1
		// This allows slave device to respond
		// with ACK
		delay( ClockDelay );
		SCL = 0;
		SDA = 1;
	
		// After clock delay,
		// Set SCL to 1 and wait
		// until SCL = 1
		delay( ClockDelay );
		SCL = 1;
		while( SCL != 1 );
	
		// Read SDA value
		// to check for NACK
		if ( SDA == 1 )
		{
			// NACK Received
			errorStatus = 2;
			return errorStatus;
		}

		// Increment index
		byteIndex++;
	}

	/////////////////////////
	// Send Stop Condition
	/////////////////////////
	delay( ClockDelay );
	SCL = 0;
	SDA = 0;
	delay( ClockDelay );
	SCL = 1;
	while( SCL != 1 );
	SDA = 1;
	
	// Return error status
	return errorStatus;
}

uint8 I2C_Read(uint8 device_addr, uint8 number_of_bytes, uint8 *array_name)
{
	// Variable declaration
	uint8 error_status = 0;

	// Return error status
	return error_status;
}
