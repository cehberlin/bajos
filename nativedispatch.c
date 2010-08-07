/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#include <stdio.h>
#include <stdlib.h> 
#include "typedefinitions.h"
// insert and update here functionForNativeMethodType-arrays for classes with native methods
// array length ->  at least up to last native method < methods_count
// lock at methods in the *.java or *.class file in increasing order 
// if method is non native -> insert NULL, otherwise pointer to nativce C-function
#define STRINGCLASSMETHODS	7
#define THREADCLASSMETHODS	16
#define OBJECTCLASSMETHODS	9
#define FLOATCLASSMETHODS	17

#include "nativedispatch.h"
#include "JAVALANGNATIVE/langnative.h"
// fill this array with classes containing native methods
// dont forget to update nativedispatch.h
const char*	nativeClassNames[] =		{
			"platform/PlatForm",
			"java/lang/Object",
			"java/lang/String",
			"java/lang/Thread",
			"java/lang/Float"	
};

#if (LINUX||AVR32-LINUX)
#include "LINUX/native.h"
#define PLATFORMCLASSMETHODS	5
functionForNativeMethodType functionForNativePlatFormMethod[PLATFORMCLASSMETHODS] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis
};
#endif

#ifdef AVR8
#include "AVR8/native.h"
#define PLATFORMCLASSMETHODS	8
functionForNativeMethodType functionForNativePlatFormMethod[PLATFORMCLASSMETHODS] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis,
	charLCDOut,
	controlLCD,
	getTemperature
};
#endif

#ifdef EVK1100
#include "EVK1100/native.h"
#define PLATFORMCLASSMETHODS	14
functionForNativeMethodType functionForNativePlatFormMethod[PLATFORMCLASSMETHODS] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
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
};
#endif

#ifdef NGW100
#include "NGW100/native.h"
#define PLATFORMCLASSMETHODS	5
functionForNativeMethodType functionForNativePlatFormMethod[PLATFORMCLASSMETHODS] =	{
	NULL,
	nativeCharIn,
	nativeCharOut,
	nativeExit,
	currentTimeMillis
};
#endif

#ifdef STK1000
#include "STK1000/native.h"
#define PLATFORMCLASSMETHODS	28
functionForNativeMethodType functionForNativePlatFormMethod[PLATFORMCLASSMETHODS] =	{
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

functionForNativeMethodType functionForNativeStringMethod[STRINGCLASSMETHODS] =	{
	NULL,
	NULL,
	NULL,
	NULL,
	nativeStringLength,
	NULL,
	nativeCharAt
};

functionForNativeMethodType functionForNativeThreadMethod[THREADCLASSMETHODS] =	{
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

functionForNativeMethodType functionForNativeFloatMethod[FLOATCLASSMETHODS] =	{
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

functionForNativeMethodType functionForNativeObjectMethod[OBJECTCLASSMETHODS] =	{
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
const functionForNativeMethodType* funcArray[NUMNATIVECLASSES]	=	{
	functionForNativePlatFormMethod,
	functionForNativeObjectMethod,
	functionForNativeStringMethod,
	functionForNativeThreadMethod,
	functionForNativeFloatMethod					
};
