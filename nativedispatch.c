/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#include <stdio.h>
#include <stdlib.h> 
#include "typedefinitions.h"
// insert and update here functionForNativeMethodType-arrays for classes with native methods
// array length ->  at least up to last native method < methods_count
// lock at methods in the *.java or *.class file in increasing order 
// if method is non native -> insert NULL, otherwise pointer to nativce C-function

#include "JAVALANGNATIVE/langnative.h"
// fill this array with classes containing native methods

const char*	nativeClassNames[] =		{
			"platform/PlatForm",
			"java/lang/Object",
			"java/lang/String",
			"java/lang/Thread",
			"java/lang/Float"	
};

u2 numNativeClassNames=sizeof(nativeClassNames)/sizeof(char*);

#if (LINUX||AVR32LINUX)
#include "LINUX/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis
};
#endif

#ifdef AVR8
#include "AVR8/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis,
	charLCDOut,
	controlLCD,
	getTemperature,
	getAnalogueValue,
	nativeConStat,
	nativeSetBarGraph,
	nativeGetDIL,
	nativeSetPort,
	nativeSetIOPort,
	nativeGetPort,
	nativeGetIOPort
};
#endif

#ifdef EVK1100
#include "EVK1100/native.h"

functionForNativeMethodType functionForNativePlatFormMethod[] =	{
	NULL,
	nativeCharIn,	// for our board only TXD,RXD works correct on UART0
	nativeCharOut,
	conStat,
	getButtons,
	setOnBoardLEDs,
	charLCDOut,
	controlLCD,
	nativeExit,
	currentTimeMillis,
	ser1In,
	ser1Out,
	ser1Stat,
	ser1RTS,
	ser1DTR,
	pwmStart,
	pwmStop,
	adcGetValue
// usb, ether, sdcard
};
#endif

#ifdef NGW100
#include "NGW100/native.h"
functionForNativeMethodType functionForNativePlatFormMethod[] = {
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis
};
#endif

#ifdef STK1000
#include "STK1000/native.h"
functionForNativeMethodType functionForNativePlatFormMethod[] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis,
/*Grafik-Methoden*/
	drawPointRGB,
	drawFillRectRGB,
	drawRectangleRGB,
	clearScreenRGB,
	drawRoundRectRGB,
	drawEllipseRGB,
	drawCircleRGB,
	drawLineRGB,
	drawPointHSB,
	drawLineHSB,
	drawTriangleFill,
	clearZBuffer,
	drawPointHSBZBuffer,
	drawLineHSBZBuffer,
/*Schrift-Methoden*/
	setFont,
	setFontWindow,
	setFontAutoLineFeed,
	setFontFixedFont,
	setFontCursor,
	setFontColor,
	drawChar,
	drawCharAt,
	getCharWidth
};
#endif

functionForNativeMethodType functionForNativeStringMethod[] =	{
	NULL,
	NULL,
	NULL,
	NULL,
	nativeStringLength,
	NULL,
	nativeCharAt
};

functionForNativeMethodType functionForNativeThreadMethod[] =	{
	NULL,
	NULL,
	NULL,
	NULL,
	start,
	yield,
	nativeSleep,
	currentThread,
	getPriority,
	setPriority,
	interrupt,
	interrupted,
	isInterrupted,
	isDaemon,
	setDaemon,join
};

functionForNativeMethodType functionForNativeFloatMethod[] =	{
	NULL,
	NULL,
	NULL,
	floatToCharArray,
	NULL,
	NULL,
	NULL,
	nativeParseFloat,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	typeConvert,
	typeConvert
};

functionForNativeMethodType functionForNativeObjectMethod[] =	{
	NULL,
	NULL,
	notify,
	notifyAll,
	nativeWait,
	NULL,
	waitTime,
	NULL,
	getDataAddress
};

// insert array of function pointer
const functionForNativeMethodType* funcArray[]	=	{
	functionForNativePlatFormMethod,
	functionForNativeObjectMethod,
	functionForNativeStringMethod,
	functionForNativeThreadMethod,
	functionForNativeFloatMethod					
};
