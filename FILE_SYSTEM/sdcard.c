// sdcard.c
//
// Handles interfacing with an SD card
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// Oct. 2014

#include "sdcard.h"

uint8 initialize_card()
{
	uint8 response[6];
	uint8 i;
	uint8 card_type=0;
	uint8 timeout=0x00;
	uint8 version_1=0;
	uint16 spi_return;
	uint8 cmd_error_status;
	
	//Transfers 10 bytes without selecting the peripheral
	//This allows the SD card to receive the necessary 74
	//clock pulses to complete its setup routine
	printf("Sending 10 bytes of clock pulse");
	print_newline();

	// Set chip select high
	SPI_nCS0 = 1;

	for(i=0;i<10;i++)
	{
		spi_transfer(0xFF);
	}

	//Send CMD0 to SD card
	SPI_nCS0 = 0;
	printf("Sending CMD0...");
	print_newline();

	cmd_error_status = send_command( 0 , 0x00000000 );
	if( cmd_error_status != 0 )
	{
		printf("Command Send Error");
		print_newline();		
	}

	// Check for SPI error
	spi_return = receive_response(1 , response);
	if ( spi_return != 0 )
	{
		printf( "SPI Error Status %2.2bX", spi_return );
		print_newline();
		return 1;
	}

	SPI_nCS0=1;

	// Check for good R1 response
	if ( response[0] != 1 )
	{
		printf("Initialization Error: Unexpected Response: %2.2bX", response[0]);
		print_newline();
		return 1;
	}

	printf("0x01 received");
	print_newline();
	
	//0x01 received, send CMD8 to SD card
	printf("sending CMD8...");
	print_newline();

	// Set Chip Select Low
	SPI_nCS0=0;
	send_command( 8 , 0x000001AA );
	
	spi_return = receive_response(5 , response);
	if ( spi_return != 0 )
	{
		printf( "SPI Error Status %2.2bX", spi_return );
		print_newline();
		return 1;
	}

	// Set Chip Select High
	SPI_nCS0=1;
	
	//if the first byte is 0x01, four bytes will follow
	if (response[0] == 0x01 && response[1] == 0x00 && response[2] == 0x00 && response[3] == 0x01 && response[4] == 0xAA)
	{
		printf("Version 2.x detected");
		print_newline();
	}
	//if the first byte is 0x05, SD card is an old version and no more bytes will follow
	else if ( response[0] == 0x05 )
	{
		printf("Version 1.x detected");
		print_newline();

		version_1 = 1;
	}
	else
	{
		printf("Initialization Error: Unexpected Response 1: %2.2bX", response[0]);
		print_newline();
		printf("Initialization Error: Unexpected Response 2: %2.2bX", response[1]);
		print_newline();
		printf("Initialization Error: Unexpected Response 3: %2.2bX", response[2]);
		print_newline();
	   	printf("Initialization Error: Unexpected Response 4: %2.2bX", response[3]);
		print_newline();
		printf("Initialization Error: Unexpected Response 5: %2.2bX", response[4]);
		print_newline();
		printf("Initialization Error: Unexpected Response 6: %2.2bX", response[5]);
		print_newline();
		return 1;
	}
	
	//Response received, send CMD58
	printf("sending CMD58...");
	print_newline();

	SPI_nCS0=0;
	send_command( 58 , 0x00000000 );

	spi_return = receive_response( 5 , response);
	if ( spi_return != 0 )
	{
		printf( "SPI Error Status %2.2bX", spi_return );
		print_newline();
		return 1;
	}
	SPI_nCS0=1;
	
	if ( response[0] != 0x01 )
	{
		printf("Initialization Error: Unexpected Response: %2.2bX", response[0]);
		print_newline();
		return 1;
	}

	//check that SD card voltage range is acceptable
	if ( ((response[2] & 0x3C) != 0x3C) )
	{
		printf("Initialization error: Voltage not compatible");
		print_newline();
		printf("%2.2bX", response[0]);
		printf(" %2.2bX", response[1]);
	 	printf(" %2.2bX", response[2]); 
		printf(" %2.2bX", response[3]);
	 	printf(" %2.2bX", response[4]);
		print_newline();
		return 1;
	}
	
	else
	{
		printf("SD Card Voltage Compatible");
		print_newline();
	}
	
	//send CMD55 followed by ACMD41 until card returns ready
	SPI_nCS0=0;
	timeout=0;
	do
	{
		//send CMD55
		printf("sending CMD55");
		print_newline();

		send_command( 55 , 0x00000000 );

		spi_return = receive_response( 1 , response );
		if ( spi_return != 0 )
		{
			printf( "SPI Error Status %2.2bX", spi_return );
			print_newline();
			return 1;
		}
		
		if ( response[0] != 0x01 )
		{
			printf("Initialization Error: Unexpected Response: %2.2bX", response[0]);
			print_newline();
			return 1;
		}

		//send ACMD41. Older versions should have an argument of 0.
		//Newer versions should have an argument of 2 for the last nibble
		printf("sending ACMD41");
		print_newline();

		if ( version_1 == 0 )
		{
			send_command( 41 , 0x40000000 );
		}
		else
		{
			send_command( 41 , 0x00000000 );
		}

		spi_return = receive_response( 1 , response );
		if ( spi_return != 0 )
		{
			printf( "SPI Error Status %2.2bX", spi_return );
			print_newline();
			return 1;
		}

		if (response[0] == 0x00)
		{
	   		printf("Active State Received");
			print_newline();
			break;
		}
		else if (response[0] == 0x01)
		{
			printf("Idle State Received");
			print_newline();
		}
		else
		{
			printf("Initialization Error: Unexpected Response: %2.2bX", response[0]);
			print_newline();
			return 1;
		}
		timeout++;
		if ( timeout == 0x00 )
		{
			printf("Initialization Error: ACMD41 timeout");
			print_newline();
			return 1;
		}
	} while ( 1 );
	SPI_nCS0=1;
	
	//V2 cards require CMD58 again to check capacity
	if ( version_1 == 0 )
	{
		printf("sending CMD58");
		print_newline();

		SPI_nCS0=0;
		send_command( 58 , 0x00000000 );
		
		printf("Waiting for response");
		print_newline();

		spi_return = receive_response( 5 , response);
		if ( spi_return != 0 )
		{
			printf( "SPI Error Status %2.2bX", spi_return );
			print_newline();
			return 1;
		}
		SPI_nCS0=1;

		if ( (response[1] & 0xC0)  == 0xC0 )
		{
			printf("SDHC/XC found");
			print_newline();
			card_type = 1;
		}
		//if SDSC is found, set block size to 512 bytes
		else if ( (response[1] & 0xC0) == 0x80 )
		{
			printf("SDSC found. Sending CMD16");
			print_newline();

			send_command( 16 , 512 );
			spi_return = receive_response( 1 , response );
			if ( spi_return != 0 )
			{
				printf( "SPI Error Status %2.2bX", spi_return );
				print_newline();
				return 1;
			}
			printf("Block size set to 512 bytes");
			print_newline();
		}
		else
		{
			printf("Initialization Error: Unexpected Response: %2.2bX", response[0]);
			printf("%2.2bX", response[1]);
			printf("%2.2bX", response[2]);
			printf("%2.2bX", response[3]);
			printf("%2.2bX", response[4]);
			print_newline();
			return 1;
		}
	}
	printf("Initialization successful");
	print_newline();

	return 0;
}

uint8 send_command( uint8 command, uint32 argument )
{
	// Variables
	uint8 error_status = 0;
	uint8 byte_to_send;
	uint16 spi_return;
	uint8 first_byte;

	// Check that command is only 6 bits
	if ( command > 63 )
	{
		// Set error flag and return
		error_status = 1;
		return error_status;
	}		

	//////////////////
	// Send Byte 1
	// Command Byte
	//////////////////
	
	// Apend start and transmission bits
	first_byte = command;
	first_byte &= 0x7F;
	first_byte |= 0x40;
	byte_to_send = first_byte;

	// Send
	spi_return = spi_transfer( byte_to_send );

	// Check for error
	if((spi_return & 0xF000) != 0)
    {
		// Error
		return 1;
	}

   	//////////////////
	// Send Byte 2
	// Argument Byte
	//////////////////

	// Shift argument to get correct byte
	byte_to_send = ( argument >> 24 );

	// Send out SPI
	spi_return = spi_transfer(byte_to_send);

	// Check for error
	if((spi_return & 0xF000) != 0)
    {
		// Error & Abort
		return 1;
	}

	//////////////////
	// Send Byte 3
	// Argument Byte
	//////////////////

	// Shift argument to get correct byte
	byte_to_send = ( argument >> 16 ) & 0xFFUL;

	// Send out SPI
	spi_return = spi_transfer(byte_to_send);

	// Check for error
	if((spi_return & 0xF000) != 0)
    {
		// Error & Abort
		return 1;
	}

	//////////////////
	// Send Byte 4
	// Argument Byte
	//////////////////

	// Shift argument to get correct byte
	byte_to_send = ( argument >> 8 ) & 0xFFUL;

	// Send out SPI
	spi_return = spi_transfer(byte_to_send);

	// Check for error
	if((spi_return & 0xF000) != 0)
    {
		// Error & Abort
		return 1;
	}

	//////////////////
	// Send Byte 5
	// Argument Byte
	//////////////////

	// Shift argument to get correct byte
	byte_to_send = argument & 0xFFUL;

	// Send out SPI
	spi_return = spi_transfer(byte_to_send);

	// Check for error
	if((spi_return & 0xF000) != 0)
    {
		// Error & Abort
		return 1;
	}

	//////////////////
	// Send Byte 6
	// Checksum
	//////////////////

	// CMD 0
	if( command == 0 )
	{
		byte_to_send = 0x95;
	}
	// CMD 8
	else if( command == 8 )
	{
		byte_to_send = 0x87;
	}
	// Other CMD
	else
	{
		byte_to_send = 0x01;
	}

	// Send value and check for errors
	spi_return = spi_transfer( byte_to_send );
	if((spi_return & 0xF000) != 0)
    {
		// Error
		return 1;
	}

	// Return error status
	return error_status;   
}

uint8 receive_response( uint8 number_of_bytes, uint8 *response_array )
{
	// Variable Declaration
	uint8 error_status;
	uint8 spi_return;
	uint8 timeout_counter = 1;
	uint8 response_value;
	uint8 index;

	// Send out 0xFF repeatedly
	// Wait until received byte is
	// not 0xFF or until a timeout
	// has occured
	do
	{
		// Send 0xFF
		spi_return = spi_transfer( 0xFF );

	    // Get error status from transfer
		error_status = (spi_return & 0xF000)>>8;

		// Get lower byte of spi return
		response_value = spi_return & 0x00FF;

		// Increment timeout counter
	    // timeout_counter++;

		// Check for valid response, error, or timeout
	} while( (response_value == 0xFF) && (error_status == 0x00) && (timeout_counter != 0) );

	// Check for timeout 
	if ( timeout_counter == 0 )
	{
		// Timeout error occured
		error_status = 1;

		// ABORT
		return error_status;
	}

	// Check for SPI error
	if ( error_status != 0x00 )
	{
		// Timeout error occured
		error_status = 2;

		// ABORT
		return error_status;		
	}

	// Check R1 response for errors
	if( !(response_value == 0x01 || response_value == 0x00) )
	{
		// Error and abort
		return 3;
	}

	// Assign reponse to buffer
	*response_array = response_value;

	// Check if there are more bytes in the response
	if( number_of_bytes > 1 )
	{
		// Get remaining bytes in response
		for(index = 1; index < number_of_bytes; index++)
		{
			// Get SPI return
			spi_return = spi_transfer( 0xFF );

			// Get data value
			response_value = spi_return & 0x00FF;

			// Check for error status
			if((spi_return & 0xF000) != 0)
		    {
				// Error
				return 4;
			}

			// Place data in response buffer
			*( response_array + index) = response_value;
		}
	}

	// Send out final 0xFF
	// Ignore received value
	spi_transfer( 0xFF );

	if((spi_return & 0xF000) != 0)
    {
		// Error
		return 5;
	}

	// Return Error Status
	return error_status;
}

uint8 read_sd_block( uint16 number_of_bytes, uint8 *array )
{
	uint8 response[5];
	uint16 timeout;
	uint16 spi_response;
	uint8 spi_data;
	uint8 spi_error_status;
	uint16 index;

	// Check for good R1 response
	printf("Read block");
	print_newline();
	printf("Waiting for R1 response...");
	print_newline();

	// Check for SPI error
	spi_response = receive_response(5 , response);
	if ( spi_response != 0 )
	{
		printf("SPI Error");
		print_newline();
		return 1;
	}

	// Check that R1 is 0x00
	if ( response[0] != 0x00 )
	{
		printf("R1 Error");
		print_newline();
		return 1;
	}

	// Wait for data token
	timeout = 0;
	do
	{
		spi_response = spi_transfer( 0xFF );
		spi_error_status = (spi_response >> 8);
		spi_data = (spi_response & 0x00FF);
		timeout++;
	} while((timeout != 0) && (spi_data == 0xFF) && (spi_error_status == 0));

	// Check for timeout 
	if ( timeout == 0 )
	{
		printf("Read Block Timeout");
		print_newline();

		// ABORT
		return 1;
	}
	// Check for SPI error
	if ( spi_error_status != 0x00 )
	{
		printf("Read Block SPI Error");
		print_newline();
		
		// ABORT
		return 2;		
	}
	// Check if we got start token
	if ( spi_data != 0xFE )
	{
		// Receive error token
		printf("Received error token");
		print_newline();

		return 1;
	}

	printf("Got the data start token");
	print_newline();

	// Receive data bytes
	for ( index=0; index<(number_of_bytes); index++ )
	{
		spi_response = spi_transfer( 0xFF );
		spi_error_status = (spi_response >> 8);
		spi_data = (spi_response & 0x00FF);	

		// Check for error
		if ( spi_error_status != 0 )
		{
			printf("Error receiving block data");
			print_newline();
			return 1;
		}
		else // No errors
		{
			// Receive data
			array[index] = spi_data;
		}
	}

	// Discard checksum
	printf("Received data. Sending three more bytes");
	print_newline();

	for(index = 0; index < 3; index++) //Discarding checksum (first two bytes)
	{
		spi_response = spi_transfer( 0xFF );
	}

	printf("Exiting print block");
	print_newline();

	// Return
	return 0;	
}

uint8 load_sector( uint32 sector, uint8 *array )
{
	// Declare variables
	uint8 error_code = 0;

	// Set Chip Select Low
	SPI_nCS0 = 0;

	// Send read command
	error_code = send_command( 17 , sector );

	// Check error status
	if ( error_code != 0 )
	{
		// Send command error
		error_code = 1;
		return  error_code;
	}

	// Read data response
	read_sd_block( 512, array );

	// Check error status
	if ( error_code != 0 )
	{
		if ( error_code == 1 )
		{
			// Receive timeout error
			error_code = 2;
		}
		else if( error_code == 2 )
		{
			// Receive Response SPI Error
			error_code = 3;
		}
	}

	// Set Chip Select High
	SPI_nCS0 = 1;
	
	// Return error code
	return error_code;
}

