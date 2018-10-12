/*
 * File:   lab006_main.c
 * Author: baska
 *
 * Created on April 3, 2018, 10:25 PM
 */


#include <xc.h>
#include "jarga001_LCD.h"
#include <stdio.h>
#include <math.h>
#include "ADC_lib.h"
// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
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

volatile unsigned long int buffer[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                                        //intialize buffer values
volatile unsigned int i = 0;            //circular buffer index variable
volatile unsigned long int current_value;
volatile unsigned long int stdev;
volatile unsigned long int ave = 0;
volatile unsigned long int total = 0;
volatile unsigned long int stdevtotal = 0;


volatile unsigned long int old_ave = 0;
volatile unsigned long int new_ave = 0;

volatile unsigned long int old_stdev = 0;
volatile unsigned long int new_stdev = 0;

volatile unsigned long int temp_buff = 0;

void timer1_setup(void) {
    T1CON = 0x0020; //Turn off timer 1, PRE 1:64 (T3CONbits <5:4> = 01)
    TMR1 = 0; // Initialize timer 1 to zero
    PR1 = 25000; // set PR to configure 100ms
    _T1IF = 0; //reset timer 1 flag
    _T1IE = 1; //enable interrupt
    T1CONbits.TON = 1; // Turn on timer 1
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    _T1IF = 0; // reset t1 flag
    
    int j; // looping variable
    for (j = 0; j < 32; j++) {     // computing total
        total = total + buffer[j];
    }
    ave = total / 32;    //computing the average
    total = 0;
    int k;
    for (k = 0; k < 32; k++) {
        stdevtotal = stdevtotal + (ave - buffer[k])*(ave - buffer[k]); // computing the variance
    }
    stdev = sqrt(stdevtotal / 31);     // computing the standard deviation
    stdevtotal = 0;

    //current_value = buffer[i];
    char adSTR[20]; // creating a character array to hold values to print to the display
    sprintf(adSTR, "%6.4f V", (3.3 / 1024) * ave);  // converting the average to string
    lcd_setCursor(0, 0);                            // setting a cursor position to left top corner
    lcd_printString(adSTR);                         // printing to the LCD
    sprintf(adSTR, "%5.1f mV", ((3.3 / 1024) * stdev)*1000); // converting the standard deviation to string
    lcd_setCursor(1, 0);                            // setting the cursor position to the second row
    lcd_printString(adSTR);                         // printing to the LCD

}

void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    _AD1IF = 0;
    i = (i + 1) & 31;
    buffer[i] = ADC1BUF0; //putting the values to software buffer
}

int main(void) {
    lcd_init();          //initialize LCD
    timer1_setup();      // initialize timer1
    adc_init();          // intialize ADC unit   
    while (1);
    {

    }

    return 0;
}
