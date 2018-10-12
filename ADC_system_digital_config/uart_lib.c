/*
 * File:   uart_lib.c
 * Author: baasanjav
 *
 * Created on July 9, 2018, 4:11 PM
 */


#include "xc.h"

void uart1_send(const unsigned char c)
{
    while (!IFS0bits.U1TXIF) ;
    IFS0bits.U1TXIF = 0;
    U1TXREG = c;
}

unsigned char uart1_receive(void)
{
    unsigned char x;
    x = U1RXREG;
    return x;
}

void setup_uart1(void)
{
    U1MODE = 0;  // UEN<1:0> bits control the pins , 8 bits data, no parity, 1 stop
  
    //U1BRG = 25; // 38400 baud (check the calculation with the datasheet) when   U1MODEbits.BRGH = 0;
    
    U1MODEbits.BRGH = 1;
    U1BRG = 34; // 115200 baud,
    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1; //uart enabled
    U1STAbits.UTXEN = 1; // uart transmit enabled 

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10;   //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}


