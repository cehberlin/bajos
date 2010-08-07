/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
/********************************************************************************************/
/* 2006 - Erweiterungen von Matthias Böhme und Jakob Fahrenkrug			*/
/* Studenten der Informatik an der FHW-Berlin/ Fachbereich Berufsakademie	*/
/* Ausbildungsbetrieb: Bayer-Schering AG											*/
/*******************************************************************************************/
// statisches Linken von native-methoden code zur jvm !! -> febr 2007
// no double, no long
// no access flags evaluation
// wait,notify not tested
// no utf8 but ascii
// ignore some attributes
// no interfaces
// no inner classes
// no classloader
// static fields in classes must be the first elements in a class!!!!!!!!!!!!!!!!!!!!!
// javac -g:none
// no ...
// and errors ........................................................................
// 64836 May  9 14:34 baJVM128.bin*
// 65310 Aug 1 2007
// 67170 Aug 10 2007
// 64740 sept 07
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#ifndef UNIX
#include <avr/io.h>
#include <avr/interrupt.h>
#endif

#include "definitions.h"
#define __DEF_GLOBALS__
#include "bajosavr.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "native.h"

#include "scheduler.h"
// einmal durch 4.7.06 unix
// lief am 24.8.06 erstmals auf atmega128

#ifndef UNIX
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
// Monitor functions
//void(*sendByte) (u1) 			=(void*)((0xf000+6));
//u1(*getByte) (void) 			=(void*)((0xf000+4));

//u2 (*loadInSRam)(u1* ,u1*)	=(void*)((0xf000+26));

ISR(TIMER2_COMP_vect)	{ asm volatile	("\t	pop	r29 \n\
												\t	pop	r28\n\
												\t	pop	r0\n\
												\t	pop	r0\n\
												\t	pop	r1\n\
												jmp	2*(0xf03e)");}
// das muss jedesmal ueberprueft werden

void sendByte (char c) { asm volatile	("call	2*(0xf006)");	}
char getByte () { asm volatile	("call	2*(0xf004)");	}
void gotoMonitor () { asm volatile	("call	2*(0xf002)");	}
void saveCPU () { asm volatile	("call	2*(0xf03e)");	}

void my_init_sp (void) __attribute__ ((naked)) \
     __attribute__ ((section (".init2")));
void	my_init_sp (void)	{
         SPL = 0xff;
         SPH = 0x6E;			}
#endif

int main(int argc,char* argv[]){
	initHW();
	#ifndef UNIX
	printf("start\n");
#endif
	initVM(argc-1,argv);					// einlesen der classfiles (superclass first!!) und erzeugen von "class-object" für klassen mit static fields
											// initialisierun des native interface
	createThread();						// for main
	opStackBase		= actualThreadCB->opStackBase;
	opStackSetSpPos(0);

	methodStackBase	= actualThreadCB->methodStackBase;
	methodStackSetSpPos(0);	
#ifndef UNIX
	printf("SP: %x cFB: %x hB: %x oPSB: %x mSB: %x\n", 256*SPH+SPL,classFileBase, heapBase, opStackBase, methodStackBase);
#endif
	executeClInits();							// anfangs ausfuehren aller clinit-methoden!!		
	printf("<clinit> 's executed");
	if (findMain() == 0)		return 1;		// eine und nur eine main muss sein
												// wir suchen die Klasse mit main und fuehren main aus
												// allgemeiner Ausfuehren von main der Klasse X sucht in Klassenhierarchie die erste main
												//  no error handling!!
	printf("  -> run <main> :\n");
	opStackPush((slot) (u4)0);				// args parameter to main (should be a string array)
	opStackSetSpPos(findMaxLocals());
	run(); 									//  run main
#ifndef UNIX
gotoMonitor();
#endif
	return 0;

}
#ifndef UNIX
void exit(int n)	{
gotoMonitor();
}
#endif

void executeClInits()		{ 				// alle clinit in er eingelesenen reihenfolge
DEBUGPRINTSTACK;
DEBUGPRINTLOCALS;
DEBUGPRINTHEAP;
	for (cN=0; cN < numClasses;cN++)	
	if (findMethodByName((u1*)"<clinit>",8,(u1*)"()V",3))	{
			opStackPush(cs[cN].classInfo); // wenn <clinit>, dann gibt evtl. es static fields, die zu initialisieren sind
			opStackSetSpPos(findMaxLocals());	
			run();									};	
}
#ifndef Unix
#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
// atmega128 Monitor functions
#define		loadInSRam1		(2*(0xf000+26))

u2 	loadInSram1(u1*) __attribute__ ((naked));

u2 loadInSram1(unsigned char* addr)	{
asm	 (INLINEASM(jmp,loadInSRam1));			}

#endif


void initVM(int argc, char* argv[]){	// read, analyze classfiles and fill structures
	u2 length;
	if ((classFileBase=(u1*)malloc((size_t)MAXBYTECODE))==NULL) printf("malloc error\n");	// Platz fuer classfiles -> fixed size
	heapInit();
	length=0;
	#ifdef UNIX
		for (cN=0; cN < argc; cN++)			{
			cs[cN].classFileStartAddress=classFileBase+length;
			cs[cN].classFileLength=readClassFile((u1*)argv[cN+1],cs[cN].classFileStartAddress);
			analyzeClass(&cs[cN]);	
			length+=cs[cN].classFileLength;	}
	#else
		
		printf("Laden der BootKlassen - Geben Sie ein  'w' ein\n"); 
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
(*loadInSram1)(classFileBase);
u1* addr;
numClasses=*classFileBase;
addr=classFileBase+4; // after numclasses
for (cN=0; cN<numClasses;cN++)	{
cs[cN].classFileStartAddress=addr+4;	// after length
cs[cN].classFileLength=(u1)(*addr)+256*(u1)(*(addr+1));
			analyzeClass(&cs[cN]);	
addr+=cs[cN].classFileLength+4;
}
cN--;
addr-=4;
		do
		{
			cN++;
			cs[cN].classFileStartAddress=addr+length;
			cs[cN].classFileLength=readClassFile(NULL,cs[cN].classFileStartAddress);
			length+=cs[cN].classFileLength;
			if (cs[cN].classFileLength ==0) break;
			analyzeClass(&cs[cN]);
		} 
		while(cs[cN].classFileLength !=0);

	#endif

	numClasses=cN;
	DEBUGPRINTHEAP;
	initNativeDispatch();
	printf("initNativeDispatch\n");
}

void initHW(){
#ifndef UNIX
		stdout = stdin=stderr = &uart_str;	}
int uart_putchar(char c, FILE *stream){
		loop_until_bit_is_set(UCSR0A, UDRE0);
		UDR0 = c;
		if (c=='\n') uart_putchar('\r',stream);
		return 0;								}
		
int uart_getchar(FILE *stream)	{
		loop_until_bit_is_set(UCSR0A, RXC0);
		return (int)UDR0;	
#endif
}
		
void VT102Attribute (u1 fgcolor, u1 bgcolor)	{
    printf("%c",0x1b);
      printf("%c",'[');
//            printf("%c",'4');
     printf("%c",fgcolor);
      //printf("%c",';');
      //printf("%c",40 + bgcolor);
      printf("%c",'m');
}
/*
printf(OUTSTREAM,"Geben Sie ein  'w' ein\n"); 
// the damu2 classFileLength=0;
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
u2 length1=(*loadInSram)(classFileBase);

numClasses=*(classFileBase);
printf("numclasses %d\n",numClasses);
printf("%c\n",*classFileBase);
printf("%c\n",*(classFileBase+1));
printf("%c\n",*(classFileBase+2));
printf("%c\n",*(classFileBase+3));

u1* addr=classFileBase+4; // after numclasses
for (cN=0; cN<numClasses;cN++)	{
cs[cN].classFileStartAddress=addr+4;
cs[cN].classFileLength=(u2)*(u4*)addr;
			analyzeClass(&cs[cN]);	
addr+=cs[cN].classFileLength;
}
*/