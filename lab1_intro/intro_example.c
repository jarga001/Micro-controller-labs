/*
 * File:   jarga001_Lab1_main_v001.c
 * Author: baska
 *
 * Created on January 15, 2018, 8:22 PM
 */


#include "xc.h"
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

void setup(void)
{
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
AD1PCFG = 0x9fff;
TRISA = 0b0000000000011111;
TRISB = 0b0000000000000011;
LATA = 0xffff;
LATB = 0xffff;
}

int main(void)
{
    unsigned short int count = 0;
    unsigned long int delay;
    setup();
    while(1) {
        // Execute repeatedly forever and ever and ever and ever ...
        count++;
        PORTB = count << 2;
        delay = 40;
        while (delay--) ;
        asm("nop");
        asm("nop");

    }
    return 0; 
}
