/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/

/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef AVR8 
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>  
#endif
#include "../definitions.h"
#include "../typedefinitions.h"
#include "../bajvm.h"
#include "../stack.h"
#include "../classfile.h"
#include "../scheduler.h"
#include "../heap.h"

#include "native.h"
#include "platform.h"

#ifdef WITHMON
#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
extern char (*conIn)();
#endif 

char	nativeCharIn()	{
#ifdef WITHMON
opStackPush((slot)(u4)conIn());
#else
opStackPush((slot)(u4)uart_getchar(&uartAVR8));
#endif
return 1;		}

char nativeCharOut()		{
char val=opStackGetValue(local+1).UInt;

	printf("%c",val);

	return 0;		}


char nativeExit()	{
goto *0xf002;	/*asm	 (INLINEASM(jmp,0xf002));*/
}

char currentTimeMillis()	{
#ifdef WITHMON
	opStackPush((slot)(u4)((*(u4*)SYSTIMEMILLISEC)&0x7ffff));
#else
	opStackPush((slot) (u4) timerMilliSec);
#endif
	return 1;
}

/* added 2009 by: Friedrich Große & Rainer Kirchhoff, FHW-BA Berlin*/
/* Deutsche Post IT-Services GmbH & MSA Auer GmbH*/
char nativeConStat() {
	opStackPush((slot) (u4)  ((UCSR0A>>RXC0)&0x01));	
	return 1;
}

char nativeSetData() { //sdram
	/*Get possible 16-Bit adr.*/
	u2 portAddr = opStackGetValue(local+1).bytes[0];
	portAddr |= (opStackGetValue(local+1).bytes[1]<<8);
	/*Get value*/
	u2 portValue = opStackGetValue(local+2).bytes[0];
	/*Assign Value to Port (Adr)*/
	*((u2*)portAddr) = portValue;
	return 0;
}

char nativeGetData() { //sdarm
	/*Get possible 16-Bit adr.*/
	u2 portAddr = opStackGetValue(local+1).bytes[0];
	portAddr |= opStackGetValue(local+1).bytes[1]<<8;
	opStackPush((slot) (u4) (char) *((u2*)portAddr));
	return 1;
}

ThreadControlBlock* timer8bitThread=NULL;

//overflow
char initTimer8bit(){

  cli();

  slot threadObj = opStackGetValue(local+1);

  u2 prescaler = opStackGetValue(local+2).UInt;

  // Timer 0 konfigurieren
  switch(prescaler){
  case 1:
	printf("prescaler 1");
  	TCCR0A = (1<<CS00); // Prescaler 1
  	break;
  case 8:
	printf("prescaler 8");
  	TCCR0A = (1<<CS01); // Prescaler 8
  	break;
  case 64:
	printf("prescaler 64");
  	TCCR0A = (1<<CS01) | (1<<CS00); // Prescaler 64
  	break;
  case 256:
	printf("prescaler 256");
  	TCCR0A = (1<<CS02); // Prescaler 256
  	break;
  case 1024:
	printf("prescaler 1024");
  	TCCR0A = (1<<CS02)| (1<<CS00); // Prescaler 1024
  	break;
  default:
  	TCCR0A = 0; // Prescaler 0 = STOPP	
  }

  timer8bitThread=findThreadCB(threadObj);
 
  // Overflow Interrupt erlauben
  TIMSK0 |= (1<<TOIE0);
 
  // Global Interrupts aktivieren
  sei();

  return 0;
}


/*
Der Overflow Interrupt Handler
wird aufgerufen, wenn TCNT0A von
255 auf 0 wechselt (256 Schritte),
*/
ISR (TIMER0_OVF_vect)
{
	printf("foo\n");
	//yet not tested!!
	if(timer8bitThread){
		releaseMutexOnObject(timer8bitThread,timer8bitThread->obj,timer8bitThread);
	}
}
