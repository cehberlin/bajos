/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-function ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/

#include <stdio.h>
#include <stdlib.h>
#include "../definitions.h"
#include "../typedefinitions.h"
#include "../bajvm.h"
#include "../classfile.h"
#include "../stack.h"
#include "../bajvm.h"
#include "../scheduler.h"
#include "../heap.h"
#include "langnative.h"


char nativeStringLength()	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.magic==CPSTRINGMAGIC)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.classNumber);
opStackPush((slot)(u4)getU2(CP(cN,getU2(CP(cN,mySlot.stackObj.pos)+1 )) +1 ));
cN=methodStackPop();
}
else	
opStackPush((slot)(u4)0xffff);
return 1;
}

char nativeCharAt()	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.magic==CPSTRINGMAGIC)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.classNumber);
opStackPush((slot)(u4)getU1(CP(cN,getU2(CP(cN,mySlot.stackObj.pos) + 1))+3+(u2)opStackGetValue(local+1).UInt));
cN=methodStackPop();
}
else	
opStackPush((slot)(u4)0);
return 1;
}

char start()	{
 cN=opStackGetValue(local).stackObj.classNumber;
  createThread();
return 0;
}

char yield()	{//by ceh
	//force scheduling!
	actualThreadCB->numTicks=0;
	return 0; 
}

char currentThread(){
  opStackPush((slot)(actualThreadCB->obj));
  return 1; }

char nativeSetPriority() { //by ceh
// thread is alive -> thread control block exists
// cN -> of method (Thread)
  u1 newPrio = opStackGetValue(local+1).UInt;
  u4* pCurrentPrio;
  slot soi = opStackGetValue(local);
  cN = soi.stackObj.classNumber;	// of object, which calls the method 
  if (!findFieldByRamName("priority",8,"I",1)) errorExit(78,"field priority not found");
  pCurrentPrio= (u4*)(heapBase+soi.stackObj.pos+fNO+1); 	// position of int field priority of the thread creating object
  if (newPrio == (*pCurrentPrio)) return 0; // nothing to do
    // search thread control block af calling object
ThreadControlBlock*	found=NULL;
u1 i,k;
  for(i=0;i<(MAXPRIORITY);i++)							{
    if (found!=NULL) break;
    for(k=0;k<(threadPriorities[i].count);k++)			{
      if ((threadPriorities[i].cb->obj).UInt == soi.UInt) { found=threadPriorities[i].cb; break;}
      threadPriorities[i].cb=threadPriorities[i].cb->succ;	}		}
  if (found==NULL) errorExit(78,"thread not found");
  removeThreadFromPriorityList(found);
  *(found->pPriority)=newPrio;
  insertThreadIntoPriorityList(found);
  return 0; 
}

//to do
char interrupt(){return 0; }

char interrupted(){return 1; }

char isInterrupted(){return 1; }

char  sleep(){return 0; }

char  join(){return 0; }

char jointimeout(){return 0; }

char lock(){
	setMutexOnObject(actualThreadCB,opStackGetValue(local));
	return 0;
}

char unlock(){
	releaseMutexOnObject(actualThreadCB,opStackGetValue(local));

	//force scheduling
	actualThreadCB->numTicks=0;

	return 0; 
}

char tryLock(){
	if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex==MUTEXBLOCKED)	{
		opStackPush((slot)0); 
	}else{
		lock();
		opStackPush((slot)1);
	}

	return 1;
}

/* "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"*/
char notify() {
	u1 i,k;
	u1 max;
	ThreadControlBlock* cb;
	if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{
 		exit(253);
	}
	
	notifyThread(opStackGetValue(local));
	return 0; 
}

char notifyAll() {
	u1 i,k,max;
	ThreadControlBlock* cb;
	if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{
		exit(249); 
	}
	/*can not be ->IllegalMonitorStateException*/
	for(i=0;i<(MAXPRIORITY);i++){
		max=(threadPriorities[i].count);
		cb=threadPriorities[i].cb;
		for(k=0;k<max;k++){
			if ((cb->state==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))
				cb->state=THREADWAITAWAKENED;
			cb=cb->succ;
		}	
	}	
	return 0; 
}


char nativeWait() {
	u1 i,k,max;
	if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{
		exit(254);
	}
	/*can not be ->IllegalMonitorStateException*/
	HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXNOTBLOCKED; /* free lock for another thread and lock this */ 
	
	ThreadControlBlock* myTCB;
	for(i=0;i<(MAXPRIORITY);i++){
		max=(threadPriorities[i].count);
		myTCB=threadPriorities[i].cb;
		for(k=0;k<max;k++){
			//alle blocked for object wecken!
			if  ((myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(local).UInt)&&
					(myTCB->state==THREADMUTEXBLOCKED)){
				myTCB->state=THREADNOTBLOCKED; //!!
				myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;		
			}
			
			myTCB=myTCB->succ;
		}	
	}
	//its better to change own state after notify, to avoid cycles
	actualThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
	actualThreadCB->state=THREADWAITBLOCKED;	
						
	return 0; 
}

char waitTime(){return 0; }

char getDataAddress (u4 obj)	{
return 1;	/* ret val is  on Stack !!*/
} 

char floatToCharArray()	{
	slot mySlot;
	f4 f=opStackGetValue(local).Float;	/* the float*/
	char buf[8];
	u1 i;
	for(i=0; i<8; ++i) {buf[i]=0;}
	snprintf(buf,7,"%f",f);
	u2 heapPos=getFreeHeapSpace(8+ 1);	/* char arr length + marker*/
	mySlot.stackObj.pos=heapPos;
	mySlot.stackObj.magic=OBJECTMAGIC;
	/*mySlot.stackObj.type=STACKNEWARRAYOBJECT;*/
	mySlot.stackObj.arrayLength=(u1)8;/* char array length*/
	opStackPush(mySlot);
	HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
	HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
	HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
	for(i=0; i<8; i++){	heapSetElement(( slot)(u4)(*(buf+i)),heapPos++);}
	return 1;
}

char typeConvert()	{
opStackPush(opStackGetValue(local));
return 1;
}


/* char arr to float*/
char nativeParseFloat()	{
slot mySlot=opStackGetValue(local); /* the char array*/
char buf[mySlot.stackObj.arrayLength];
f4 f;
u4 i;
for (i=0;i<mySlot.stackObj.arrayLength;i++) 
buf[i]= (u1)heapGetElement(mySlot.stackObj.pos+i+1).UInt;
buf[mySlot.stackObj.arrayLength]=0;
sscanf(buf,"%f",&f);
opStackPush((slot)f);
return 1;
}

char nativeSleep()	{ return 0;}

