/*
 * File:   lab004_main.c
 * Author: baska
 *
 * Created on February 28, 2018, 2:39 PM
 */

#include "xc.h"
#include "input_capture.h"
#include "output_compare.h"

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

int main(void) {
    AD1PCFG = 0x9fff;           //makes pins digital
    initServo();                //initialize servo
    initTwoPushButtons();       //initialize push buttons
    CLKDIVbits.RCDIV = 0;       //set to 16 Mhz
    setServo(2999);             //initializes servo to middle position


    while (1) {

        //if user 2 is winning (his period is smaller than user 1's),
        //and he is not sleeping, do these things:
        if (avePeriod1 > avePeriod2 && sleep2 == 0) {
            
            //point servo towards him
            setServo(2999 + 600);
            
            //start the 5 second timer (timer 5)
            TMR5 = 0;
            T5CONbits.TON = 1;
            
            //hold the program here so long as user 2 is winning
            //and is not asleep. If user 2 begins losing or 
            //goes to sleep...
            while (avePeriod1 > avePeriod2 && sleep2 == 0);
            
            //...reset timer 5
            T5CONbits.TON = 0;
            count5 = 0;
            
            
        //if user 1 is winning (his period is smaller than user 2's),
        //and he is not sleeping, do these things:
        } else if (avePeriod1 < avePeriod2 && sleep1 == 0) {
            
            //point servo towards him
            setServo(2999 - 600);
            
            //start the 5 second timer (timer 5)
            TMR5 = 0;
            T5CONbits.TON = 1;
            
            //hold the program here so long as user 1 is winning
            //and is not asleep. If user 1 begins losing or 
            //goes to sleep...
            while (avePeriod1 < avePeriod2 && sleep1 == 0) {
            }

            //...reset timer 5
            T5CONbits.TON = 0;
            count5 = 0;
            
            //otherwise, the program is at the beginning of the game
            //or both users are asleep, in which case...
        } else {
            //make the servo point to a neutral position
            setServo(2999);
        }
    }
    return 0;
}
