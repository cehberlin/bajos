/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#include <stdio.h>
#include <stdlib.h>

#include "bajvm.h"
#include "definitions.h"
#include "stack.h"

// op stack holds  locals and operanden
// method stack holds globale variable (cN, mN, local,..)
static slot* 	opSp;
static u2* 	methodSp;

void opStackInit(slot** m)	{		// per thread, fixed size 
#if (LINUX||AVR8||AVR32LINUX)
	if ((*m= (slot*) calloc((size_t)OPSTACKSIZE,sizeof(slot))) == NULL)
			errorExit(-1,"op stack malloc error\n");			
#else
// classfiles - heap - (opstack methodstack)/ per thread
//make it better
*m=(slot*)((u4)apClassFileBase+MAXBYTECODE+4*MAXHEAP+numThreads*(4*OPSTACKSIZE+2*METHODSTACKSIZE));
#endif
}
void opStackPush( slot val)	{	*(opSp++)=val;			}	
//  sp grothws with increasing addresses
// and shows to TOS -> first free place

slot opStackPop()		{	return *(--opSp);		}	
// operand stack stores 4 bytes

slot  opStackPeek()		{	return *(opSp-1);		}

void opStackPoke( slot val)	{	*(opSp-1)=val;			}

void opStackSetValue(u2 pos, slot val)	{*(opStackBase+pos)=val;	}
 
slot opStackGetValue(u2  pos)	{	return *(opStackBase+pos);	}

u2 opStackGetSpPos()		{  	return (opSp-opStackBase);	}	
// relative to actual base

void opStackSetSpPos(u2 pos)	{	opSp=pos+opStackBase;		}

void methodStackInit(u2** m)	{
#if (LINUX||AVR8||AVR32LINUX)
	if ((*m=(u2*)calloc((size_t)METHODSTACKSIZE,sizeof(u2)))==NULL)
				errorExit(-1,"method stack malloc error\n");
#else
*m=(u2*)((u4)(apClassFileBase+MAXBYTECODE+4*MAXHEAP+4*OPSTACKSIZE+numThreads*(4*OPSTACKSIZE+2*METHODSTACKSIZE)));
#endif
									}

void methodStackPush(u2 val)	{	*(methodSp++)=val;		}
u2 methodStackPop()		{	return *(--methodSp);		}
u2 methodStackPeek()		{	return *(methodSp-1);		}
u1 methodStackEmpty()		{	return (methodSp==methodStackBase) ? 1:0;	}
u2 methodStackGetSpPos()	{  	return (methodSp-methodStackBase);		}
// relative to actual base
void methodStackSetSpPos(u2 pos){	methodSp=pos+methodStackBase;	}
