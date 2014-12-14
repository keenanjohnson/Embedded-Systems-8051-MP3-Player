#ifndef _SPI_H
#define _SPI_H

#include "Main.H"

//------- Public Constant definitions --------------------------------
// error values
#define init_okay (0)
#define illegal_clockrate (0x0F)
#define TIMEOUT_ERROR (0x8000)


// ------ Public function prototypes -------------------------------
uint8 SPI_Master_Init(uint32 clock_rate);
uint16 SPI_Transfer(uint8 data_input);

#endif