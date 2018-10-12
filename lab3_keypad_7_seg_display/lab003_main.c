/*
 * File:   display.c
 * Author: baska
 *
 * Created on February 9, 2018, 8:26 PM
 */

#include <stdio.h>
#include<stdio.h>
#include<string.h>
#include "xc.h"
//#include "jarga001_lab3_asmLib_v001.h"
#include "lab3_display.h"
#include "lab3_keypad.h"
//#include "jarga001_lab3_myLib.c"
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
//(see PIC24 Family Reference Manual 24.1)
// Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1) // Watchdog Timer Enable (Watchdog Timer is disabled)
// General Code Segment Write Protect (Writes to program memory are allowed) // General Code Segment Code Protect (Code protection is disabled)
// JTAG Port Enable (JTAG port is disabled)
//(see PIC24 Family Reference Manual 24.1)
// I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
// IOLOCK Protection (IOLOCK may be changed via unlocking seq)
// Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin) // Clock Switching and Monitor (Clock switching is enabled,
// Fail-Safe Clock Monitor is enabled)
// Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

void general_setup(void){
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;
}

void delay_20ms()
{
    int i;
    for(i=0;i<1;i++)
    {
        delay_2ms();
    }
}
void delay_1s()
{
    int i;
    for(i=0;i<100;i++)
    {
        delay_2ms();
    }
}

int main(void)
{
    
    char holder1;
    char holder2;
    char runner;
    general_setup();
    init_7_seg();
    init_keypad();
    
    
    while(1) {
        // Execute repeatedly forever and ever and ever and ever ... 
        while(runner!='!'){
            runner=readKeyPadRAW();
            showChar7seg(holder1,LSB);
            delay_20ms();
            showChar7seg(holder2,MSB);
            delay_20ms();
        }
        while(runner=='!'){
            runner=readKeyPadRAW();
            showChar7seg(holder1,LSB);
            delay_20ms();
            showChar7seg(holder2,MSB);
            delay_20ms();   
        }
        holder2=holder1;
        holder1=runner;   
    }
    return 0; 
}
