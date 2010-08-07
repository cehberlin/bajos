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
t->status=THREADNOTBLOCKED;
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
*(t->opStackBase)=opStackGetValue(local); // referenc to caller object (from start())
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
u1 i;
if ((numThreads==1)/*&&((actualThreadCB->status)==THREADNOTBLOCKED)*/) return;
// scheduling -> next thread
// gegenwärtig thread wechsel nach jedem jvm befehl
methodStackPush(local);
methodStackPush(cN);
methodStackPush(mN);
methodStackPush(pc);
methodStackPush((u2)(opStackGetSpPos()));
actualThreadCB->methodSpPos=methodStackGetSpPos();//methodSp;
//printf("th %d %d %d %d\n",numThreads,actualThreadCB->status,actualThreadCB->succ->status,THREADMUTEXBLOCKED);
do {
actualThreadCB=actualThreadCB->succ;
//printf(":::: %d\n",actualThreadCB->tid);
if ((actualThreadCB->status)==THREADNOTBLOCKED) break;
if ((actualThreadCB->status)==THREADMUTEXBLOCKED) break;
//if ((actualThreadCB->status)==THREADMUTEXBLOCKED) continue; // handle hier!!
if ((actualThreadCB->status)==THREADWAITBLOCKED) continue;
if (((actualThreadCB->status)==THREADWAITAWAKENED) && 
	(HEAPOBJECTMARKER((actualThreadCB->isMutexBlockedOrWaitingForObject).stackObj.pos).status==MUTEXBLOCKED))
	continue;
// awakened and mutexnotblocked
if ((actualThreadCB->status)==THREADWAITAWAKENED)	{
HEAPOBJECTMARKER((actualThreadCB->isMutexBlockedOrWaitingForObject).stackObj.pos).status=MUTEXBLOCKED;
actualThreadCB->status=THREADNOTBLOCKED;
									for (i=0; i<MAXLOCKEDTHREADOBJECTS;i++)	
										if (actualThreadCB->hasMutexLockForObject[i].UInt!=NULLOBJECT.UInt)continue;else break;
										if (i==MAXLOCKEDTHREADOBJECTS) {printf("1too many locks\n"); exit(251);	}
								actualThreadCB->lockCount[i]+=1;	// count
								actualThreadCB->hasMutexLockForObject[i]=(actualThreadCB->isMutexBlockedOrWaitingForObject);
actualThreadCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;
}
} while (1);

methodStackBase=actualThreadCB->methodStackBase;
methodStackSetSpPos(actualThreadCB->methodSpPos);
opStackBase=actualThreadCB->opStackBase;
//currThread=actualThreadCB->tid;
opStackSetSpPos(methodStackPop());
pc=methodStackPop();
mN=methodStackPop();
cN=methodStackPop();
local=methodStackPop();
}

