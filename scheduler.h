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

//This makro initalizes a interrupt Thread. The only thing you need is a global ThreadControlBlock* 
//the threadobject must be the first value
#define INIT_INTERRUPT_THREAD(thread) \
	{ \
  	(thread)=findThreadCB((slot)opStackGetValue(local+1)); \
 	}
//This makro could be used in every C-ISR. The only thing you need is a global ThreadControlBlock* with initialized thread --> look for INIT_INTERRUPT_THREAD
#define INTERRUPT_THREAD(thread) \
	{ \
	if((thread)){ \
		if(actualThreadCB!=(thread)){ \
			actualThreadCB->numTicks=0; \
		} \
		(thread)->isMutexBlockedOrWaitingForObject=NULLOBJECT; \
		(thread)->lockCount[0]=1; \
		(thread)->hasMutexLockForObject[0]=(thread)->obj; \
		(thread)->state=THREADNOTBLOCKED; \
		} \
	}

ThreadControlBlock* findThreadCB(slot obj);
void deleteNotCurrentThread(ThreadControlBlock** t);
void notifyThread(slot obj);
void awakeThreadFromMutex(slot obj);
void setMutexOnObject(ThreadControlBlock* t,slot obj);
void releaseMutexOnObject(ThreadControlBlock* t,slot obj,ThreadControlBlock* awakeThread);
void createThread(void);
void deleteThread(void);
void scheduler(void);
void insertThreadIntoPriorityList(ThreadControlBlock* t);
void removeThreadFromPriorityList(ThreadControlBlock* t);
#endif
