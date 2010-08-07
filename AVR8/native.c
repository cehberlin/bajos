/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
// remember:
// invokespecial Operand Stack
// ..., objectref, [arg0, [arg1 ...]] -> ...
// invokestatic: Operand Stack
// ..., [arg0, [arg1 ...]] -> ...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>  
#include "../definitions.h"
#include "../typedefinitions.h"
#include "../bajvm.h"
#include "../stack.h"
#include "../classfile.h"
#include "../scheduler.h"
#include "../heap.h"
#include "native.h"
#include "platform.h"
#include "lcd.h"
#include "thermo.h"

#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
// atmega128 Monitor functions
#define		loadInSRam1		(2*(0xf000+26))
extern char (*conIn)();

char	nativeCharIn()	{
opStackPush((slot)(u4)conIn());
return 1;		}

char nativeCharOut()		{
char val=opStackGetValue(local+1).UInt;
	printf("%c",val);
	return 0;		}

// added 2008 by: Stephan Bauer, FHW-BA Berlin
// Bayer Schering Pharma AG
u1 rowLCD=0;
u1 columnLCD=0;
#define LCDRowLength 15
char charLCDOut()	{
LCD_Putch(opStackGetValue(local+1).UInt);
if (rowLCD == LCDRowLength){
rowLCD=0;
columnLCD=(columnLCD+1)&0xf7;
}
else rowLCD++;
return 0;		}

char controlLCD()	{
u4 val=opStackGetValue(local+1).UInt;
// 0x00000000 clear
// 0x0100yyxx cursor at x y
switch ((val&0xff000000)>>24)	{
case 0x00: LCD_SendCmd(1);
CASE 0x01:	if (val&0x0000ff00)LCD_SendCmd(0xc0+(val&0x000000ff));
			else LCD_SendCmd(0x80+(val&0x000000ff));
}
return 0; 		}


char nativeExit()	{
goto *0xf002;	//asm	 (INLINEASM(jmp,0xf002));
}

// added 2009 by: Hannes Walz, FHW-BA Berlin
// Berliner Stadtreinigung
char getTemperature() 	{		
	opStackPush((slot) (f4) nativeGetTemperature());
	return 1;
}

char currentTimeMillis()	{
opStackPush((slot)(u4)timerMilliSec);
	return 1;
}
SIGNAL(SIG_OUTPUT_COMPARE0)		{
	timerMilliSec++;	
}
