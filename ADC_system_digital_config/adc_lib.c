/*
 * File:   adc_lib.c
 * Author: baasanjav
 *
 * Created on July 4, 2018, 5:47 PM
 */


#include "xc.h"
#include "adc_lib.h"

volatile uint8_t lower_bit=0;
volatile uint8_t higher_bit=0;
volatile uint8_t config_bit=0;
volatile uint8_t tar_value_higher=0;
volatile uint8_t tar_value_lower=0;

void delay_xms(long x) { //approximate delay for x milliseconds
    long delay = 1600 * x;
    while (delay--);
}

void adc_config(char Package) {
    I2C2CONbits.SEN = 1; //Initiate Start condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
     
    I2C2TRN = 0b11010000; // 8-bits consisting of the slave address and the R/nW bit
         
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = Package; // 8-bits consisting of the data byte    00011000
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1; //send Stop condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
}

void adc_read()
{
    I2C2CONbits.ACKDT = 0; 
    I2C2CONbits.SEN = 1; //Initiate Start condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b11010001; // 8-bits consisting of the slave address and the R/nW bit
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.RCEN = 1; //set in read mode
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
        higher_bit=I2C2RCV; 
      
    I2C2CONbits.ACKEN=1;
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.RCEN = 1; //set in read mode
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
        lower_bit=I2C2RCV;
    
     I2C2CONbits.ACKEN=1;
     while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.ACKDT = 1;    
    
    I2C2CONbits.RCEN = 1; //set in read mode
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
        config_bit=I2C2RCV;
        
    I2C2CONbits.ACKEN=1;
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1; //send Stop condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
}

void i2c_init() {
    AD1PCFG = 0x9fff; //set pins digital
    I2C2CON = 0;
    I2C2BRG = 0x009D; //Baud Rate: Fcy = 16 Mhz, Fscl = 100 kHz
    I2C2CONbits.I2CEN = 1; //enable I2C protocol
    I2C2CONbits.ACKDT = 0; 
    IFS3bits.MI2C2IF = 0; //for safety's sake, make sure flag starts at 0  
}

void timer_init(){
    //Timer SETUP
    T1CON = 0x0020;     // Stop timer 2, make prescalar 1:64 (T2CONbits <5:4> = 0b11)
    TMR1 = 0;           // Initialize timer 2 to zero
    PR1 = 25000;        // Set period to be as large as possible to minimize overflows
    // _T1IP = 7;          //ensures time is recorded correctly by making timer 2 priority 1
    _T1IE = 1;          //enables timer 2 interrupt
    _T1IF = 0;
}

