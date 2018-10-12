/* 
 * File:   input_capture.h
 * Author: baska
 *
 * Created on March 13, 2018, 3:17 PM
 */

#ifndef INPUT_CAPTURE_H
#define	INPUT_CAPTURE_H

#ifdef	__cplusplus
extern "C" {
#endif
    // extern allows main to read these variables
    extern volatile unsigned long int overflow;
    extern volatile unsigned long int buffer1[4];
    extern volatile unsigned long int buffer2[4];
    extern volatile unsigned long int curPeriod1;
    extern volatile unsigned long int avePeriod1;
    extern volatile unsigned long int curPeriod2;
    extern volatile unsigned long int avePeriod2;
    extern volatile int sleep1;
    extern volatile int sleep2;
    
    // definitions of the input capture functions that are used in main
    void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
    void initTwoPushButtons(void);
    void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void);
    void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void);
    void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
    void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void);




#ifdef	__cplusplus
}
#endif

#endif	/* INPUT_CAPTURE_H */

