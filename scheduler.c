/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#include<stdio.h>
#include "definitions.h"
#include "typedefinitions.h"
#include "bajvm.h"
#include "classfile.h"
#include "stack.h"
#include "scheduler.h"
#include "heap.h"
/*
//EXAMPLE FOR ITERATING OVER ALL THREADS:
void 	iterateOverThreads2(){
	u1 k,i,max;
	ThreadControlBlock* t;
	printf("start\n");
	for(i=0;i<(MAXPRIORITY);i++){
		max=(threadPriorities[i].count);
		t=threadPriorities[i].cb;
		for(k=0;k<max;k++){
			//do something with t
			printf("NR: %d\n", t->tid);
			//do something with t
			t=t->succ;
		}	
	}
}
*/

void	createThread(void)			{
  if (numThreads == MAXTHREADS)	{
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
	t->state=THREADNOTBLOCKED;
	
	//set thread in matching priority list
	if (actualThreadCB==NULL)	{ //==the first thread (main thread) is created
		//init priority array because first thread is created
		u1 i;	
		for(i=0;i<(MAXPRIORITY);i++){
			threadPriorities[i].cb=NULL;
			threadPriorities[i].count=0;	
		}	
		actualThreadCB=t;
		t->obj=NULLOBJECT;
		mainThreadPriority[0] = (u4)NORMPRIORITY;	// priority (and alive) of main thread -> immutable
		mainThreadPriority[1] = (u4)1;		// alive -> doesnt need for main thread??
		t->pPriority=mainThreadPriority;
	}
	else				{
		  cN=opStackGetValue(local).stackObj.classNumber;
	  	  if (!findFieldByRamName("priority",8,"I",1)) errorExit(77,"field priority not found");
		  t->pPriority=(u4*)(heapBase+opStackGetValue(local).stackObj.pos+fNO+1);
		   // position of int field priority of the thread creating object, next field is aLive
		  cN=opStackGetValue(local).stackObj.classNumber;  // restore class number of object
		  t->obj=opStackGetValue(local);
		}
	*((t->pPriority)+1)=(u4)1;		// isALive == true
	t->numTicks=*(t->pPriority);
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
	u1 prio=*t->pPriority-1;
	if (prio > 10) exit(99);
	pos=threadPriorities[prio].cb;
	if(pos==NULL){
		threadPriorities[prio].cb=t;
		t->pred=t;
		t->succ=t;
	}else{
		t->pred=pos;
		t->succ=pos->succ;
		pos->succ=t;
		t->succ->pred=t;
	}
	threadPriorities[prio].count++;
	if ((*actualThreadCB->pPriority-1)<prio){
		actualThreadCB->numTicks=0;		// force scheduling
	}
}
/*
 * Function does only remove the given Thread from his current list
 * but does not delete th>e thread 
 * list will be recognised by *(thread->pPriority)!! Do not edit before!
 * by Christopher-Eyk Hrabia
 */
void removeThreadFromPriorityList(ThreadControlBlock* t){
//	ThreadControlBlock* temp=t->succ;
	u1 prio=*t->pPriority-1;
	if (prio > 10) exit(100);
	if(threadPriorities[prio].count == 1 ){ //last thread of current priority
		threadPriorities[prio].cb=NULL;
	}else{
		t->pred->succ=t->succ;
		t->succ->pred=t->pred;
		if(t==threadPriorities[prio].cb){
			threadPriorities[prio].cb=t->pred;
		}
	}
	threadPriorities[prio].count--;
}

/*
 * Delete one thread by Christopher-Eyk Hrabia
 */
void	deleteThread(void)	{
	*((actualThreadCB->pPriority)+1)=(u4)0;		// isALive == false
	removeThreadFromPriorityList(actualThreadCB);
	u1 i=MAXPRIORITY-1;
	while(threadPriorities[i].count == 0){ //it should not be possible that i becomes lower than 0 therefore NO CHECK
		i--;	
	}
	free(actualThreadCB->methodStackBase); /*/ crash -> to do*/ /*EDIT CEH: AFTER IMPLEMENTING NEW SCHEDULING IT SEEMS TO WORK*/
	free(actualThreadCB->opStackBase);
	free(actualThreadCB);
	actualThreadCB =threadPriorities[i].cb;
	actualThreadCB->numTicks=0;	// force scheduling
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
 * was scheduled is his priority. The scheduler schedules(get active) if the numTicks of
 * current thread are 0. //CEH
 */
void scheduler(void)	{
	if (numThreads == 1) return;
	//A Thread runs until his numTicks is 0
	if(((--actualThreadCB->numTicks) && ((actualThreadCB->state)==THREADNOTBLOCKED)))	return;
		// select a runnable thread
	u1 p,n,mark;
	ThreadControlBlock*	found;
	for (p=(MAXPRIORITY-1); p!=255; p--)	{
	  mark=0;
	  found=threadPriorities[p].cb;
	  for (n=0; n < threadPriorities[p].count; n++) 	{
	      found=found->succ;
	      if ((found == actualThreadCB) && (n == 0) && ((found->state)==THREADNOTBLOCKED)) 
		{ actualThreadCB->numTicks= *(actualThreadCB->pPriority); 	  		return; }
		      /* found in the highest priority list lonely old buddy -> take it against*/
	      if (( found == actualThreadCB ) && ((found->state)==THREADNOTBLOCKED))	{ mark=1;   continue;	}	
		      // mark it and take it, if not another found
	      if ((found->state)==THREADNOTBLOCKED) 				{ mark=2; break;} // I take it
	      if ((found->state)==THREADMUTEXBLOCKED) 				continue;  // next n
	      if ((found->state)==THREADWAITBLOCKED) 				continue;  // next n
	      if (((found->state)==THREADWAITAWAKENED) && 
		((HEAPOBJECTMARKER((found->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex) ==MUTEXBLOCKED))
										continue; 
	      /* awakened and mutexnotblocked*/
	      if ((found->state)==THREADWAITAWAKENED)	{ //not nesessary because no other state ist possible
		HEAPOBJECTMARKER((found->isMutexBlockedOrWaitingForObject).stackObj.pos).mutex=MUTEXBLOCKED;
		found->state=THREADNOTBLOCKED;
		found->isMutexBlockedOrWaitingForObject=NULLOBJECT;		{ mark=2;	break; }		
							}	 } // end for n
	      if (mark==2) break;
	      if (mark == 1)
		{ actualThreadCB->numTicks= *(actualThreadCB->pPriority); 		  	return; }
	      /* not another thread for priority is runnable, take the old thread */	
		} // end for p
	// assume: not all threads are blocked
	// if interrupts or so are able to change thread state inster an infinity loop here
	if (mark == 0) errorExit(111,"scheduling error");
	/* scheduling -> next thread*/
	methodStackPush(local);
	methodStackPush(cN);
	methodStackPush(mN);
	methodStackPush(pc);
	methodStackPush((u2)(opStackGetSpPos()));
	actualThreadCB->methodSpPos=methodStackGetSpPos();
	actualThreadCB=found;
	threadPriorities[*actualThreadCB->pPriority-1].cb=actualThreadCB;		
	//reset numTicks
	actualThreadCB->numTicks= *actualThreadCB->pPriority;
	methodStackBase=actualThreadCB->methodStackBase;
	methodStackSetSpPos(actualThreadCB->methodSpPos);
	opStackBase=actualThreadCB->opStackBase;
	opStackSetSpPos(methodStackPop());
	pc=methodStackPop();
	mN=methodStackPop();
	cN=methodStackPop();
	local=methodStackPop();
}
void scheduler1(void)	{
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
	threadPriorities[(*(actualThreadCB->pPriority))-1].cb=actualThreadCB;
	//find highest priority list with members
	u1 prio=MAXPRIORITY-1;
	actualThreadCB=threadPriorities[prio].cb;
	u1 max=(threadPriorities[prio].count);
	do 	{
		//this loop prevent the scheduler from spinning in one empty or complete block priority list
		while(!max){
			prio--;
				if (prio > 10) exit(101);
			actualThreadCB=threadPriorities[prio].cb;
			max=(threadPriorities[prio].count);
			//it should not be possible to have all threads blocked or all empty, therefore no check for prio<0
		}
		actualThreadCB=actualThreadCB->succ;
		max--;
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
	actualThreadCB->numTicks= prio; //*(actualThreadCB->pPriority)=i
	methodStackBase=actualThreadCB->methodStackBase;
	methodStackSetSpPos(actualThreadCB->methodSpPos);
	opStackBase=actualThreadCB->opStackBase;
	opStackSetSpPos(methodStackPop());
	pc=methodStackPop();
	mN=methodStackPop();
	cN=methodStackPop();
	local=methodStackPop();
}
