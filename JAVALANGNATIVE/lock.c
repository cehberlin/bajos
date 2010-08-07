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
#include "lock.h"

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

