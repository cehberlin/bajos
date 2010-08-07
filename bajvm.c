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
/* version 0.6.- 15.6.10 */
/********************************************************************************************
Erweiterungen von:
2006	Matthias Böhme und Jakob Fahrenkrug, Ausbildungsbetrieb: Bayer-Schering Pharma AG
2008	Anna Maria Damm, Stephan Bauer, Bayer Schering Pharma AG
	Hans-Christian Hecht, CoMedServ GmbH
	Adrian Mikosch Lang, Fritz-Haber-Institut
	Christopher Hartl, Alcatel-Lucent AG
	Jascha Radziewski, DP-IT-Service GmbH
2009	Rainer Kirchhoff, MSA-Auer
	Friedrich Grosse, Deutsche Post IT
	Sebastian Staroske, Joerdis Loesser, Bayer Schering Pharma AG
	Chris Engel, CONTROL
	Felix Fehlberg, Berliner Volksbank
	Steffen Kalisch, COMED
2010	Christopher-Eyk Hrabia BTC
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
/*
but include are:
garbage collector
threads
synchronized constructs
native methods interface
exception handling
scheduler
*/
/*AVR8(arduinoMega and CharonII) EVK1100 EVK1104 NGW100 STK1000 LINUX AVR32-LINUX -> Target Systems*/

#include <stdio.h>

#ifdef AVR8
#include <avr/io.h>
#include <avr/pgmspace.h>
#endif

#include "definitions.h"

#define __DEF_GLOBALS__
#include "bajvm.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "scheduler.h"

#if !(AVR32LINUX||LINUX || AM || CH || XPLAIN || NGW100||STK1000||EVK1100|| EVK1104)
#error ein Zielsystem muß es doch geben?
#endif

int main(int argc,char* argv[]){
	initHW();
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("Bajos starting\n"));
#else
	printf("Bajos starting\n");
#endif
	initVM(argc-1,argv);	
	createThread();			/* for main*/
	opStackBase = actualThreadCB->opStackBase;
	opStackSetSpPos(0);
	methodStackBase	= actualThreadCB->methodStackBase;
	methodStackSetSpPos(0);	
#ifdef AVR8
	printf_P(PSTR("SP: %x cFB: %x hB: %x oPSB: %x mSB: %x cs: %x\n"), 
			256*SPH+SPL,AVR8_FLASH_JAVA_BASE, heapBase, opStackBase, methodStackBase,cs);
#endif

#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("start clinit"));
#else
	printf("start clinit");
#endif

for (cN=0; cN < numClasses;cN++)
if (findMethodByName("<clinit>",8,"()V",3))	{
			opStackPush(cs[cN].classInfo); 
			opStackSetSpPos(findMaxLocals());
			run();			}
for (cN=0; cN < numClasses;cN++)
if (findMethodByName("main",4,"([Ljava/lang/String;)V",22))	{
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	printf_P(PSTR("  -> run <main> :\n"));
#else
	printf("  -> run <main> :\n");
#endif

	opStackPush((slot) (u4)0);	/* args parameter to main (should be a string array)*/
	opStackSetSpPos(findMaxLocals());
	run(); 				/*  run main*/
	return 0;						}
#ifdef AVR8	// change all avr8 string to flash strings gives more data ram space for java!!
	errorExit(1,PSTR("\nno main found %d"),numClasses);
#else
	errorExit(1,"\nno main found %d",numClasses);
#endif													
	
	return 1;
}


void errorExit(char nr,const char *format, ...)	{
	va_list list;
	va_start(list,format);
#ifdef AVR8
	vfprintf_P(stdout, format, list); //vprintf_P does not exist in current avr-libc
#else
	vprintf(format,list);
#endif
	va_end(list);
	exit(nr);				}
