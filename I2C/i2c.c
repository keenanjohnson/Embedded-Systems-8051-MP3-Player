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

void set_frequency( uint32 desired_frequency )
{
	uint16 reload;
	reload = 65536 - ((OSC_FREQ) / (OSC_PER_INST * 2 * desired_frequency));
	timer1_reload_high = reload << 8;
	timer1_reload_low = reload & 0x00FF;
}

void I2C_delay()
{
	TMOD &= 0x0F;
	TMOD |= 0x10;
	
	//Disable interrupts
	ET1 = 0;
	
	TH1 = timer1_reload_high;
	TL1 = timer1_reload_low;
	TF1 = 0;
	TR1 = 1;
	
	//Wait until timer ends
	while ( TF1 == 0 );
	
	//Stop the timer
	TR1 = 0;
	TF1 = 0;
	
	// Enable Timer1 Int
	ET1 = 1;
}

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
		I2C_delay();
		SCL = 0;

	   	// Calculate bit to send
		outputBit = ((outputByte >> (7 - index)) & 0x01);

		// Send bit
		SDA = outputBit;

		// Set SCL to 1 after delay
		I2C_delay();
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
	I2C_delay();
	SCL = 0;
	SDA = 1;

	// After clock delay,
	// Set SCL to 1 and wait
	// until SCL = 1
	I2C_delay();
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
			I2C_delay();
			SCL = 0;
	
		   	// Calculate bit to send
			outputBit = ((outputByte >> (7 - index)) & 0x01);
	
			// Send bit
			SDA = outputBit;
	
			// Set SCL to 1 after delay
			I2C_delay();
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
		I2C_delay();
		SCL = 0;
		SDA = 1;
	
		// After clock delay,
		// Set SCL to 1 and wait
		// until SCL = 1
		I2C_delay();
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
	I2C_delay();
	SCL = 0;
	SDA = 0;
	I2C_delay();
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
	uint8 timeout = 0;
	uint8 received_byte = 0;
	uint8 address_output_byte;
	uint8 temp;
	uint8 i;
	uint8 j;
	bit address_bit;
	
	
	//Setting initial states and verifying
	SCL = 1;
	SDA = 1;
	if (SCL != 1 || SDA != 1)
	{
		error_status = 1;
		return error_status;
	}
	
	//Setting R/W to 1
	address_output_byte = ((device_addr << 1) | 0x01);
		
	//Send the 7-bit device address and read bit
	for ( i = 0; i < 8; i++)
	{
		// After clock delay, clear SCL to 0
		I2C_delay();
		SCL = 0;
		
		//setting current bit
		address_bit = ((address_output_byte >> (7 - i)) & 0x01);
		SDA = address_bit;
		
		//After clock delay, set SCL to 1
		I2C_delay();
		if ( set_SCL() )
		{
			error_status = 2;
			return error_status;
		}
		
		//Read the value on SDA and verify that it is the same value sent
		if (SDA != address_bit)
		{
			error_status = 1;
			return error_status;
		}
	}
	
	//After a clock delay, clear SCL and set SDA
	I2C_delay();
	SCL = 0;
	SDA = 1;
	
	//After a clock delay, set SCL and wait until SCL = 1
	if ( set_SCL() )
	{
		error_status = 2;
		return error_status;
	}
	
	//Checking for ACK
	if ( SDA != 0 )
	{
		error_status = 3;
		return error_status;
	}

	//Read specified number of bytes
	for (i = 1; i <= number_of_bytes; i++)
	{
		//Reading received bits and placing resulting byte into array
		for ( j = 7; j <= 0; j--)
		{
			I2C_delay();
			SCL = 0;
			SDA = 1;
			I2C_delay();
			if ( set_SCL() )
			{
				error_status = 2;
				return error_status;
			}
			temp = SDA;
			received_byte |= (temp << j);
			temp = 0;
		}
		array_name[i] = received_byte;
		
		I2C_delay();
		SCL = 0;
		//Sending ACK for next byte
		if ( i < number_of_bytes )
		{	
			SDA = 0;
			I2C_delay();
			if ( set_SCL() )
			{
				error_status = 2;
				return error_status;
			}
		}
		//Sending NACK for last byte
		else
		{
			SDA = 1;
			I2C_delay();
			if ( set_SCL() )
			{
				error_status = 2;
				return error_status;
			}
			SDA = 1;
		}
	}
	
	//Send STOP after last byte is read
	I2C_delay();
	SCL = 0;
	SDA = 0;
	I2C_delay();
	if ( set_SCL() )
	{
		error_status = 2;
		return error_status;
	}
	SDA = 1;
	
	// Return error status
	return error_status;
}

uint8 set_SCL()
{
	// Variable Declarations
	uint8 timeout = 0;
	
	SCL = 1;
	do
	{
		timeout++;
	} while (SCL != 1 && timeout !=0);
	if (timeout == 0)
	{
		return 1;
	}
	return 0;
}