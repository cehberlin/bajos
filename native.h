/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
#ifndef __NATIVE__
#define __NATIVE__
#include "bajvm.h"
#include "stack.h"
void initNativeDispatch();
char nativeDispatch();
char nativeIntOut();
char nativeFloatOut();
char nativeStringOut();
char nativeCharOut();
char nativeCharIn();
char getButtons();
char setOnBoardLEDs();
char charLCDOut();
char controlLCD();
char currentTimeMillis();
char javaExit();
void myExit(u4);

char drawPointRGB();
char drawFillRectRGB();
char drawRectangleRGB();
char clearScreenRGB();
char drawRoundRectRGB();
char drawEllipseRGB();
char drawCircleRGB();
char drawLineRGB();
char drawPointHSB();
char drawLineHSB();
char drawTriangleFill();
char clearZBuffer();
char drawPointHSBZBuffer();
char drawLineHSBZBuffer();

/*character methods*/
char setFont();
char setFontWindow();
char setFontAutoLineFeed();
char setFontFixedFont();
char setFontCursor();
char setFontColor();
char drawChar();
char drawCharAt();
char getCharWidth();

char nativeCharAt();
char nativeStringLength();

char start();
char yield();
char currentThread();
char getPriority();
char setPriority();
char interrupt();
char interrupted();
char isInterrupted();
char isDaemon();
char setDaemon();
char  join();
char jointimeout();

char notify();
char notifyAll();
char wait();
char waitTime();

char currentTimeMillis();
char nativeParseFloat();
char typeConvert();
char floatToCharArray();
char getTemperature();
#endif

