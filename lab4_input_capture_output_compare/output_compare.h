/* 
 * File:   output_compare.h
 * Author: baska
 *
 * Created on March 13, 2018, 3:18 PM
 */

#ifndef OUTPUT_COMPARE_H
#define	OUTPUT_COMPARE_H

#ifdef	__cplusplus
extern "C" {
#endif
    // extern allows main to read this variable
    extern volatile int count5;
    
    // definitions of the output compare functions that are used in main
    void initServo(void);
    void setServo(int Val);
    void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void);



#ifdef	__cplusplus
}
#endif

#endif	/* OUTPUT_COMPARE_H */

