/* 
 * File:   lab3_keypad.h
 * Author: baska
 *
 * Created on February 21, 2018, 9:42 AM
 */

#ifndef LAB3_KEYPAD_H
#define	LAB3_KEYPAD_H

#ifdef	__cplusplus
extern "C" {
#endif
void delay_2ms();
char check_row1();
char check_row2();
char check_row3();
char check_row4();
char readKeyPadRAW();
void init_keypad();

#ifdef	__cplusplus
}
#endif

#endif	/* LAB3_KEYPAD_H */

