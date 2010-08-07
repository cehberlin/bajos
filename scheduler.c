/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#include<stdio.h>
#include "definitions.h"
#include "bajosavr.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"

void	createThread(void)	{
if (numThreads == MAXTHREADS) {printf("zu viele Threads\n");exit(-1);}
ThreadControlBlock* t=(ThreadControlBlock*) malloc(sizeof(ThreadControlBlock));
opStackInit(&(t->opStackBase));
methodStackInit(&(t->methodStackBase));
t->methodSpPos=(numThreads==0)?0:5;
t->tid=tid++;
t->priority=(actualThreadCB==NULL) ?NORMPRIORITY:actualThreadCB->priority;
t->numTicks=0;
t->state=THREADNOTBLOCKED;
t->pred=(actualThreadCB==NULL)?t:actualThreadCB;
t->succ=(actualThreadCB==NULL)?t:actualThreadCB->succ;
t->pred->succ=t;
t->succ->pred=t;
if (actualThreadCB/*!*/==NULL) /*actualThreadCB->succ=t; else*/  actualThreadCB=t;

if (numThreads!=0)	{
findMethodByName((u1*)"run",3,(u1*)"()V",3);
*(t->methodStackBase+0)=(u2)0;
*(t->methodStackBase+1)=cN;
*(t->methodStackBase+2)=mN;
*(t->methodStackBase+3)=getStartPC();
*(t->methodStackBase+4)=findMaxLocals();
*(t->opStackBase)=opStackGetValue(local); // reference to caller object (from start())
}
u2 i;
for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	{
t->hasMutexLockForObject[i]=NULLOBJECT;
t->lockCount[i]=0; 											// lockCount
}
t->isMutexBlockedOrWaitingForObject=NULLOBJECT;
numThreads++;
}

void	deleteThread(void)	{
ThreadControlBlock* t=actualThreadCB->succ;
t->pred=actualThreadCB->pred;
t->pred->succ=t;
//free(actualThreadCB->methodStackBase); // Absturz
//free(actualThreadCB->opStackBase);
//free(actualThreadCB);
actualThreadCB =t;
methodStackBase=actualThreadCB->methodStackBase;
methodStackSetSpPos(actualThreadCB->methodSpPos);
opStackBase=actualThreadCB->opStackBase;
//currThread=actualThreadCB->tid;
opStackSetSpPos(methodStackPop());
pc=methodStackPop();
mN=methodStackPop();
cN=methodStackPop();
local=methodStackPop();
numThreads--;
}

void scheduler(void)	{
if ((numThreads==1)/*&&((actualThreadCB->state)==THREADNOTBLOCKED)*/) return;
// scheduling -> next thread
// gegenwärtig thread wechsel nach jedem jvm befehl
methodStackPush(local);
methodStackPush(cN);
methodStackPush(mN);
methodStackPush(pc);
methodStackPush((u2)(opStackGetSpPos()));
actualThreadCB->methodSpPos=methodStackGetSpPos();//methodSp;
//printf("th %d %d %d %d\n",numThreads,actualThreadCB->state,actualThreadCB->succ->state,THREADMUTEXBLOCKED);
do {
actualThreadCB=actualThreadCB->succ;
//printf(":::: %d    stat %d\n",actualThreadCB->tid,actualThreadCB->state);
if ((actualThreadCB->state)==THREADNOTBLOCKED) break;
//if ((actualThreadCB->state)==THREADMUTEXBLOCKED) ;
if ((actualThreadCB->state)==THREADMUTEXBLOCKED) continue;
if ((actualThreadCB->state)==THREADWAITBLOCKED) continue;
if (((actualThreadCB->state)==THREADWAITAWAKENED) && 
	((HEAPOBJECTMARKER((actualThreadCB->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex)
		==MUTEXBLOCKED))
	continue;
// awakened and mutexnotblocked
if ((actualThreadCB->state)==THREADWAITAWAKENED)	{
HEAPOBJECTMARKER((actualThreadCB->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex=MUTEXBLOCKED;
actualThreadCB->state=THREADNOTBLOCKED;
/*	for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
				if (actualThreadCB->hasMutexLockForObject[i].UInt!=NULLOBJECT.UInt)continue;else break;
				if (i==MAXLOCKEDTHREADOBJECTS) {printf("too many locks\n"); exit(251);	}
				actualThreadCB->lockCount[i]+=1;	// count
				actualThreadCB->hasMutexLockForObject[i]=(actualThreadCB->isMutexBlockedOrWaitingForObject);
				actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
*/
actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
break;
}

} while (1);

methodStackBase=actualThreadCB->methodStackBase;
methodStackSetSpPos(actualThreadCB->methodSpPos);
opStackBase=actualThreadCB->opStackBase;
opStackSetSpPos(methodStackPop());
pc=methodStackPop();
mN=methodStackPop();
cN=methodStackPop();
local=methodStackPop();
}
