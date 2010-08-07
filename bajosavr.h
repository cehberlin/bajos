/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// eine jvm auch für 8 Bit MC !!, z.B. CharonII
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#ifndef __BAJOSAVR__
#define __BAJOSAVR__
#include <stdlib.h>
#include <stdlib.h>

#include "typedefinitions.h"
#include "limits.h"
#include "definitions.h"

	void initVM(int argc, char* argv[]);
	void initHW();
	void executeClInits();
	void VT102Attribute (u1 fgcolor, u1 bgcolor)	;
	#ifndef UNIX 

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

#ifdef __DEF_GLOBALS__
		#define GLOBAL
		#define INIT__(_a) =(_a)
#else
		#define GLOBAL extern
		#define INIT__(_a) 
#endif
#undef __DEF_GLOBALS__
GLOBAL u2		pc;							// one thread
GLOBAL u1 		cN;						// class structure Number
GLOBAL u1 		mN;						// method Number in class structure
GLOBAL u1 		local 			INIT__(0);
GLOBAL slot*	opStackBase INIT__(NULL);
GLOBAL u2* 	methodStackBase INIT__(NULL);
GLOBAL slot*	heapBase 	INIT__(NULL);
GLOBAL u2		heapPos INIT__(0);
GLOBAL u2		heapTop INIT__(MAXHEAP);
GLOBAL u1*		classFileBase INIT__(NULL);
GLOBAL u1		numClasses	INIT__(0);
GLOBAL 			ThreadControlBlock*  actualThreadCB	INIT__(NULL);
GLOBAL u1		numThreads	INIT__(0);
GLOBAL u1		tid INIT__(0);
GLOBAL classStructure cs[MAXCLASSES];				// class structure decl und def Verschwendung!!
GLOBAL char* 	nry	INIT__(" ->  not realized yet\n");
#ifndef UNIX
GLOBAL 	 FILE uart_str;
#endif
#endif
