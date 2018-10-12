/*
 * File:   keypad.c
 * Author: baska
 *
 * Created on February 21, 2018, 8:05 AM
 */


#include "xc.h"
#include "lab3_keypad.h"

void delay_2ms()
{
    int delay=1600*2;
    while(delay--);
}

char check_row1()
{
    LATBbits.LATB12=0;
    LATBbits.LATB13=1;
    LATBbits.LATB14=1;
    LATBbits.LATB15=1;
    
    delay_2ms();
    
    
    if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==0)
    {
        return '1';
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==0 && PORTAbits.RA3==1)
    {
        return '2'; 
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==0 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return '3'; 
    }
    else if(PORTAbits.RA0==0 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return 'a'; 
    }
    else {
        return '!';
    }
} 

char check_row2(){
    LATBbits.LATB12=1;
    LATBbits.LATB13=0;
    LATBbits.LATB14=1;
    LATBbits.LATB15=1;
    
    delay_2ms();
    
    
    if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==0)
    {
        return '4';
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==0 && PORTAbits.RA3==1)
    {
        return '5'; 
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==0 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return '6'; 
    }
    else if(PORTAbits.RA0==0 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return 'b'; 
    }
    else {
        return '!';
    }
}

char check_row3(){
    LATBbits.LATB12=1;
    LATBbits.LATB13=1;
    LATBbits.LATB14=0;
    LATBbits.LATB15=1;
    
    delay_2ms();
    
    
    if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==0)
    {
        return '7';
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==0 && PORTAbits.RA3==1)
    {
        return '8'; 
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==0 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return '9'; 
    }
    else if(PORTAbits.RA0==0 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return 'c'; 
    }
    else {
        return '!';
    }
}

char check_row4(){
    LATBbits.LATB12=1;
    LATBbits.LATB13=1;
    LATBbits.LATB14=1;
    LATBbits.LATB15=0;  
    
    delay_2ms();
    
    
    if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==0)
    {
        return '*';
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==1 && PORTAbits.RA2==0 && PORTAbits.RA3==1)
    {
        return '0'; 
    }
    else if(PORTAbits.RA0==1 && PORTAbits.RA1==0 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return '#'; 
    }
    else if(PORTAbits.RA0==0 && PORTAbits.RA1==1 && PORTAbits.RA2==1 && PORTAbits.RA3==1)
    {
        return 'd'; 
    }
    else {
        return '!';
    }
}

char readKeyPadRAW(){
 
 char a,b,c,d;
 a=check_row1();
 b=check_row2();
 c=check_row3();
 d=check_row4();
 
if(a!='!'){
    return a;
}

 if(b!='!'){
    return b;
}
 
 if(c!='!'){
    return c;
}
 
 if(d!='!'){
    return d;
}
 return '!';
}

void init_keypad(void){
    TRISA = 0b0000000000001111;
    LATA = 0xffff;
    CNPU1bits.CN2PUE=1;
    CNPU1bits.CN3PUE=1;
    CNPU2bits.CN29PUE=1;
    CNPU2bits.CN30PUE=1;
}
