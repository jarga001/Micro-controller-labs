/*
 * File:   input_capture_buttons.c
 * Author: baska
 *
 * Created on March 13, 2018, 2:57 PM
 */


#include "xc.h"
#include "input_capture.h"

volatile unsigned long int overflow = 0;
volatile unsigned long int buffer1[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
volatile unsigned long int buffer2[4] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
volatile unsigned long int curPeriod1 = 0;
volatile unsigned long int avePeriod1 = 0xffffffff;
volatile unsigned long int curPeriod2 = 0;
volatile unsigned long int avePeriod2 = 0xffffffff;
volatile int sleep1 = 0;
volatile int sleep2 = 0;



//timer 2 is associated with IC. It will help keep track of the time passed.
//when the timer 2 overflows...
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    _T2IF = 0;          //(reset timer 2 flag)
    overflow++;         //increment the overflow counter to keep track of
                        //the number of times timer 2 has overflowed
}

void initTwoPushButtons() {

    TRISB |= 0b0000001100000000;                //initialize RB8 and RB9 as input pins

    //initialize internal pull-up resistors for pins RB8 and RB9
    //(associated with buttons 1 and 2)
    CNPU2bits.CN21PUE = 1;
    CNPU2bits.CN22PUE = 1;

    __builtin_write_OSCCONL(OSCCON & 0xbf);     // unlock PPS
    RPINR7bits.IC1R = 9;                        // Use Pin RP9 = "9", for Input Capture 1 (Table 10-2)
    RPINR7bits.IC2R = 8;                        // Use Pin RP8 = "8", for Input Capture 2 (Table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40);     // lock   PPS


    T2CON = 0x0030;     // Stop timer 2, make prescalar 1:256 (T2CONbits <5:4> = 0b11)
    TMR2 = 0;           // Initialize timer 2 to zero
    PR2 = 65535;        // Set period to be as large as possible to minimize overflows
    T2CONbits.TON = 1;  // turn on Timer2
    _T2IP = 1;          //ensures time is recorded correctly by making timer 2 priority 1
    _T2IE = 1;          //enables timer 2 interrupt


    IC1CON = 0;             // Turn off and reset internal state of IC1
    IC1CONbits.ICTMR = 1;   // Use Timer 2 for capture source
    IC1CONbits.ICM = 0b010; // Turn on and capture every falling edge


    IC2CON = 0;             // Turn off and reset internal state of IC2
    IC2CONbits.ICTMR = 1;   // Use Timer 2 for capture source
    IC2CONbits.ICM = 0b010; // Turn on and capture every rising edge
    


    //set up timer 1 for checking for 1 second sleep duration
    T1CON = 0x0030;         //Stop Timer 1, make pre-scalar 1:256 (T1CONbits <5:4> = 0b11)
    TMR1 = 0;               // Initialize timer 1 to zero
    PR1 = 62499;            // Set period to be 1 second
    //(PR1 + 1) * PRE * 1/frequency = (62499 + 1) * 256 * 1/(16*10^6) = 1 second

    //set up timer 4 for checking for 1 second sleep duration
    T4CON = 0x0030;         //Stop Timer 4, make pre-scalar 1:256 (T4CONbits <5:4> = 0b11)
    TMR4 = 0;               // Initialize timer 4 to zero
    PR4 = 62499;            // Set period to be 4 second
    //(PR4 + 1) * PRE * 1/frequency = (62499 + 1) * 256 * 1/(16*10^6) = 1 second

    //turn on both timers
    T1CONbits.TON = 1;
    T4CONbits.TON = 1;

    //resets both timer flags
    _T1IF = 0;
    _T4IF = 0;

    //enables IC1, IC2, timer 1 and timer 4 interrupts
    _IC1IE = 1;
    _IC2IE = 1;
    _T1IE = 1;
    _T4IE = 1;
}

//if button 1 has been pressed...
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void) {
    // Process the captured values

    //set up variables
    static unsigned long int prevEdge1, curEdge1;
    static unsigned long int prevOverflow1, curOverflow1;
    static unsigned int i = 0;

    //reset IC1 flag
    _IC1IF = 0;

    //if button 1 was pressed, user 1 is not asleep,
    //so set his sleep flag to 0
    sleep1 = 0;
    //set timer 1 to zero to check for his sleep status over the next second
    TMR1 = 0;

    //while the FIFO buffer has data
    while (IC1CONbits.ICBNE == 1) {
        //grab most recent TMR1 value and store as curEdge1
        curEdge1 = IC1BUF;
    }

    //the curOverflow1 takes the current number of overflows
    curOverflow1 = overflow;

    //the curPeriod1 is calculated by the difference in overflows times the timer length
    //plus the difference in the edges
    curPeriod1 = ((curOverflow1 - prevOverflow1)*65535)+(curEdge1 - prevEdge1);

    //debouncing check: if the period is greater than 2 ms
    //Debouncing period / (PRE * 1/(frequency)) = 2 ms / (256*1/(16*10^6)) = 125
    if (curPeriod1 > 125) {

        prevEdge1 = curEdge1;           //the old curEdge1 becomes the new prevEdge1
        prevOverflow1 = curOverflow1;   //the old curOverflow1 becomes the new prevOverflow1

        buffer1[i % 4] = curPeriod1;    //saves the period to the circular buffer
                                        //i is the buffer counter, and since there are
                                        //four elements in the buffer, i is mod 4ed to
                                        //circulate through all the index positions
        i++;                            //buffer counter is incremented
        avePeriod1 = (buffer1[0] + buffer1[1] + buffer1[2] + buffer1[3]) / 4;
                                        //avePeriod1 is calculated as the average of the
                                        //last four periods
    }
}


//if button 2 has been pressed...
void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void) {
    // Process the captured values

    //set up variables
    static unsigned long int prevEdge2, curEdge2;
    static unsigned long int prevOverflow2, curOverflow2;
    static unsigned int j = 0;

    //reset IC2 flag
    _IC2IF = 0;

    //if button 2 was pressed, user 2 is not asleep,
    //so set his sleep flag to 0
    sleep2 = 0;
    //set timer 1 to zero to check for his sleep status over the next second
    TMR4 = 0;

    //while the FIFO buffer has data
    while (IC2CONbits.ICBNE == 1) {
        //grab most recent TMR1 value and store as curEdge1
        curEdge2 = IC2BUF;
    }

    //the curOverflow2 takes the current number of overflows
    curOverflow2 = overflow;

    //the curPeriod2 is calculated by the difference in overflows times the timer length
    //plus the difference in the edges
    curPeriod2 = ((curOverflow2 - prevOverflow2)*65535)+(curEdge2 - prevEdge2);

    //debouncing check: if the period is greater than 2 ms
    //Debouncing period / (PRE * 1/(frequency)) = 2 ms / (256*1/(16*10^6)) = 125
    if (curPeriod2 > 125) {

        prevEdge2 = curEdge2;           //the old curEdge2 becomes the new prevEdge2
        prevOverflow2 = curOverflow2;   //the old curOverflow2 becomes the new prevOverflow2

        buffer2[j % 4] = curPeriod2;    //saves the period to the circular buffer
                                        //j is the buffer counter, and since there are
                                        //four elements in the buffer, j is mod 4ed to
                                        //circulate through all the index positions
        j++;                            //buffer counter is incremented
        avePeriod2 = (buffer2[0] + buffer2[1] + buffer2[2] + buffer2[3]) / 4;
                                        //avePeriod2 is calculated as the average of the
                                        //last four periods
    }
}

//Associated with IC1 (push button 1). This will help us determine
//whether the first button has stopped being pressed and whether
//it has been winning for 5 seconds (can't win while sleeping)

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    _T1IF = 0;                  //reset timer 1 flag
    sleep1 = 1;                 //if user has reached this interrupt, it is because
                                //user 1 has not pressed his button for at least a
                                //second. Therefore, he is considered "asleep" and
                                //his sleep flag is set to 1

    buffer1[0] = 0xffffffff;    //while user 1 is sleeping, make his click rate as slow as possible
    buffer1[1] = 0xffffffff;
    buffer1[2] = 0xffffffff;
    buffer1[3] = 0xffffffff;

    avePeriod1 = 0xffffffff;
}


//Associated with IC2 (push button 2). This will help us determine
//whether the first button has stopped being pressed and whether
//it has been winning for 5 seconds (can't win while sleeping)

void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void) {
    _T4IF = 0;                  //reset timer 2 flag
    sleep2 = 1;                 //if user has reached this interrupt, it is because
                                //user 1 has not pressed his button for at least a
                                //second. Therefore, he is considered "asleep" and
                                //his sleep flag is set to 1

    buffer2[0] = 0xffffffff;    //while user 2 is sleeping, make his click rate as slow as possible
    buffer2[1] = 0xffffffff;
    buffer2[2] = 0xffffffff;
    buffer2[3] = 0xffffffff;

    avePeriod2 = 0xffffffff;
}
