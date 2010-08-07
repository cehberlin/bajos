/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// scheduler invokes after completation of a byte code instruction 
#include "definitions.h"
#ifndef __SCHEDULER__
#define __SCHEDULER__
#define	DEFAULTTICKS 5
// number of byte code instructions, counter

#define NORMPRIORITY 5



void	createThread(void);
void	deleteThread(void);
void scheduler(void);
#endif
