/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// scheduler invokes after completation of a byte code instruction 
#include "definitions.h"
#ifndef __SCHEDULER__
#define __SCHEDULER__
#define	DEFAULTTICKS 5
// number of byte code instructions, counter
#define NORMPRIORITY 5
#define MAXPRIORITY 10
#define MINPRIORITY 1 

void notifyThread(slot obj);
void awakeThreadFromMutex(slot obj);
void setMutexOnObject(ThreadControlBlock* t,slot obj);
void releaseMutexOnObject(ThreadControlBlock* t,slot obj);
void createThread(void);
void deleteThread(void);
void scheduler(void);
void insertThreadIntoPriorityList(ThreadControlBlock* t);
void removeThreadFromPriorityList(ThreadControlBlock* t);
#endif
