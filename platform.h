#ifndef __PLATFORM__
#define __PLATFORM__
#include <stdio.h>
#include <stdlib.h>
#include "typedefinitions.h"
#include "definitions.h"
#include "bajosavr.h"
	void initHW();

	void VT102Attribute (u1 fgcolor, u1 bgcolor)	;
	#ifdef AVR8
//u2 loadInSram1(unsigned char* addr);
u2 loadInSram(unsigned char* addr);
	/* CPU frequency */
	//#define F_CPU 1000000UL
	#define F_CPU 14745000UL
	/* UART baud rate */
	//#define UART_BAUD  9600
	#define UART_BAUD  115200
	void exit(int n);
	void uart_init(void);	
	int uart_putchar(char c, FILE *stream);
	int uart_getchar(FILE *stream);
	#endif
#ifdef AVR8
void timer_Init();
#endif
#ifdef AVR32UC3A
void delay_ms(u4 delayMilliSec);
void initTimer();
#endif
#endif
