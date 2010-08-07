/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* fuer lehrzwecke,...*/
/* version 0.1 vom 1.10.07*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ...*/

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
#include "ds182x.h"

#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
/* atmega128 Monitor functions*/
#define		loadInSRam1		(2*(0xf000+26))
extern char (*conIn)();

char	nativeCharIn()	{
opStackPush((slot)(u4)conIn());
return 1;		}

char nativeCharOut()		{
char val=opStackGetValue(local+1).UInt;
	printf("%c",val);
	return 0;		}

/* added 2008 by: Stephan Bauer, FHW-BA Berlin*/
/* Bayer Schering Pharma AG*/
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
/* 0x00000000 clear*/
/* 0x0100yyxx cursor at x y*/
switch ((val&0xff000000)>>24)	{
case 0x00: LCD_SendCmd(1);
CASE 0x01:	if (val&0x0000ff00)LCD_SendCmd(0xc0+(val&0x000000ff));
			else LCD_SendCmd(0x80+(val&0x000000ff));
}
return 0; 		}


char nativeExit()	{
goto *0xf002;	/*asm	 (INLINEASM(jmp,0xf002));*/
}

/* added 2009 by: Hannes Walz, FHW-BA Berlin*/
/* Berliner Stadtreinigung*/
char getTemperature() 	{
	init();
	u2 temp	= readTemperature(0);
	u2 frac;
	convertTemperature(0, &temp, &frac);	
	opStackPush((slot) (f4) ((float)temp + (frac / 10000.f)));
	return 1;
}

char currentTimeMillis()	{
	opStackPush((slot)(u4)timerMilliSec);
	return 1;
}


/* added 2009 by: Sebastian Starroske, Bayer Schering Pharma AG*/
/* needs to parameters:*/
/*	int channel	the channel on which the signal is set*/
/*	int mode	the mode (8bit or 10bit)*/
char getAnalogueValue(){
	int channel = opStackGetValue(local+1).UInt;
	int mode = opStackGetValue(local+2).UInt;
	u2 result;

	result = adc_ReadChannel((u1) channel);
	
	if (mode == 0) 
		/* 8bit mode*/
		/* only the 8 most significant bytes are needed*/
		result = result >> 2;

	opStackPush((slot)(u4)result);
	return 1;
}

/* Added 2009 by Sebastian Starroske
   This function starts the AD-Converter (on PortF) and reads the result of a single conversion.
   After the Conversion is finished, the AD converter is disabled.*/
u2 adc_ReadChannel(u1 channel) {
	u2 result;
	
	ADMUX = channel;	/* setting the channel on Reg ADMUX*/
	ADMUX |= (0<<REFS1) | (1<<REFS0); /* AVCC 4.3 V reference*/

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);    /* enable ADC, set prescaler: 128*/

	ADCSRA |= (1<<ADSC);	/* start single conversion*/
	loop_until_bit_is_set(ADCSRA, ADSC);
    	result = ADCW;		/* read result*/
	return result;
}



/* added 2009 by: Friedrich Große & Rainer Kirchhoff, FHW-BA Berlin*/
/* Deutsche Post IT-Services GmbH & MSA Auer GmbH*/
char nativeConStat() {
	u1 status = UCSR0A>>5;
	status &= 5;
	status |= status >>1;
	opStackPush((slot) (u4) status);	
	return 1;
}

char nativeSetBarGraph() {
	/*Get value*/
	u_char value = opStackGetValue(local+1).bytes[0];
	DevBoardShiftLedOut(value);
	return 0;
} 

char nativeGetDIL() {
	u_char val = DevBoardShiftByteIn();
	/*put Value on JavaOpstack*/
	opStackPush((slot) (u4) val);
	return 1;
}

char nativeSetPort() {
	/*Get possible 16-Bit adr.*/
	u2 portAddr = opStackGetValue(local+1).bytes[0];
	portAddr |= opStackGetValue(local+1).bytes[1]<<8;
	/*Get value*/
	u2 portValue = opStackGetValue(local+2).bytes[0];
	/*Assign Value to adr*/
	*((u2*)portAddr) = portValue;
	return 0;
}

char nativeSetIOPort() {
	/*Get possible 16-Bit adr.*/
	u2 portAddr = opStackGetValue(local+1).bytes[0];
	portAddr |= (opStackGetValue(local+1).bytes[1]<<8);
	/*Get value*/
	u2 portValue = opStackGetValue(local+2).bytes[0];

	/*Set DataDirection*/
	*((u2*)(portAddr-1)) = 0xff;
	/*Assign Value to Port (Adr)*/
	*((u2*)portAddr) = portValue;
	return 0;
}

char nativeGetIOPort() {
	/*Get possible 16-Bit adr.*/
	u2 portAddr = opStackGetValue(local+1).bytes[0];
	portAddr |= opStackGetValue(local+1).bytes[1]<<8;
	/*Set DataDirection*/
	*((u2*)(portAddr-1)) = 0x00;

	/*put Value on JavaOpstack*/
	opStackPush((slot) (u4) *((u2*)portAddr));
	return 1;
}

char nativeGetPort() {
	/*Get possible 16-Bit adr.*/
	u2 portAddr = opStackGetValue(local+1).bytes[0];
	portAddr |= opStackGetValue(local+1).bytes[1]<<8;

	/*put Value on JavaOpstack*/
	opStackPush((slot) (u4) *((u2*)portAddr));
	return 1;
}
