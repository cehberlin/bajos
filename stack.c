/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07

#include <stdio.h>
#include <stdlib.h>

#include "bajosavr.h"
#include "definitions.h"
#include "stack.h"

// op stack hält  locals und operanden
// method stack hält globale variable (cN, mN, local,..)
static slot* 	opSp;
static u2* 		methodSp;

void opStackInit(slot** m)	{		// per thread, fixed size 
	if ((*m= (slot*) calloc((size_t)OPSTACKSIZE,sizeof(slot))) == NULL){printf("malloc error\n");exit(-1);}			
}
void opStackPush( slot val)				{	*(opSp++)=val;						}	//  sp grothws with increasing addresses
																								// and shows to TOS first free place

 slot opStackPop()							{	return *(--opSp);						}	// operand stack stores 4 bytes

 slot  opStackPeek()							{	return *(opSp-1);					}

void opStackPoke( slot val)				{	*(opSp-1)=val;						}

void opStackSetValue(u2 pos, slot val)	{	*(opStackBase+pos)=val;			}
 
 slot opStackGetValue(u2  pos)			{	return *(opStackBase+pos);		}

 u2 opStackGetSpPos()						{  	return (opSp-opStackBase);		}	// relative to actual base

void opStackSetSpPos(u2 pos)			{	opSp=pos+opStackBase;			}

void methodStackInit(u2** m)				{
	if ((*m=(u2*)calloc((size_t)METHODSTACKSIZE,sizeof(u2)))==NULL){printf("malloc error\n");exit(-1);}
												}

void methodStackPush(u2 val)		{	*(methodSp++)=val;					}

u2 methodStackPop()					{	return *(--methodSp);				}
u2 methodStackPeek()					{	return *(methodSp-1);				}
u1 methodStackEmpty()										{	
	return (methodSp==methodStackBase) ? 1:0;			}

u2 methodStackGetSpPos()			{  	return (methodSp-methodStackBase);		}	// relative to actual base

void methodStackSetSpPos(u2 pos)	{	methodSp=pos+methodStackBase;			}
