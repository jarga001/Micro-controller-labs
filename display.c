/*
 * File:   jarga001_lab3_myLib.c
 * Author: baska
 *
 * Created on February 14, 2018, 5:26 PM
 */



#include "xc.h"
#include "lab3_display.h"

void write_0()
{
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    
    return; 
}

void write_1()
{
    LATBbits.LATB7=0;
    LATBbits.LATB8=0;
    return;
}

void write_2()
{
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB3=0;
    LATBbits.LATB5=0;
    LATBbits.LATB6=0;
    return;
}

void write_3()
{
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB3=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    return;
}

void write_4()
{
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    return;
}

void write_5()
{
    LATBbits.LATB9=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    return;
}

void write_6()
{
    LATBbits.LATB9=0;
    LATBbits.LATB3=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    return;
}

void write_7()
{
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    return;
}

void write_8()
{
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    return;
}

void write_9()
{
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    return;
}

void write_a(){
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    return;
}

void write_b(){
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    return;
}

void write_c(){
    LATBbits.LATB9=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    return;
}

void write_d(){
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB3=0;
    return;
}

void write_e(){
    LATBbits.LATB9=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    return;
}

void write_f(){
    LATBbits.LATB9=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
    return;
}
void write_pound(){
   
    LATBbits.LATB8=0;
    LATBbits.LATB7=0;
    LATBbits.LATB5=0;
    LATBbits.LATB4=0;
    LATBbits.LATB3=0;
}
void write_star(){
    LATBbits.LATB9=0;
    LATBbits.LATB8=0;
    LATBbits.LATB6=0;
    LATBbits.LATB5=0;
}
void write_no(){
    LATBbits.LATB9=1;
    LATBbits.LATB8=1;
    LATBbits.LATB7=1;
    LATBbits.LATB6=1;
    LATBbits.LATB5=1;
    LATBbits.LATB4=1;
    LATBbits.LATB3=1;
    return;
}

void init_7_seg(void){
    TRISB = 0b0000000000000011;
    LATB = 0b0000001111111100;
}

void showChar7seg(char myChar, enum Digit myDigit){
    if(myDigit==MSB)
    {
        LATB = 0b0000011111111100;
        
        switch(myChar)
        {
            case '0':
                write_0();
                return;
            case '1':
                write_1();
                return;
            case '2':
                write_2();
                return;  
            case '3':
                write_3();
                return;
            case '4':
                write_4();
                return;
            case '5':
                write_5();
                return;
            case '6':
                write_6();
                return;
            case '7':
                write_7();
                return;
            case '8':
                write_8();
                return;
            case '9':
                write_9();
                return;
            case 'a':
                write_a();
                return;
            case 'b':
                write_b();
                return;
            case 'c':
                write_c();
                return;
            case 'd':
                write_d();
                return;
            case 'e':
                write_e();
                return;
            case 'f':
                write_f();
                return;
            case '*':
                write_star();
                return;
            case '#':
                write_pound();
                return;
            case '!':
                write_no();
                return;
        }
    }
        if(myDigit==LSB)
        {
            LATB = 0b0000101111111100;
            //LATBbits.LATB11=1;
            //LATBbits.LATB10=0;
    
            switch(myChar)
            {
            case '0':
                write_0();
                return;
            case '1':
                write_1();
                return;
            case '2':
                write_2();
                return;  
            case '3':
                write_3();
                return;
            case '4':
                write_4();
                return;
            case '5':
                write_5();
                return;
            case '6':
                write_6();
                return;
            case '7':
                write_7();
                return;
            case '8':
                write_8();
                return;
            case '9':
                write_9();
                return;
            case 'a':
                write_a();
                return;
            case 'b':
                write_b();
                return;
            case 'c':
                write_c();
                return;
            case 'd':
                write_d();
                return;
            case 'e':
                write_e();
                return;
            case 'f':
                write_f();
                return;
            case '*':
                write_star();
                return;
            case '#':
                write_pound();
                return;
            case '!':
                write_no();
                return;
            }
        } 
}
