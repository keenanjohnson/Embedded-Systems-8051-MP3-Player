

#ifndef _PORT_H
#define _PORT_H

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
/*sbit P1_0 = P1^0;
sbit P1_1 = P1^1;
sbit P1_2 = P1^2;
sbit P1_3 = P1^3;
sbit nCS0 = P1^4;
//sbit MISO = P1^5;
//sbit SCK = P1^6;
//sbit MOSI = P1^7;	*/

// Port 2
sbit SW1 = P2^0;
sbit SW2 = P2^1;
sbit SW3 = P2^2;
sbit SW4 = P2^3;
sbit AMBERLED = P2^4;
sbit YELLOWLED = P2^5;
sbit GREENLED = P2^6;
sbit REDLED = P2^7;

// Port 3
sbit TxD_pin = P3^0;
sbit RxD_pin = P3^1;
/*sbit P3_2 = P3^2;
sbit P3_3 = P3^3;
sbit P3_4 = P3^4;
sbit P3_5 = P3^5;
sbit P3_6 = P3^6;
sbit P3_7 = P3^7; */

#endif
