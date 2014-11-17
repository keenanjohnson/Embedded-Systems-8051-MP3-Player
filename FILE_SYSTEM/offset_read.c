// offset_read.h
//
// Contains functions to read values 
// from a 512 byte block of data
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Nov. 2014

#include "offset_read.h"

uint8 read8(uint16 offset, uint8 *array_name)
{
	// Declare return value
	uint8 value = 0;

	// Check that offset is < 512
	if (offset < 512)
	{
		// Get value from array
		value = *(array_name + offset);
	}
	else // Offset > 511
	{
		printf( "Offset greater than 511" );
	}
 
	return value; 
}

uint16 read16(uint16 offset, uint8 *array_name)
{
	// Declare return value
	uint16 value = 0;

	// Other variable declarations
	uint8 i;
	uint8 byte;
	
	// Check that offset is < 512
	if (offset < 512)
	{
		// Read both bytes of uint16
		for( i=0; i<2; i++ )
		{
			// Read each byte starting with MSB
			byte = *( array_name + offset + (1-i) );
	
			// Shift return value left 8 bits
			value = value << 8;
	
			// OR byte with shifted return value
			value = value | byte;
		}
	}
	else // Offset > 511
	{
		printf( "Offset greater than 511" );
	}

	return value; 
}

uint32 read32(uint16 offset, uint8 *array_name)
{
	// Declare return value
	uint32 value = 0;

	// Other variable declarations
	uint8 i;
	uint8 byte;

	// Check that offset is < 512
	if (offset < 512)
	{
		// Read both bytes of uint16
		for( i=0; i<4; i++ )
		{
			// Read each byte starting with MSB
			byte = *( array_name + offset + (3-i) );
	
			// Shift return value left 8 bits
			value = value << 8;
	
			// OR byte with shifted return value
			value = value | byte;
		}
	}
	else // Offset > 511
	{
		printf( "Offset greater than 511" );
	}
 
	return value; 
}

