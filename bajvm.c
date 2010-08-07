/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 - 1.10.07
// version 0.2 - 15.3.08
// version 0.3 - 15.1.09
/********************************************************************************************
Erweiterungen von:
2006	Matthias Böhme und Jakob Fahrenkrug, Ausbildungsbetrieb: Bayer-Schering Pharma AG
2008	Anna Maria Damm, Stephan Bauer, Bayer Schering Pharma AG
	H.-Christian Hecht, CoMedServ GmbH
	Adrian Lang, Fritz-Haber-Institut
	Christopher Hartl, Alcatel-Lucent AG
	Jascha Radziewski, DP-IT-Service GmbH
2009	Rainer Kirchhoff, MSA-Auer
	Friedrich Grosse, Deutsche Post IT
	Sebastian Staroske, Joerdis Loesser, Bayer Schering Pharma AG
	Chris Engel, CONTROL
	Felix Fehlberg, Berliner Volksbank
	Steffen Kalisch, COMED
Studenten der Informatik an der HWR-Berlin/Fachbereich Berufsakademie	
********************************************************************************************/
// speed is not our primary goal!!!
// no double, no long
// no access flags evaluation
// no utf8 but ascii
// ignore some attributes
// no classloader
// no ...
// and errors ........................................................................
/*AVR8(CharonII) EVK1100 NGW100 STK1000 LINUX AVR32-LINUX -> Target Systems*/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#if LINUX||AVR32LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#ifdef AVR8
#include <avr/io.h>
#include "AVR8/platform.h"
#endif
#include "definitions.h"
#define __DEF_GLOBALS__
#include "bajvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "scheduler.h"
#include "bajvm.h"

#ifdef NGW100
#include "NGW100/platform.h"
#endif
#ifdef EVK1100
#include "EVK1100/platform.h"
#endif
#ifdef STK1000
#include "STK1000/platform.h"
#endif

#if !(AVR32LINUX||LINUX || AVR8 || NGW100||STK1000||EVK1100)
#error ein Zielsystem muß es doch geben?
#endif


int main(int argc,char* argv[]){
	initHW();
	printf("Bajos starting\n");
	initVM(argc-1,argv);	
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
	executeClInits();	
	printf("<clinit> 's executed");
	if (findMain() == 0)		{printf("no main found %d %d\n",numClasses,cN);return 1;	}	
	printf("  -> run <main> :\n");
	opStackPush((slot) (u4)0);	// args parameter to main (should be a string array)
	opStackSetSpPos(findMaxLocals());
	run(); 				//  run main
	return 0;
}

void executeClInits()		{
//DEBUGPRINTSTACK;
//DEBUGPRINTLOCALS;
//DEBUGPRINTHEAP;
	for (cN=0; cN < numClasses;cN++)	
	if (findMethodByName("<clinit>",8,"()V",3))	{
			opStackPush(cs[cN].classInfo); 
			opStackSetSpPos(findMaxLocals());	
			run();				}
}


// class files stored for linux in DS (malloc)
// for avr8 in sram	(malloc)
// for ap7000 and uc3a:	bootclasses in flash
// 			application classes  DS(Ram) -> hard coded
void initVM(int argc, char* argv[]){	// read, analyze classfiles and fill structures
	u4 length;
// use malloc!!
#ifdef EVK1100
classFileBase=(char*)UC3A_FLASH_JAVA_BASE;  	// boot classes in flash
apClassFileBase=(char*)UC3A_SDRAM_JAVA_BASE;	// app classes in sdram
#endif

#ifdef STK1000
classFileBase=(char*)STK1000_FLASH_JAVA_BASE;  	// boot classes in flash
apClassFileBase=STK1000_SDRAM_JAVA_BASE;	// app classes in sdram
#endif

#ifdef NGW100
classFileBase=(char*)NGW_FLASH_JAVA_BASE;  // boot classes in flash
apClassFileBase=(char*)NGW_SDRAM_JAVA_BASE;	// app classes in sdram
#endif

#if (AVR32LINUX||LINUX||AVR8)
    classFileBase=(char*)malloc((size_t) MAXBYTECODE);
    if (classFileBase==NULL)
        errorExit(-1,"malloc error while trying to allocate %d bytes for class files.\n", MAXBYTECODE);
    if (classFileBase==NULL) {
        MALLOCERR(MAXBYTECODE, "class files");
    }
	// memory for classfiles -> fixed size
#endif
	heapInit();	// linux avr8 malloc , others hard coded!
	length=0;
#if LINUX|| AVR32LINUX
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
// analyze bootclasses, which are programmed in flash
char* addr;
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
addr=apClassFileBase;
length=0;
		do
		{
		printf("load application classes-> type \"w\" \n");
			cN++;
			cs[cN].classFileStartAddress=addr+length;
			cs[cN].classFileLength=readClassFile(NULL,cs[cN].classFileStartAddress);
			printf("\n");
			length+=cs[cN].classFileLength;
			analyzeClass(&cs[cN]);
			printf("still another appl. class ? (y) \n");
			if (conIn()!='y') break;
		} 
		while(cs[cN].classFileLength !=0);
//!!
cN++;
#endif

#if AVR8
		printf("load boot classes - type  'w'! -> \n"); 
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
(*loadInSram)(classFileBase);
printf("\ndone\n");
char* addr;
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
}

void errorExit(char nr,const char *format, ...)	 {
va_list list;
va_start(list,format);
vprintf(format,list);
va_end(list);
exit(nr);
 }
