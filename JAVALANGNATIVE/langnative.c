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

char isAlive()	{//by ceh
//TODO
	return 1;
}

char start()	{
cN=opStackGetValue(local).stackObj.classNumber;
createThread();
return 0;
}
/*6*/
char yield()	{//by ceh
	//force scheduling!
	actualThreadCB->numTicks=0;
	return 0; 
}
/*7*/
char currentThread(){return 1; }
/*8*/
char getPriority(){ //by ceh
	opStackPush((slot)(u4)actualThreadCB->priority);
	return 1; 
}
/*9*/
char setPriority(){ //by ceh
	u1 newPrio;
	u1 currentPrio;
	newPrio=opStackGetValue(local+1).UInt;
	currentPrio=actualThreadCB->priority;
	if(newPrio>=MINPRIORITY && newPrio<=MAXPRIORITY && newPrio!=currentPrio){
		removeThreadFromPriorityList(actualThreadCB);
		actualThreadCB->priority=newPrio;
		insertThreadIntoPriorityList(actualThreadCB);
		//if priority was decreased the scheduling for next bytecode is forced
		if(currentPrio>newPrio){
			actualThreadCB->numTicks=0;
		}
	}
	return 0; 
}
/*10*/
char interrupt(){return 0; }
/*11*/
char interrupted(){return 1; }
/*12*/
char isInterrupted(){return 1; }
/*13*/
char isDaemon(){return 1; }
/*14*/
char setDaemon(){return 0; }
char  join(){return 0; }
/*15*/
char jointimeout(){return 0; }

/* "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"*/
char notify(){	/* not tested yet aug2007*/
	u1 i,k;
	u1 max,breakNested;
	ThreadControlBlock* cb=actualThreadCB;
	if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(253);};
	/*can not be ->IllegalMonitorStateException*/

	for(i=0;i<(MAXPRIORITY);i++){
		max=(threadPriorities[i].count);
		breakNested=0;
		cb=threadPriorities[i].cb;
		for(k=0;i<max;k++){
			if ((cb->state==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt)){	
				cb->state=THREADWAITAWAKENED;
				breakNested=1;
				break;
			}	
			cb=cb->succ;
		}
		if(breakNested)break;	
	}	
	return 0; 
}

char notifyAll() {
u1 i;
return 0;
ThreadControlBlock* cb=actualThreadCB;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{exit(249); };
/*can not be ->IllegalMonitorStateException*/
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->state==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))
cb->state=THREADWAITAWAKENED;
}
return 0; }


char nativeWait() {
u1 i;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(254);};
/*can not be ->IllegalMonitorStateException*/
HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXNOTBLOCKED; /* lock abgeben*/
actualThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
actualThreadCB->state=THREADWAITBLOCKED;
ThreadControlBlock* myTCB=actualThreadCB;
	for (i=1; i < numThreads; i++)	{	/* alle blocked for object wecken!*/
		myTCB=myTCB-> succ;
		if  ((myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(local).UInt)&&
				(myTCB->state==THREADMUTEXBLOCKED))	{
			myTCB->state=THREADNOTBLOCKED; /*!!*/
			myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;		}
					}
return 0; }

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

