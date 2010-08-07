/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* for educational purposes,...*/
/* version 0.1 - 1.10.07*/
/* version 0.2 - 15.3.08*/
/* version 0.3 - 15.1.09*/
/* version 0.4 - 15.6.09*/
/* version 0.5 - 15.1.10 */
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
students of informatics at the HWR-Berlin/Berufsakademie	
********************************************************************************************/
/* speed is not our primary goal!!!*/
/* no double, no long*/
/* no complete access flags evaluation*/
/* no utf8 but ascii*/
/* ignore some attributes*/
/* no classloader*/
/* no ...*/
/* and errors ........................................................................*/
/*AVR8(arduinoMega and CharonII) EVK1100 NGW100 STK1000 LINUX AVR32-LINUX -> Target Systems*/

#include <stdio.h>

#ifdef AVR8
#include <avr/io.h>
#endif

#include "definitions.h"

#define __DEF_GLOBALS__
#include "bajvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "scheduler.h"

#if !(AVR32LINUX||LINUX || AM || CH || NGW100||STK1000||EVK1100)
#error ein Zielsystem muß es doch geben?
#endif

int main(int argc,char* argv[]){
	initHW();
	printf("Bajos starting\n");
	initVM(argc-1,argv);	
	createThread();			/* for main*/
	opStackBase = actualThreadCB->opStackBase;
	opStackSetSpPos(0);
	methodStackBase	= actualThreadCB->methodStackBase;
	methodStackSetSpPos(0);	
#ifdef AVR8
	printf("SP: %x cFB: %x hB: %x oPSB: %x mSB: %x cs: %x\n", 
			256*SPH+SPL,AVR8_FLASH_JAVA_BASE, heapBase, opStackBase, methodStackBase,cs);
#endif
	printf("start clinit\n");
	for (cN=0; cN < numClasses;cN++)
	if (findMethodByName("<clinit>",8,"()V",3))	{
			opStackPush(cs[cN].classInfo); 
			opStackSetSpPos(findMaxLocals());
			run();				}	
	for (cN=0; cN < numClasses;cN++)
	if (findMethodByName("main",4,"([Ljava/lang/String;)V",22))	{
	printf("  -> run <main> :\n");
	opStackPush((slot) (u4)0);	/* args parameter to main (should be a string array)*/
	opStackSetSpPos(findMaxLocals());
	run(); 				/*  run main*/
	return 0;;							}
	errorExit(1,"no main found %d %d\n",numClasses);
	return 1;
}


void errorExit(char nr,const char *format, ...)	{
	va_list list;
	va_start(list,format);
	vprintf(format,list);
	va_end(list);
	exit(nr);				}
