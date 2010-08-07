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

//array of priority lists
ThreadControlBlock*  threadPriorities[MAXPRIORITY];

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
	//set thread in matching priority list
	if (actualThreadCB==NULL){ //==the first thread (main thread) is created
		//init priority array because first thread is created
		u1 i;	
		for(i=0;i<MAXPRIORITY;i++){
			threadPriorities[i]=NULL;	
		}	
		actualThreadCB=t;
	}
	insertThreadIntoPriorityList(t);

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
	numThreads++;					
}

/*
 * Inserts thread into the correct priority list and if the priority of
 * the current running thread is lower than these, current thread gets
 * his last tick(last bytecode executed)
 * by Christopher-Eyk Hrabia
 */
void insertThreadIntoPriorityList(ThreadControlBlock* t){
	ThreadControlBlock* pos;
	u1 prio=t->priority;
	pos=threadPriorities[prio];
	if(pos==NULL){
		threadPriorities[prio]=t;
		t->pred=t;
		t->succ=t;
	}else{
		t->pred=pos;
		t->succ=pos->succ;
		t->pred->succ=t;
		t->succ->pred=t;
	}

	if(actualThreadCB->priority<prio){
		actualThreadCB->numTicks=0;		
	}
}
/*
 * Function does only remove the given Thread from his current list
 * but does not delete the thread 
 * list will be recognised by thread->priority!! Do not edit before!
 * by Christopher-Eyk Hrabia
 */
void removeThreadFromPriorityList(ThreadControlBlock* t){
	ThreadControlBlock* temp=t->succ;
	u1 prio=t->priority;
	if(t==temp){ //last thread of current priority
		threadPriorities[prio]=NULL;
	}else{
		temp->pred=t->pred;
		temp->pred->succ=temp;
		if(t==threadPriorities[prio]){
			threadPriorities[prio]=temp;
		}
	}
}

/*
 * Delete one thread by Christopher-Eyk Hrabia
 */
void	deleteThread(void)	{
	removeThreadFromPriorityList(actualThreadCB);
	u1 i=MAXPRIORITY;
	while(threadPriorities[i]==NULL){ //it should not be possible that i becomes lower than 0 therefore NO CHECK
		i--;	
	}
	free(actualThreadCB->methodStackBase); /*/ crash -> to do*/ /*EDIT CEH: AFTER IMPLEMENTING NEW SCHEDULING IT SEEMS TO WORK*/
	free(actualThreadCB->opStackBase);
	free(actualThreadCB);
	actualThreadCB =threadPriorities[i];
	methodStackBase=actualThreadCB->methodStackBase;
	methodStackSetSpPos(actualThreadCB->methodSpPos);
	opStackBase=actualThreadCB->opStackBase;
	opStackSetSpPos(methodStackPop());
	pc=methodStackPop();
	mN=methodStackPop();
	cN=methodStackPop();
	local=methodStackPop();
	numThreads--;			
}

/*
 * the scheduling process function, this function is called after every bytecode
 * one thread can run until is numTicks are 0. The numTicks a thread gets after he
 * was scheduled is his priority+1. The scheduler schedules if the numTicks of
 * current thread are 0. //CEH
 */
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
	//set current thread on first position of priority list to ensure that on next scheduling a new thread could be run
	threadPriorities[actualThreadCB->priority]=actualThreadCB;
	//find highest priority list with members
	u1 i=MAXPRIORITY;
	while(threadPriorities[i]==NULL){ //it should not be possible that i becomes lower than 0 therefore NO CHECK
		i--;	
	}
	actualThreadCB=threadPriorities[i];
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
	actualThreadCB->numTicks= i+1; //actualThreadCB->priority=i
	methodStackBase=actualThreadCB->methodStackBase;
	methodStackSetSpPos(actualThreadCB->methodSpPos);
	opStackBase=actualThreadCB->opStackBase;
	opStackSetSpPos(methodStackPop());
	pc=methodStackPop();
	mN=methodStackPop();
	cN=methodStackPop();
	local=methodStackPop();
}
