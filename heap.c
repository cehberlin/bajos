/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "definitions.h"
#include "bajosavr.h"
#include "stack.h"
#include "heap.h"
// heap 
void heapInit()	{
if  ((heapBase=(  slot*)malloc(sizeof( slot)*(size_t)MAXHEAP))==NULL)  printf("malloc error\n");			// heap fixed size!!
	while (heapTop > 0 )			*(heapBase+(--heapTop))= NULLOBJECT;
}

slot heapGetElement(u2 pos)			{	return *(heapBase + pos);	}

void heapSetElement( slot e,u2 pos)	{	*(heapBase+pos) = e;		}

u2	getNextHeapObjectPos(u2 pos)	{
		return  ((pos+HEAPOBJECTMARKER(pos).length)< heapTop)?  pos+HEAPOBJECTMARKER(pos).length:(heapTop+1);
											}

u2 getFreeHeapSpace(u2 length)	{	// todo er muss genau einen Platz der Länge zurückgeben
// todo verschmelzen von löchern
// erkennnen circularer referencen on heap ohne bezug zu opstack
if ((heapTop+length-1) < MAXHEAP) 	{
	HEAPOBJECTMARKER(heapTop).length=length;
	heapTop+=length;
	return heapTop-length;					} // free space on heap

u2 nextElementPos=0;
do	{		// first fit
if (( HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE) 
	&& ((HEAPOBJECTMARKER(nextElementPos).length)>=length))  return nextElementPos; // first fit	 
	}	while  ((nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop);

checkObjects(); 

// noch mal probieren
 nextElementPos=0;
do	{
if (( HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE) 
	&& ((HEAPOBJECTMARKER(nextElementPos).length)>=length))  return nextElementPos; // first fit	 
	}	while  ((nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop);

// verschmelzen
int schmelz;
for ( schmelz=0; schmelz < 3; schmelz++)	{	
printf("verschmelzen\n");
nextElementPos=0;
u2 oldElementPos=0;
while ( (nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop)	{		
if ((HEAPOBJECTMARKER(nextElementPos).status==HEAPFREESPACE)&& (HEAPOBJECTMARKER(oldElementPos).status==HEAPFREESPACE))	
{HEAPOBJECTMARKER(oldElementPos).length+=HEAPOBJECTMARKER(nextElementPos).length;
break;}
else
oldElementPos=nextElementPos;
}
// jetzt noch male Probieren
nextElementPos=0;
do	{
if (( HEAPOBJECTMARKER(nextElementPos).status == HEAPFREESPACE) 
	&& ((HEAPOBJECTMARKER(nextElementPos).length)>=length))  return nextElementPos; // first fit	 
	}	while  ((nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop);
}
// alles umsonst
printf(" no free heap space for object/array of length: 0x%x  _> tschüß",length);
exit(-1);
}


void checkObjects()	{ // static objects or objects in opstack (root objects) or objects, referenced by other objects (circular??)
u1 threadNr=0;
u2 opSPPos;
u2 nextElementPos=0;
int i=0;
u1 stillAConcatedObject;
do		{
stillAConcatedObject=0;
HEAPOBJECTMARKER(nextElementPos).rootCheck=0;
if (/*(HEAPOBJECTMARKER(nextElementPos).status==HEAPFREESPACE)||*/(HEAPOBJECTMARKER(nextElementPos).status==HEAPALLOCATEDSTATICCLASSOBJECT))	{ // empty or static
	HEAPOBJECTMARKER(nextElementPos).rootCheck=1;
	continue;							}
ThreadControlBlock* tCB=actualThreadCB;
opSPPos=opStackGetSpPos();
for (threadNr=0;threadNr<numThreads;threadNr++)											{		// searching for root objects on stack
while (opSPPos>0) 		{
		if  	(  (nextElementPos==((*(tCB->opStackBase+(--opSPPos))).stackObj.pos)) &&
				( ((*(tCB->opStackBase+(opSPPos))).stackObj.magic)==OBJECTMAGIC  ))//UInt)
					{HEAPOBJECTMARKER(nextElementPos).rootCheck=1;break;}
							}
if (HEAPOBJECTMARKER(nextElementPos).rootCheck==1) break;
tCB=tCB->succ;	
opSPPos=*(tCB->methodStackBase+tCB->methodSpPos-1);								}
if (HEAPOBJECTMARKER(nextElementPos).rootCheck==1) continue;
}	while ( (nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop);
// alle Objekte von stack erreichbar (root elements)	markiert	
while (stillAConcatedObject)	{	
stillAConcatedObject=0;
nextElementPos=0;
do 	{
if (HEAPOBJECTMARKER(nextElementPos).rootCheck==1){
												// seraching for "objects in root-objects"
for (i=1; i <HEAPOBJECTMARKER(nextElementPos).length;i++)	
if (HEAPOBJECT(nextElementPos+i).stackObj.magic==OBJECTMAGIC/*&&   HEAPOBJECTMARKER(         HEAPOBJECT(nextElementPos+i).stackObj.pos).rootCheck==0      */) {HEAPOBJECTMARKER(HEAPOBJECT(nextElementPos+i).stackObj.pos).rootCheck=1;stillAConcatedObject=1; }
}
if(stillAConcatedObject) break;
} while ( (nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop)	;
}

nextElementPos=0;
while ( (nextElementPos=getNextHeapObjectPos(nextElementPos)) <heapTop)		
if (!HEAPOBJECTMARKER(nextElementPos).rootCheck) HEAPOBJECTMARKER(nextElementPos).status=HEAPFREESPACE;
}
