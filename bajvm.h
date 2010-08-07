/*
* BAJOS BA-JAVA OPERATING SYSTEM
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// a jvm for 8 Bit MC !!, e.g. atmega128 CharonII and linux and avr32
// version 0.1 vom 1.10.07

#ifndef __BAJOSAVR__
#define __BAJOSAVR__
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "typedefinitions.h"
#include "limits.h"
#include "definitions.h"
#include "scheduler.h"

void errorExit(char nr,const char *format, ...);

#ifdef __DEF_GLOBALS__
		#define GLOBAL
		#define INIT__(_a) =(_a)
#else
		#define GLOBAL extern
		#define INIT__(_a) 
#endif
#undef __DEF_GLOBALS__
GLOBAL u2	pc;			// one thread
GLOBAL u1 	cN;			// class structure Number
GLOBAL u1 	mN;			// method Number in class structure
GLOBAL u1	fNC;			// field Number in class structure
GLOBAL u1	fNO;			// fieldNumber in object
GLOBAL u1 	local 		INIT__(0);
GLOBAL slot*	opStackBase	INIT__(NULL);
GLOBAL u2* 	methodStackBase INIT__(NULL);
GLOBAL slot*	heapBase 	INIT__(NULL);
GLOBAL u2	heapTop		INIT__(MAXHEAP);
#ifndef AVR8
GLOBAL char*	classFileBase	INIT__(NULL);
#endif
#if (AVR32UC3A|| AVR32AP7000)
GLOBAL char*	appClassFileBase INIT__(NULL);
#endif
GLOBAL u1	numClasses	INIT__(0);
GLOBAL 		ThreadControlBlock*  actualThreadCB	INIT__(NULL);
GLOBAL u1	numThreads	INIT__(0);
GLOBAL u1	tid 		INIT__(0);
GLOBAL ThreadPriorityList  threadPriorities[MAXPRIORITY]; //array of priority lists
GLOBAL classStructure cs[MAXCLASSES];
#ifndef AVR8
GLOBAL u4	timerMilliSec 	INIT__(0);
#else
#ifndef WITHMON
GLOBAL u4	timerMilliSec 	INIT__(0);
#endif
#endif

GLOBAL u1	timeOut 	INIT__(0);
//GLOBAL u2	endBajos	INIT__(0);
#ifdef AVR8
GLOBAL FILE 	uartAVR8;
#endif
#endif
