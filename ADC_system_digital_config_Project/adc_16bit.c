/*
 * File:   adc_16bit.c
 * Author: baasanjav
 *
 * Created on June 20, 2018, 3:09 PM
 */


#include "xc.h"
#include "adc_lib.h"
#include "uart_lib.h"
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


void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    _T1IF = 0;          //(reset timer 2 flag)
    adc_read();
   uart1_send(higher_bit - tar_value_higher);
   uart1_send(lower_bit - tar_value_lower);
}

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt(void){
    delay_xms(100);
    _INT0IF=0;
    tar_value_higher = higher_bit;
    tar_value_lower = lower_bit;
}

void setup_button(){
    _TRISB7=1; //this button is input
    _CN23PUE=1; // pull up resistor
    _INT0EP=1; // falling edge
    _INT0IE=1; // enable interrupt
    _INT0IF=0; // reset flag
}

int main(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;  // For digital I/O.
    i2c_init();
    timer_init();
    setup_uart1();
    setup_button();
    adc_config(0b00011000);
    //delay_xms(100);
    //adc_read();
    T1CONbits.TON = 1;  // turn on Timer2
    
    while(1){
       
    }
    
    return 0;
}
