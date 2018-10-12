.include "xc.inc"
.text ;BP (put the following data in ROM(program memory))
; This is a library, thus it can *not* contain a _main function: the C file will
; deine main(). However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading
; underscore (_) and be included in a comment delimited list below.

.global _write_0, _write_1, _jar_wait_50microSec, _wait1_msec
    
_write_0:
    inc LATA 
    repeat #4 ;4
    nop
    
    clr LATA
    repeat #7 ;7
    nop
    return 

_write_1:
    inc LATA
    repeat #8
    nop
    
    clr LATA
    repeat #2
    nop
    return 

_jar_wait_50microSec:
    repeat #793
    nop
    return
    
_wait1_msec:                    
    repeat  #15993
    nop
    return
