// Port.h
//
// Pin definitions
//
// Keenan Johnson
// Franco Santa-Maria
//
// CpE 4160
// FALL 2014

#ifndef _PORT_H
#define _PORT_H

#include "Main.h"

// Port 0
sbit DB0 = P0^0;
sbit DB1 = P0^1;
sbit DB2 = P0^2;
sbit DB3 = P0^3;
sbit DB4 = P0^4;
sbit DB5 = P0^5;
sbit DB6 = P0^6;
sbit DB7 = P0^7;

// Port 1
sbit DRST = P1^0;
//sbit P1_1 = P1^1;
//sbit P1_2 = P1^2;
//sbit P1_3 = P1^3;
sbit SPI_nCS0 = P1^4;
sbit SPI_MISO = P1^5;
sbit SPI_SCK = P1^6;
sbit SPI_MOSI = P1^7;

// Port 2
sbit SW1 = P2^0;
sbit SW2 = P2^1;
sbit SW3 = P2^2;
sbit SW4 = P2^3;
sbit REDLED = P2^4;
sbit YELLOWLED = P2^5;
sbit AMBERLED = P2^6;
sbit GREENLED = P2^7;

// Port 3
sbit TxD_pin = P3^0;
sbit RxD_pin = P3^1;
//sbit P3_2 = P3^2;
//sbit P3_3 = P3^3;
sbit SCL = P3^4;
sbit SDA = P3^5;
sbit RS = P3^6;
sbit E = P3^7;

#endif
