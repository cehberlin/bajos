/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
// version 0.2 vom 15.3.08
/********************************************************************************************
Erweiterungen von:
2006 Matthias Böhme und Jakob Fahrenkrug, Ausbildungsbetrieb: Bayer-Schering Pharma AG
2008 Anna Maria Damm, Bayer Schering Pharma AG
 	 H.-Christian Hecht, CoMedServ GmbH
	 Adrian Lang,Fritz-Haber-Institut
	 Stephan Bauer, Bayer Schering Pharma AGstudent
	 Christopher Hartl, Alcatel-Lucent AG
	 Jascha Radziewski, DP-IT-Service GmbH
 Studenten der Informatik an der FHW-Berlin/Fachbereich Berufsakademie	
********************************************************************************************/
// speed is not our primary goal!!!
// no double, no long
// no access flags evaluation
// no utf8 but ascii
// ignore some attributes
// no classloader
// no ...
// and errors ........................................................................
/*AVR8(CharonII) EVK1100 NGW100 STK1000 LINUX -> Target Systems*/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef AVR8
#include <avr/io.h>
#include <avr/interrupt.h>
#include "AVR8/lcd.h"
#endif

#include "definitions.h"
#define __DEF_GLOBALS__
#include "bajvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "native.h"
#include "scheduler.h"
#include "bajvm.h"
#include "platform.h"
// einmal durch 4.7.06 unix
// lief am 24.8.06 erstmals auf atmega128
// lief im Herbst 07 auf AVR32

#if !(AVR32LINUX||LINUX || AVR8 || NGW100||STK1000||EVK1100)
#error ein Zielsystem muß es doch geben?
#endif


int main(int argc,char* argv[]){
	initHW();
	printf("Bajos starting\n");
	initVM(argc-1,argv);	
// einlesen der classfiles und erzeugen von "class-object" für klassen mit static fields
// initialisierung des native interface
	createThread();			// for main
	opStackBase		= actualThreadCB->opStackBase;
	opStackSetSpPos(0);
	methodStackBase	= actualThreadCB->methodStackBase;
	methodStackSetSpPos(0);	
#ifdef AVR8
	printf("SP: %x cFB: %x hB: %x oPSB: %x mSB: %x cs: %x\n", 
			256*SPH+SPL,classFileBase, heapBase, opStackBase, methodStackBase,cs);
#endif
	printf("start clinit\n");
	executeClInits();		// anfangs ausfuehren aller clinit-methoden!!		
	printf("<clinit> 's executed");
	if (findMain() == 0)		{printf("no main found %d %d\n",numClasses,cN);return 1;	}	
// wir suchen die erste Klasse mit main und fuehren main aus
// allgemeiner: Ausfuehren von main der Klasse X sucht in Klassenhierarchie die erste main
// no error handling!!
	printf("  -> run <main> :\n");
	opStackPush((slot) (u4)0);				// args parameter to main (should be a string array)
	opStackSetSpPos(findMaxLocals());
	run(); 									//  run main
	return 0;
}

void executeClInits()		{ 				// alle clinit in er eingelesenen reihenfolge
//DEBUGPRINTSTACK;
//DEBUGPRINTLOCALS;
//DEBUGPRINTHEAP;
	for (cN=0; cN < numClasses;cN++)	
	if (findMethodByName("<clinit>",8,"()V",3))	{
			opStackPush(cs[cN].classInfo); 
// wenn <clinit>, dann gibt evtl. es static fields, die zu initialisieren sind
			opStackSetSpPos(findMaxLocals());	
			run();									};	
}


// class file stehen in linux im DS (malloc)
// im avr8 im sram	(malloc)
// in ap7000 und uc3a:
// 	bootclassen im flash
// 	anwendungsklassen im DS(Ram) -> hard coded
void initVM(int argc, char* argv[]){	// read, analyze classfiles and fill structures
	u4 length;
// use malloc!!
#ifdef EVK1100
classFileBase=(u1*)UC3A_FLASH_JAVA_BASE;  	// boot classes in flash
apClassFileBase=(u1*)UC3A_SDRAM_JAVA_BASE;	// app classes in sdram
#endif
#ifdef STK1000
classFileBase=(u1*)STK1000_FLASH_JAVA_BASE;  			// boot classes in flash
apClassFileBase=STK1000_SDRAM_JAVA_BASE;	// app classes in sdram
#endif
#ifdef NGW100
classFileBase=(u1*)NGW_FLASH_JAVA_BASE;  // boot classes in flash
apClassFileBase=(u1*)NGW_SDRAM_BASE;	// app classes in sdram
#endif

#if (AVR32LINUX||LINUX||AVR8)
    classFileBase=(u1*)malloc((size_t) MAXBYTECODE);
    if (classFileBase==NULL)
        errorExit(-1,"malloc error while trying to allocate %d bytes for class files.\n", MAXBYTECODE);
    if (classFileBase==NULL) {
        MALLOCERR(MAXBYTECODE, "class files");
    }
	// Platz fuer classfiles -> fixed size
#endif

	heapInit();	// linux avr8 malloc , others hard coded!
	length=0;

#ifdef LINUX
    if (argc > MAXCLASSES)
        errorExit(-1,"ERROR: trying to load %d classes, MAXCLASSES is %d\n", argc, MAXCLASSES);
		for (cN=0; cN < argc; cN++)			{
			cs[cN].classFileStartAddress=classFileBase+length;
			cs[cN].classFileLength=readClassFile(argv[cN+1], cs[cN].classFileStartAddress);
			analyzeClass(&cs[cN]);
			length+=cs[cN].classFileLength;
	        if (length > MAXBYTECODE) {
    	        printf("MAXBYTECODE reached!\n"); exit(-1);
    	    }
}
#endif

#if (NGW100||STK1000|| EVK1100)
// analyze der bootclasses, which are programmed in flash
u1* addr;
u4 temp;
char buf[5];
strncpy(buf,classFileBase,4);
buf[4]=0;
sscanf(buf,"%4d",&temp);
numClasses=(u1)temp;
addr=classFileBase+4; // after numclasses
for (cN=0; cN<numClasses;cN++)	{
strncpy(buf,addr,4);
sscanf(buf,"%4d",&temp);
cs[cN].classFileStartAddress=addr+4;	// after length of class
cs[cN].classFileLength=temp;//(u1)(*addr)+256*(u1)(*(addr+1));
analyzeClass(&cs[cN]);	
addr+=cs[cN].classFileLength+4;
}
printf("%d bootclasses are loaded\n",cN);
cN=numClasses;
// thats to boot classes
// now the application classes
cN--;
addr-=4;
addr=apClassFileBase;
length=0;
		do
		{
		printf("load application classes-> type \"w\" \n");
			cN++;
			cs[cN].classFileStartAddress=apClassFileBase+length;
			cs[cN].classFileLength=readClassFile(NULL,cs[cN].classFileStartAddress);
			printf("\n");
			length+=cs[cN].classFileLength;
			analyzeClass(&cs[cN]);
			printf("still another appl. class ? (y)");
			if (conIn()!='y') break;
		} 
		while(cs[cN].classFileLength !=0);
cN++;//!!
#endif

#ifdef AVR8
		printf("load boot classes - type  'w'! -> \n"); 
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
//(*loadInSram1)(classFileBase);
(*loadInSram)(classFileBase);
printf("\ndone\n");
u1* addr;
u4 temp;
char buf[5];
strncpy(buf,classFileBase,4);
buf[4]=0;
sscanf(buf,"%4d",&temp);
numClasses=(u1)temp;
addr=classFileBase+4; // after numclasses
for (cN=0; cN<numClasses;cN++)	{
strncpy(buf,addr,4);
sscanf(buf,"%4d",&temp);
cs[cN].classFileStartAddress=addr+4;	// after length of class
cs[cN].classFileLength=temp;//(u1)(*addr)+256*(u1)(*(addr+1));
analyzeClass(&cs[cN]);	
addr+=cs[cN].classFileLength+4;
printf("bootclass: %x length:%x loaded\n",cN,temp);
}

cN=numClasses;
// thats to boot classes
// now the application classes

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
printf("load java class: \n");
DEBUGPRINTHEAP;
initNativeDispatch();
printf("initNativeDispatch\n");
}



void errorExit(char nr,const char *format, ...)	 {
va_list list;
va_start(list,format);
vprintf(format,list);
va_end(list);
exit(nr);
 }
