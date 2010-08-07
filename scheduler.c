/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#include<stdio.h>
#include "definitions.h"
#include "bajvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"

void	createThread(void)			{
if (numThreads == MAXTHREADS){
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	errorExit(-2,PSTR("zu viele Threads\n"));
#else
	errorExit(-2,"zu viele Threads\n");
#endif
} 
ThreadControlBlock* t=(ThreadControlBlock*) malloc(sizeof(ThreadControlBlock));
opStackInit(&(t->opStackBase));
methodStackInit(&(t->methodStackBase));
t->methodSpPos=(numThreads==0)?0:5;
t->tid=tid++;
t->priority=(actualThreadCB==NULL) ?NORMPRIORITY:actualThreadCB->priority;
t->numTicks=t->priority+1;//+1 because 0 ticks are stupid
t->state=THREADNOTBLOCKED;
t->pred=(actualThreadCB==NULL)?t:actualThreadCB;
t->succ=(actualThreadCB==NULL)?t:actualThreadCB->succ;
t->pred->succ=t;
t->succ->pred=t;
if (actualThreadCB==NULL) actualThreadCB=t;

if (numThreads!=0)	{
	findMethodByName("run",3,"()V",3);
	*(t->methodStackBase+0)=(u2)0;
	*(t->methodStackBase+1)=cN;
	*(t->methodStackBase+2)=mN;
	*(t->methodStackBase+3)=getStartPC();
	*(t->methodStackBase+4)=findMaxLocals();
	*(t->opStackBase)=opStackGetValue(local); /* reference to caller object (from start())*/
}
u2 i;
for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	{
	t->hasMutexLockForObject[i]=NULLOBJECT;
	t->lockCount[i]=0; 		}
t->isMutexBlockedOrWaitingForObject=NULLOBJECT;
numThreads++;					}

void	deleteThread(void)	{
ThreadControlBlock* t=actualThreadCB->succ;
t->pred=actualThreadCB->pred;
t->pred->succ=t;
/*free(actualThreadCB->methodStackBase); // crash -> to do*/
/*free(actualThreadCB->opStackBase);*/
/*free(actualThreadCB);*/
actualThreadCB =t;
methodStackBase=actualThreadCB->methodStackBase;
methodStackSetSpPos(actualThreadCB->methodSpPos);
opStackBase=actualThreadCB->opStackBase;
opStackSetSpPos(methodStackPop());
pc=methodStackPop();
mN=methodStackPop();
cN=methodStackPop();
local=methodStackPop();
numThreads--;			}

void scheduler(void)	{
if (numThreads==1) return;
//A Thread runs until his numTicks is 0
if((actualThreadCB->numTicks && ((actualThreadCB->state)==THREADNOTBLOCKED)))
{
  actualThreadCB->numTicks--;
  return;
}
/* scheduling -> next thread*/
methodStackPush(local);
methodStackPush(cN);
methodStackPush(mN);
methodStackPush(pc);
methodStackPush((u2)(opStackGetSpPos()));
actualThreadCB->methodSpPos=methodStackGetSpPos();
do 	{
	actualThreadCB=actualThreadCB->succ;
	if ((actualThreadCB->state)==THREADNOTBLOCKED) break;
	if ((actualThreadCB->state)==THREADMUTEXBLOCKED) continue;
	if ((actualThreadCB->state)==THREADWAITBLOCKED) continue;
	if (((actualThreadCB->state)==THREADWAITAWAKENED) && 
	((HEAPOBJECTMARKER((actualThreadCB->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex)
		==MUTEXBLOCKED))
	continue;
	/* awakened and mutexnotblocked*/
	//if ((actualThreadCB->state)==THREADWAITAWAKENED)	{ //not nesessary because no other state ist possible
	HEAPOBJECTMARKER((actualThreadCB->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex=MUTEXBLOCKED;
	actualThreadCB->state=THREADNOTBLOCKED;
	actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
	break;							
	//}
	} while (1);
//reset numTicks
actualThreadCB->numTicks= actualThreadCB->priority+1;
methodStackBase=actualThreadCB->methodStackBase;
methodStackSetSpPos(actualThreadCB->methodSpPos);
opStackBase=actualThreadCB->opStackBase;
opStackSetSpPos(methodStackPop());
pc=methodStackPop();
mN=methodStackPop();
cN=methodStackPop();
local=methodStackPop();
}
