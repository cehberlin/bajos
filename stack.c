/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* fuer lehrzwecke,...*/
/* version 0.1 vom 1.10.07*/

#include <stdio.h>
#include <stdlib.h>
#ifdef AVR8 
#include <avr/pgmspace.h>
#endif

#include "bajvm.h"
#include "definitions.h"
#include "stack.h"

/* op stack holds  locals and operanden*/
/* method stack holds globale variable (cN, mN, local,..)*/
#ifndef AVR8
static slot* 	opSp;
static u2* 	methodSp;
#else
slot* 	opSp;
u2* 	methodSp;
#endif
void opStackInit(slot** m)	{		/* per thread, fixed size */
#if (LINUX||AVR8||AVR32LINUX)
	if ((*m= (slot*) calloc((size_t)OPSTACKSIZE,sizeof(slot))) == NULL)
			MALLOCERR(OPSTACKSIZE * sizeof(slot), "op stack");			
#else
/* classfiles - heap - (opstack methodstack)/ per thread*/
/*make it better*/
*m=(slot*)((u4)appClassFileBase+MAXBYTECODE+4*MAXHEAP+numThreads*(4*OPSTACKSIZE+2*METHODSTACKSIZE));
#endif
}
<<<<<<< .mine
#ifndef AVR8
=======
#ifndef AVR8 //all these functions are rewritten in assembler to increase speed => routines_stack.asm
>>>>>>> .r1129
void opStackPush( slot val)	{	*(opSp++)=val;			}	
/*  sp grothws with increasing addresses*/
/* and shows to TOS -> first free place*/

slot opStackPop()		{	return *(--opSp);		}	
/* operand stack stores 4 bytes*/

slot  opStackPeek()		{	return *(opSp-1);		}

void opStackPoke( slot val)	{	*(opSp-1)=val;			}

void opStackSetValue(u2 pos, slot val)	{*(opStackBase+pos)=val;	}

slot opStackGetValue(u2  pos)	{	return *(opStackBase+pos);	}

u2 opStackGetSpPos()		{  	return (opSp-opStackBase);	}

/* relative to actual base*/
void opStackSetSpPos(u2 pos)	{	opSp=pos+opStackBase;		}	
#endif

<<<<<<< .mine
void opStackSetSpPos(u2 pos)	{	opSp=pos+opStackBase;		}
#endif

=======
>>>>>>> .r1129
void methodStackInit(u2** m)	{
#if (LINUX||AVR8||AVR32LINUX)
	if ((*m=(u2*)calloc((size_t)METHODSTACKSIZE,sizeof(u2)))==NULL)
			MALLOCERR(METHODSTACKSIZE * sizeof(u2), "method stack");			
#else
*m=(u2*)((u4)(appClassFileBase+MAXBYTECODE+4*MAXHEAP+4*OPSTACKSIZE+numThreads*(4*OPSTACKSIZE+2*METHODSTACKSIZE)));
#endif
}
#ifndef AVR8 //all these functions are rewritten in assembler to increase speed => routines_stack.asm
#ifndef AVR8
void methodStackPush(u2 val)	{	*(methodSp++)=val;		}
u2 methodStackPop()		{	return *(--methodSp);		}
u2 methodStackPeek()		{	return *(methodSp-1);		}
u2 methodStackGetSpPos()	{  	return (methodSp-methodStackBase);		}

/* relative to actual base*/
void methodStackSetSpPos(u2 pos){	methodSp=pos+methodStackBase;	}

u1 methodStackEmpty()		{	return (methodSp==methodStackBase) ? 1:0;	}
#endif

#endif
/*//BH
void dummy(void)	{
char* s1,*s2;
char n;
strncmpFlashFlash(s1,(const char*) s2, n);
}
*/
