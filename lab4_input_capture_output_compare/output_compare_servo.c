/*
 * File:   output_compare_servo.c
 * Author: baska
 *
 * Created on March 13, 2018, 3:11 PM
 */


#include "xc.h"
#include "output_compare.h"

volatile int count5 = 0;

void initServo() {
    TRISB &= 0b1111111110011111;            //makes RB5 and RB6 outputs
                                            // RB5 is win LED indicator
                                            // RB6 is PWM generator for servo
    LATBbits.LATB5 = 0;                     //turns off win indicator LED
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18;                    // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    //Timer 3 is setup for Output Compare
    T3CON = 0x0010;     //Turn of timer 3, PRE 1:8 (T3CONbits <5:4> = 01)
    TMR3 = 0;           // Initialize timer 3 to zero
    PR3 = 39999;        // Set period to be 20 ms for proper servo operation
    //servo period / (PRE * 1/(frequency)) - 1 = 20ms / (8 * 1/(16 * 10^6)) - 1 = 39999
    T3CONbits.TON = 1;  // Turn on timer 3
    _T3IF = 0;          //reset timer 3 flag

    // Timer 3 setup should happen before this line
    OC1CON = 0;         // turn off OC1 for now
    OC1R = 2999;        // servo start position. We won't touch OC1R again
    //neutral position is duty cycle 1.5 ms on a 20 ms period
    //time high / (PRE * 1/(frequency)) - 1 = 1.5 ms / (8 * 1/(16 * 10^6)) - 1 = 2999

    OC1RS = 2999;           // We will only change this once PWM is turned on
    OC1CONbits.OCTSEL = 1;  // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults

    //setup timer 5 to check for winner after 5 seconds
    T5CON = 0x0030; //turn off timer 5, set pre-scalar to 1:256 (T5CONbits <5:4> = 11)
    TMR5 = 0;       // Initialize to zero (also best practice)
    PR5 = 62499;    // Set period to be 1 second
    //(PR5 + 1) * PRE * 1/frequency = (62499 + 1) * 256 * 1/(16*10^6) = 1 second

    _T5IF = 0;      //reset timer 5 flag
    _T5IE = 1;      //enable timer 5 interrupt

}

//set servo function lets us decide what our duty cycle is for the servo
void setServo(int Val) {
    
    //OC1RS is set as the input Val
    OC1RS = Val;
}

//if timer 5 overflows, one second has passed
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) {
    _T5IF = 0;                  //(reset timer 5 flag)
    count5++;                   //increment count5 flag
    if (count5 == 5) {          //if count5 flag has been incremented 5 times
                                //while one person has control,
        LATBbits.LATB5 = 1;     //light up LED
    }
    
    //if control of game is lost by a player, the count5 counter will reset to 0
    //(see main function)
}

