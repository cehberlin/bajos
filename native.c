/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#include <stdio.h>
#include <stdlib.h>
#ifdef UNIX
#include <termios.h>
#include <unistd.h>
#endif
#include <string.h>
#include "definitions.h"
#include "bajosavr.h"
#include "stack.h"
#include "classfile.h"
#include "scheduler.h"
#include "heap.h"
#include "native.h"
// all native methods have a return value
// 0-> is a Java native void function
// 1-> is an Java native function with anyone return value (in stack)
// alle native methoden haben einen eindeutigen namen!!
// native methods of class Serial: className, methodName,...,NULL
// fill this arrays
char*	nativeCharon[]={"platform/Charon","nativeCharOut","nativeCharIn",NULL};
char* nativeString[]={"java/lang/String","nativeCharAt","nativeStringLength",NULL};
//						   classname		0				1				2
// add arrays for other classes
char* nativeThread[]={"java/lang/Thread","start","yield","sleep","currentThread","getPriority","setPriority","interrupt","interrupted",
"isInterrupted","isDaemon","setDaemon","join",NULL};
char* nativeObject[]={"java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress",NULL};
char* nativeSystem[]={"java/lang/System","exit","currentTimeMillis",NULL};

// fill this array
char**	nativeNames[]={nativeCharon,nativeString,nativeThread,nativeObject,nativeSystem};

char ** nativeName;
u2	numNativeClasses=sizeof(nativeNames)/sizeof(char**);
u2 numMethods=0;
u1*	nativeCNMN;
int numEntryNativeCNMN=40; //???
u2 i;

char	nativeDispatch( u2 cN, u2 mN,u2 local)	{
//printf("nummezthods %d:\n",numMethods);
for(i=0;i<numMethods;i++)	{
//printf(": %d %x %x  %x\n",i,(((u2)*(nativeCNMN+2*i)<<8)+(u2)(*(nativeCNMN+2*i+1))),cN,mN);
if ((((u2)*(nativeCNMN+2*i)<<8)+(*(nativeCNMN+2*i+1)))==(((u2)cN<<8)+mN))break;

}
switch(i)					{
	case	0: return nativeCharOut(local);
	case	1: return nativeCharIn();
	case	2: return nativeCharAt(local);
	case	3: return nativeStringLength(local);
	case	4: return start(local);
	case	16: return notify(local);
	case	17: return notifyAll(local);
	case	18:	return wait(local);	// ???
	default: printf(" native method not found: %d %d %d",i,cN,mN);
			   exit(-1);		}							}

void	initNativeDispatch()	{
u1* methodDescr=NULL;	// ohne signature arbeiten!!
u2 methodDescrLength=0;
if ((nativeCNMN=(u1*)malloc(2*numEntryNativeCNMN*sizeof(u1)))==NULL) exit(-1);
for(i=0;i < numNativeClasses;i++)		   {
	for(cN=0; cN < numClasses;cN++)			{
		nativeName=nativeNames[i];
		if (strncmp( *nativeName,
getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3),
		strlen(*nativeName))==0)	 
			while((*(++nativeName))!=NULL)	{
			if (findMethodByName((u1*)*nativeName,strlen(*nativeName),methodDescr,methodDescrLength)) 	{
				*(nativeCNMN+numMethods++)=(u1)cN;
				*(nativeCNMN+numMethods++)=(u1)mN;	
/*				printf("%s %d %d    %d\n",	*nativeName,cN,mN,numMethods/2	);										*/
		}
												}	}	}
numMethods/=2;				}

//"actplatform/Serial","nativeIntOut","nativeFloatOut","println","nativeCharOut","nativeCharIn"
// 0
char nativeCharOut(u2 local)		{
char val=opStackGetValue(local+1).UInt;
	printf("%c",val);
	return 0;							}
// 1
#ifdef UNIX
char	nativeCharIn()	{
struct termios oldt,newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
opStackPush((slot)(u4)ch);
return 1;	
}
#else
char (*conIn)()=(void*)(0xF004);
char	nativeCharIn()	{
char ch=conIn();
opStackPush((slot)(u4)ch);
return 1;}
#endif
//"java/lang/Thread","start"/*5*/,"yield","currentThread","getPriority()","setPriority","interrupt","interrupted",
//"isInterrupted","isDaemon","setDaemon","join"," jointimeout"
// 5

char nativeStringLength(u2 local)	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.type==STACKCPSTRING)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.pos >>8);
getU2(CP(cN,getU2(CP(cN,mySlot.stackObj.pos&0xff)+1 )) +1 );
opStackPush((slot)(u4)0);
opStackPush((slot)(u4)getU2(CP(cN,getU2(CP(cN,mySlot.stackObj.pos&0xff)+1 )) +1 ));
cN=methodStackPop();
}
else	
opStackPush((slot)(u4)0xffff);
return 1;
}

char nativeCharAt(u2 local)	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.type==STACKCPSTRING)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.pos >>8);
opStackPush((slot)(u4)getU1(CP(cN,getU2(CP(cN,mySlot.stackObj.pos&0x00ff) + 1))+3+(u2)opStackGetValue(local+1).UInt));
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
//6
char yield()	{return 0; }
//7
char currentThread(){return 1; }
//8
char getPriority(){return 1; }
//9
char setPriority(u2 local){return 0; }
//10
char interrupt(){return 0; }
//11
char interrupted(){return 1; }
//12
char isInterrupted(){return 1; }
//13
char isDaemon(){return 1; }
//14
char setDaemon(u2 local){return 0; }
char  join(){return 0; }
//15
char jointimeout(u2 local){return 0; }

// "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"
char notify(){	// not tested yet aug2007
u1 i;
ThreadControlBlock* cb=actualThreadCB;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(253);};
//darf nicht sein ->IllegalMonitorStateException
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->status==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))
cb->status=THREADWAITAWAKENED;
break;
}
return 0; }

char notifyAll() {
u1 i;
ThreadControlBlock* cb=actualThreadCB;
if (HEAPOBJECTMARKER(opStackGetValue(local).UInt).mutex!=MUTEXBLOCKED)	{ };
//darf nicht sein ->IllegalMonitorStateException
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->status==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).stackObj.pos))
cb->status=THREADWAITAWAKENED;
}
return 0; }


char wait() {
//slot mySlot=opStackPop();
u1 i;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(254);};
//darf nicht sein ->IllegalMonitorStateException
HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXNOTBLOCKED; // lock abgeben
for (i=0; i< MAXLOCKEDTHREADOBJECTS;i++)
if ((actualThreadCB->hasMutexLockForObject[i]).UInt == opStackGetValue(local).UInt )break;
if (i==MAXLOCKEDTHREADOBJECTS)exit(253); // was dann?
//printf("::: %d %d\n",i,actualThreadCB->lockCount[i]);
actualThreadCB->lockCount[i]-=1;	// count decrementieren
if (actualThreadCB->lockCount[i]==0)actualThreadCB->hasMutexLockForObject[i]=NULLOBJECT;
actualThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
actualThreadCB->status=THREADWAITBLOCKED;
return 0; }

char waitTime(u2 local){return 0; }
char getDataAddress (u4 obj)	{
return 1;	// ret val is  on Stack !!
}

//"java/lang/System","exit","currentTimeMillis"
char exitVM(u2 local){return 0; }
char currentTimeMillis(){return 1; }
