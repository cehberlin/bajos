/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* ************************************************************************ *\

Copyright (c) 2006, Atmel Corporation All rights reserved. 

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the 
following disclaimer. 

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. The name of ATMEL may not be used to endorse or promote products 
derived from this software without specific prior written permission.  

THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS 
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
PARTICULAR PURPOSE ARE EXPRESSLY AND SPECIFICALLY 
DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY 
WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 

POSSIBILITY OF SUCH DAMAGE. 

\* ************************************************************************ */
// development board CharonII
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../definitions.h"
#include "../typedefinitions.h"
#include "../bajvm.h"
#include "lcd.h"
#include "platform.h"
#include "native.h"

#define		BYTES(word)		((word)*2)
#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)		STRING(a,b)
#define		LARGEBOOTSTART		0xf000
#define		BOARDRAMEND		0x8000
#define		MONSTART		LARGEBOOTSTART
#define 	MONRAM			CHARONRAMEND-0x100
// Monitor  functions (bamo128 -> cs.ba-berlin.de)
#define		mainLoop		BYTES(LARGEBOOTSTART+2)	// Ruecksprung in Monitor aus Programm mit goto
#define		saveCPU		BYTES(LARGEBOOTSTART+62)	//Time2Comp	// BOOTSTART+62		

FILE uartAVR8 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

SIGNAL(SIG_OUTPUT_COMPARE2) __attribute__ ((naked));
SIGNAL(SIG_OUTPUT_COMPARE2) {asm volatile  (INLINEASM(jmp,saveCPU));} // monitor für step-betrieb 

char (*conIn)()=(void*)(0xF004);

int __attribute__((weak)) _read (int file, char * ptr, int len){
int i;
//if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);
//if (file != 0)return unimplemented_syscall("_read with filedes != 0");
for ( i = 0; i < len; i++ ){
ptr[i] = (char)conIn();
}
return len;
}


void initHW(){
stdout = stdin=stderr = &uartAVR8;	
LCD_Init();
timer_Init();	// use timer 2 for real time clock
}

void VT102Attribute (u1 fgcolor, u1 bgcolor)	{
    printf("%c",0x1b);
      printf("%c",'[');
//            printf("%c",'4');
     printf("%c",fgcolor);
      //printf("%c",';');
      //printf("%c",40 + bgcolor);
      printf("%c",'m');
}

int uart_putchar(char c, FILE *stream){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	if (c=='\n') uart_putchar('\r',stream);
	return 0;			}

int uart_getchar(FILE *stream)	{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return (int)UDR0;	
}


void timer_Init()	{
OCR0=0x58;		// Timer zählt bis OCR0, dann Int und Reset Timer, Experimentell bestimmt
		TCCR0 = (1<<CS02) | (1<<CS00) | (1<<WGM01);	// Vorteiler 1024 zu Systemtakt 20 MHz??
		TIMSK |= (1<<OCIE0);
		sei();	// global interrupt zulassen
}


void exit(int n)	{
goto *0xf002;	//asm	 (INLINEASM(jmp,0xf002));
}


SIGNAL(SIG_OUTPUT_COMPARE0)		{
	timerMilliSec++;	
}
