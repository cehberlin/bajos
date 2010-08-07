/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-function ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/

#include <stdio.h>
#include <stdlib.h>
#include "../definitions.h"
#include "../typedefinitions.h"
#include "../bajvm.h"
#include "../classfile.h"
#include "../stack.h"
#include "../bajvm.h"
#include "../scheduler.h"
#include "../heap.h"
#include "lock.h"
#include "interruptthread.h"
#ifdef AVR8
	#include <avr/interrupt.h>  
#endif

char static_sei(){
	#ifdef AVR8
	sei();
	#endif
	return 0;
}

char static_cli(){
	#ifdef AVR8
	cli();
	#endif
	return 0; 
}
