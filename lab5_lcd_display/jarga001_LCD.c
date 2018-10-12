/*
 * File:   jarga001_LCD.c
 * Author: baska
 *
 * Created on March 21, 2018, 7:04 PM
 */


#include "xc.h"
#include "jarga001_LCD.h"

#define CONTRAST 0b100000

void delay_xms(long x) { //approximate delay for x milliseconds
    long delay = 1600 * x;
    while (delay--);
}

void lcd_setContrast() {
    lcd_cmd(0b01110000 + (CONTRAST & (0b001111))); // contrast C3-C0
    lcd_cmd(0b01010100 + (CONTRAST >> 4)); // Ion, Bon, C5-C4
}

void lcd_cmd(char Package) {
    I2C2CONbits.SEN = 1; //Initiate Start condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b00000000; // 8-bits consisting of control byte
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package; // 8-bits consisting of the data byte
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2CONbits.PEN = 1; //send Stop condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

}

void lcd_init() {
    //AD1PCFG = 0x9fff; //set pins digital
    //TRISB &= 0b1111111111100111; //Makes RB2, RB3 outputs
    I2C2CON = 0;
    I2C2BRG = 0x009D; //Baud Rate: Fcy = 16 Mhz, Fscl = 100 kHz
    I2C2CONbits.I2CEN = 1; //enable I2C protocol

    IFS3bits.MI2C2IF = 0; //for safety's sake, make sure flag starts at 0

    delay_xms(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_setContrast();
    //lcd_cmd(0b01111111); // contrast C3-C0
    //lcd_cmd(0b01010100); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    delay_xms(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay_xms(2);

}

void lcd_printChar(char Package) {
    I2C2CONbits.SEN = 1; //Initiate Start condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit, which is set to "write"
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1, as to prep for writing a data byte next
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = Package; // 8-bits consisting of the data byte
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2CONbits.PEN = 1; //send Stop Condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

}

void lcd_setCursor(char x, char y) { //set cursor to corresponding row and column
    char input = 0;
    //the seventh bit is the row, the first three bits are the column
    input = 0b10000000 + x * 0b01000000 + y;
    lcd_cmd(input);
}

void lcd_printString(const char *s) {
    //char array[] =

    I2C2CONbits.SEN = 1; //Initiate Start condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit to 'write'
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w C=0 to designate we are only sending data bytes from now on
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

    while (*s != 0) { //while the string still has characters to print
        I2C2TRN = *s; //send the data byte of the character to the LCD
        while (IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        s++; //point to next character
    }

    I2C2CONbits.PEN = 1; //stop condition
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;

}

int lcd_getStringLength(const char *s) { //get string and returns string length
    int i = 0; // counter for string length
    while (*s != 0) { // look for terminating zero, which is end of the string.
        i++; // increments the string counter
        s++; // points to next character
    }
    return i; // returns the value of string counter
}
