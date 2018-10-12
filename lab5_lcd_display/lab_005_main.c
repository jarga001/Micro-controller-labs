/*
 * File:   lab_005_main.c
 * Author: baska
 *
 * Created on March 20, 2018, 3:13 PM
 */


#include "xc.h"
#include "jarga001_LCD.h"
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

void setup() {
    CLKDIVbits.RCDIV = 0;
    lcd_init();
}

int main(void) {
    setup(); // here we are configuring a setups for pic microcontroller's pin arrangement
             // also the function is implicitly calling a lcd_init() function, which initializes setup for the lcd
             // such as setting a baud rate, set contrast, setting function modes, and etc.

    const char s[] = "hh"; // here we are creating a character array which holds string value
    int j;          // variable that keeps track of the number of shifts in the aniimation
    int i = 1;      // counter variable for the pointer

    for (j = 0; j < lcd_getStringLength(s); j++) {
        delay_xms(250);      // animation delay
        lcd_cmd(0b0000001);  // this is the command which clears the display.
                             // we have this command to make sure we are clearing everything that was before on lcd.
        delay_xms(2); // this is necessary delay for lcd between commands.
        if (j < 8) {  // there are 8 columns which represents one single letter in every row.
                      // when counter is 7 or less,
            lcd_setCursor(0, 7 - j); // set the cursor one column left of the last cursor position
                                     // we are kepp updating the postion of the cursor and printing the letters (from right to left)
            lcd_printString(s); // printing the string
        } else {    // once the the first row is filled with letters / the cursor position reaches beggining of the first row.
            lcd_setCursor(0, 0);    // cursor should stay in the beggining position.
            lcd_printString(s + i); // begin truncating the string being printed, such that another
                                    //leading letter is cut off each animation cycle
            i++; // incrementing the pointer counter
        }
    }
    lcd_setCursor(1,1);
    lcd_printString("gg");
    
    while (1);
    return 0;
}
