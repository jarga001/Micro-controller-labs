/*
 * File:   radio_main.c
 * Author: baska
 *
 * Created on May 2, 2018, 12:44 AM
 */


#include "xc.h"
#include "radio_transmitter.h"
// CW1: FLASH CONFIGURATION WORD 1
#pragma config ICS = PGx1
#pragma config FWDTEN = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF
// CW2: FLASH CONFIGURATION WORD 2
#pragma config I2C1SEL = PRI
#pragma config IOL1WAY = OFF
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSECME
#pragma config FNOSC = FRCPLL
 
volatile uint8_t test, test1;
 
int main(void) {
     
    CLKDIVbits.RCDIV = 0;       //set to 16 Mhz
    AD1PCFG = 0x9fff;           //makes pins digital
    TRISB=0;
    TRISB |= 0b0000010000000000; // RB10 as input MISO
    LATBbits.LATB5=0; // CE 
    LATBbits.LATB8=1; // CSN slave select
    
    __builtin_write_OSCCONL(OSCCON & 0xbf);     // unlock PPS
    RPINR20bits.SDI1R = 10; // Use Pin RP10= "10",SPI data input 
    RPOR3bits.RP6R = 7; //SPI1 Data Output at RB6
    RPOR3bits.RP7R = 8; //SPI1 Clock Output at RB7
    //RPOR4bits.RP8R = 9; //SPI1 Slave Select Output at RB8
    __builtin_write_OSCCONL(OSCCON | 0x40);     // lock   PPS
    
    spi_init();
    delay_x_amount_10us(100);
    nRF24L01_Config(1); // configured as transmitter
    
    LATBbits.LATB5=0; // CE 
    LATBbits.LATB8=0; // CSN slave select
   
    //test for debugging
    SPI_RW(0x20);
    SPI_RW(0x0e);
    SPI_RW(0x00);
    test = SPI_RW(NOP);
    

    
    while(1){
        
    }
    return 0;
}
