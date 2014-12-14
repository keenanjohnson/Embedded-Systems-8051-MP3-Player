#ifndef _TWI_Soft_H
#define _TWI_Soft_H

#include "Main.H"

#define I2C_FREQ (10000UL)
#define I2C_RELOAD (65536-((OSC_FREQ)/(OSC_PER_INST*I2C_FREQ*2)))
#define I2C_RELOAD_H (I2C_RELOAD/256)
#define I2C_RELOAD_L (I2C_RELOAD%256)

#define Continue (1)
#define Stop (2)

#define STA013 (0x43)
#define no_errors (0)
#define bus_busy_error (0x81)
#define NACK_error (0x82)

// ------ Public function prototypes -------------------------------
uint8 I2C_WRITE(uint8 device_addr,uint8 num_bytes,uint8 idata * send_array);
uint8 I2C_READ(uint8 device_addr,uint8 num_bytes,uint8 idata * rec_array);
void I2C_clock_start(void);
void I2C_clock_delay(uint8 control);
uint8 STA013_init(void);



#endif