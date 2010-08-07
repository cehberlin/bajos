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


char initInterrupt(){

  u1 vectorNr=opStackGetValue(local+1).UInt;

  printf("nr:%d",vectorNr);

  //interruptNr not valid
  if(vectorNr>=NUMBEROFINTERRUPTS) return 0;

  interruptVectors[vectorNr]=findThreadCB((slot)opStackGetValue(local));

  #ifdef AM //for test purpose

  cli();

  TCCR0A = 0; //normal increment with overflow

  TCCR0B |= (1<<CS02) | (1<<CS00); // Prescaler 1024

  // enable Overflow Interrupt 
  TIMSK0 |= (1<<TOIE0);

  DDRB |= 0x80;
 
  //enable Global Interrupts
  sei();

  #endif
 
  return 0;
}

char removeInterrupt(){
   u1 vectorNr=opStackGetValue(local).UInt;
   //interruptNr not valid
   if(vectorNr>=NUMBEROFINTERRUPTS) return 0;
   deleteNotCurrentThread(&(interruptVectors[vectorNr]));
   return 0;
}

char forceInterrupt(){
	u1 vectorNr=opStackGetValue(local).UInt;
  	 //interruptNr not valid
   	if(vectorNr>=NUMBEROFINTERRUPTS) return 0;
	interruptThread(interruptVectors[vectorNr]);
	return 0;
}
