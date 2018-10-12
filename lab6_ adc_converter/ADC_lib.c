/*
 * File:   ADC_lib.c
 * Author: baska
 *
 * Created on April 18, 2018, 6:11 PM
 */


#include "xc.h"

void adc_init(){
    AD1PCFG = 0xfffe;
    TRISA |= 0x0001;
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CHS = 0;
    IEC0bits.AD1IE = 1;
    IFS0bits.AD1IF = 0;
    AD1CON3bits.ADCS=1;
    
    AD1CON1bits.SSRC = 0b010; // use timer 3
    AD1CON1bits.ASAM=1;       // auto convertion
    
    T3CON = 0x0020;     
    TMR3 = 0;           
    PR3 = 15624;        // configuring PR to take 16 samples per second
 
    T3CONbits.TON = 1;  // Turn on timer 3
    
    AD1CON1bits.DONE = 0;
    AD1CON1bits.ADON = 1; 
    AD1CON1bits.SAMP = 1;
}
