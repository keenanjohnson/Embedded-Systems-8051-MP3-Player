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
	
	//Initialize the SPI peripheral to a maximum of 400khz
	spi_master_init(400000);
	
	//Transfers 10 bytes without selecting the peripheral
	//This allows the SD card to receive the necessary 74
	//clock pulses to complete its setup routine
	printf("Sending 80 clock pulses");
	print_newline();

	for(i=0;i<=10;i++)

	{
		spi_transfer(0xFF);

	}

	//Send CMD0 to SD card
	SPI_nCS0 = 0;
	printf("Sending CMD0");
	print_newline();
	send_command( 0 , 0x00000000 );
	
	//Waits for response from SD card and checks for expected value
	printf("Waiting for response");
	print_newline();

	if ( !receive_response( 1 , &response) )
	{
		printf("Initialization Error: No Response\n");
		print_newline();

		return 1;
	}
	SPI_nCS0=1;
	if (response[1] !=0x01)
	{
		printf("Initialization Error: Unexpected Response");
		print_newline();

		return 1;
	}

	printf("0x01 received");
	print_newline();
	
	//0x01 received, send CMD8 to SD card
	printf("sending CMD8");
	print_newline();

	// Set Chip Select Low
	SPI_nCS0=0;
	send_command( 8 , 0x000001AA );
	
	//Waits for response from SD card and checks for expected value
	printf("Waiting for response");
	print_newline();

	if ( !receive_response( 5 , &response) )
	{
		printf("Initialization Error: No Response");
		print_newline();

		return 1; 
	}

	// Set Chip Select High
	SPI_nCS0=1;
	
	//if the first byte is 0x01, four bytes will follow
	if (response[1] == 0x01 && response[2] == 0x00 && response[3] == 0x00 && response[4] == 0x01 && response[5] == 0xAA)
	{
		printf("Response received");
		print_newline();
	}
	//if the first byte is 0x05, SD card is an old version and no more bytes will follow
	else if ( response[1] == 0x05 )
	{
		printf("Version 1.x detected");
		print_newline();

		version_1=1;
	}
	else
	{
		printf("Initialization Error: Unexpected Response");
		print_newline();

		return 1;
	}
	
	//Response received, send CMD58
	printf("sending CMD58");
	print_newline();

	SPI_nCS0=0;
	send_command( 58 , 0x00000000 );
	
	printf("Waiting for response");
	print_newline();

	if ( !receive_response( 5 , &response) )
	{
		printf("Initialization Error: No Response");
		print_newline();

		return 1; 
	}
	SPI_nCS0=1;
	
	if ( !response[1] == 0x3F )
	{
		printf("Initialization Error: Unexpected Response");
		print_newline();

		return 1;
	}
	printf("Response received");
	print_newline();
	
	//check that SD card voltage range is acceptable
	if ( (((response[4] & 0x08) != 0x08) && ((response[4] & 0x04) != 0x04)) )
	{
		printf("Initialization error: Voltage not compatible");
		print_newline();

		return 1;
	}
	
	else
	{
		printf("SDSC found");
		print_newline();
	}
	
	//send CMD55 followed by ACMD41 until card returns ready
	SPI_nCS0=0;
	do
	{
		//send CMD55
		printf("sending CMD55");
		print_newline();

		send_command( 55 , 0x00000000 );
		
		//send ACMD41. Older versions should have an argument of 0.
		//Newer versions should have an argument of 2 for the last nibble
		printf("sending ACMD41");
		print_newline();

		if (!version_1)
		{
			send_command( 41 , 0x00000002 );
		}
		else
		{
			send_command( 41 , 0x00000000 );
		}
		if (response[1] == 0x00)
		{
			break;
		}
		else if (response[1] == 0x01)
		{}
		else
		{
			printf("Initialization error: unexpected response");
			print_newline();

			return 1;
		}
		timeout++;
	} while ( timeout != 0x00 );
	SPI_nCS0=1;
	
	//V2 cards require CMD58 again to check capacity
	if (!version_1)
	{
		printf("sending CMD58");
		print_newline();

		SPI_nCS0=0;
		send_command( 58 , 0x00000000 );
		
		printf("Waiting for response");
		print_newline();

		if ( !receive_response( 5 , &response) )
		{
			printf("Initialization Error: No Response");
			print_newline();

			return 1; 
		}
		SPI_nCS0=1;
		
		if ( (response[5] & 0x01) == 0x01 )
		{
			if ( ((response[5] & 0x02) == 0x02)  )
			{
				printf("SDHC/XC found");
				print_newline();

				card_type = 1;
			}
			//if SDSC is found, set block size to 512 bytes
			else
			{
				printf("SDSC found. Sending CMD16");
				print_newline();

				send_command( 16 , 512 );
				if ( !receive_response( 5 , &response) )
				{
					printf("Initialization Error: No Response");
					print_newline();

					return 1; 
				}
				printf("Block size set to 512 bytes");
				print_newline();
			}
		}
		else
		{
			printf("Initialization Error: Invalid response");
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
	uint16 spi_status;
	uint8 shift_amount;
	uint8 i; 


	// Check that command is only 6 bits
	if ( command > 63 )
	{
		// Set error flag and return
		error_status = 1;
		return error_status;
	}		

	// Apend start and transmission bits
	byte_to_send = command | 0x40;

	//////////////////
	// Send Byte 1
	// Command Byte
	//////////////////
	spi_status = spi_transfer( byte_to_send );

	//////////////////////
	// Send Bytes 2-5
	// Argument
	//////////////////////

	// Start shift amount at 24
	shift_amount = 24;

	// Send 4 bytes
	for ( i=0; i++; i<4)
	{
		if((spi_status & 0xF000) == 0) // Check errors, 0 means no errors
		{
			// Shift argument to get correct byte
			byte_to_send = ( argument >> shift_amount );

			// Send out SPI
			spi_status = spi_transfer(byte_to_send);

			// Decrement shift_amount by 8
			shift_amount = shift_amount - 8; 
		}
		else  // Error Condition
		{
			// Set error flag and return
			error_status = 1;
			return error_status;
		}
	}

	//////////////////
	// Send Byte 6
	// Checksum
	//////////////////

	if((spi_status & 0xF000) == 0)
	{
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

		// Other
		else
		{
			byte_to_send = 0x01;
		}

		error_status = spi_transfer( byte_to_send );
	}
	else
	{
		error_status = 1;
	}

	// Return error status
	return error_status;   
}

uint8 receive_response( uint8 number_of_bytes, uint8 *response_array )
{
	// Variable Declaration
	uint8 error_status;
	uint8 spi_return;
	uint8 timeout_counter = 0;
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
		error_status = ( spi_return & 0xF000 )>>8;

		// Get lower byte of spi return
		response_value = spi_return & 0x00FF;

		// Increment timeout counter
		timeout_counter++;

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
		error_status = 1;

		// ABORT
		return error_status;		
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

			// Place data in response buffer
			*( response_array + index) = response_value;
		}
	}

	// Send out final 0xFF
	// Ignore received value
	spi_transfer( 0xFF );

	// Return Error Status
	return error_status;
}

uint8 read_block( uint16 number_of_bytes, uint8 *array )
{
	return 0;	
}

