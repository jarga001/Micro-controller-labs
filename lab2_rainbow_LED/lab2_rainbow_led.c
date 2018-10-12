/*
 * File:   jarga001_2b_main_v001.c
 * Author: baska
 *
 * Created on February 7, 2018, 11:28 AM
 */

#include "xc.h"
#include "jar_lab2b_asmLib_v001.h"
#include "stdint.h"
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
#define PERIOD 5

void setup(void) {
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0; //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;
    TRISA = 0b1111111111111110;
    //TRISB = 0b0000000000000011;
    LATA = 0x0000;
    //LATB = 0xffff;

}

void writeColor(int r, int g, int b) {
    int red;
    int green;
    int blue;
    int ri, gi, bi;
    int mask = 128;

    for (ri = 0; ri < 8; ri++) {
        red = r&mask;
        if (red == mask) {
            write_1();
        } else {
            write_0();
        }
        r = r << 1;
    }

    for (gi = 0; gi < 8; gi++) {
        green = g&mask;
        if (green == mask) {
            write_1();
        } else {
            write_0();
        }
        g = g << 1;
    }

    for (bi = 0; bi < 8; bi++) {
        blue = b&mask;
        if (blue == mask) {
            write_1();
        } else {
            write_0();
        }
        b = b << 1;
    }
    jar_wait_50microSec();
}

void drawFrame(unsigned char frame) {
    writeColor(frame, 0, 255 - frame);
}

void delay(int delay_in_ms) {
    int i;
    for (i = 0; i < delay_in_ms; i++) {
        wait1_msec();
    }
}

void cycler() {
    int i;
    for (i = 0; i < 256; i++) {
        drawFrame(i);
        wait1_msec();
    }
    for (i = 256; i > 0; i--) {
        drawFrame(i);
        wait1_msec();
    }
    delay(300);
}

uint32_t packColor(unsigned char Rd, unsigned char Grn, unsigned char Blu) {
    unsigned long int RGBval = 0;
    RGBval = ((long) Rd << 16) | ((long) Grn << 8) | ((long) Blu);
    return RGBval;
}

unsigned char getR(uint32_t val) {
    return (val >> 16);
}

unsigned char getG(uint32_t val) {
    return (val >> 8);
}

unsigned char getB(uint32_t val) {
    return (val >> 0);
}

void writePacCol(uint32_t PackedColor) {
    unsigned char red = getR(PackedColor);
    unsigned char green = getG(PackedColor);
    unsigned char blue = getB(PackedColor);
    writeColor(red, green, blue);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r. 

uint32_t Wheel(unsigned char WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return packColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return packColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return packColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

int main(void) {
    setup();


    while (1) {
        // Execute repeatedly forever and ever and ever and ever ...

        int i;
        for (i = 0; i < 256; i++) {
            writePacCol(Wheel(i));
            delay(PERIOD);
        }
        for (i = 256; i > 0; i--) {
            writePacCol(Wheel(i));
            delay(PERIOD);
        }
    }
    return 0;
}
//
write_0();
write_1();
