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
char nativeDispatch(u2 local);
char nativeIntOut(u2 local);
char nativeFloatOut(u2 local);
char nativeStringOut(u2 local);
char nativeCharOut(u2 local);
char nativeCharIn();
char getButtons();
char setOnBoardLEDs(u2);
char charLCDOut(u2);
char controlLCD(u2);
char currentTimeMillis();
char javaExit(u2);
void myExit(u4);

char drawPointRGB(u2 local);
char drawFillRectRGB(u2 local);
char drawRectangleRGB(u2 local);
char clearScreenRGB(u2 local);
char drawRoundRectRGB(u2 local);
char drawEllipseRGB(u2 local);
char drawCircleRGB(u2 local);
char drawLineRGB(u2 local);
char drawPointHSB(u2 local);
char drawLineHSB(u2 local);
char drawTriangleFill(u2 local);
char clearZBuffer();
char drawPointHSBZBuffer(u2 local);
char drawLineHSBZBuffer(u2 local);

/*character methods*/
char setFont(u2 local);
char setFontWindow(u2 local);
char setFontAutoLineFeed(u2 local);
char setFontFixedFont(u2 local);
char setFontCursor(u2 local);
char setFontColor(u2 local);
char drawChar(u2 local);
char drawCharAt(u2 local);
char getCharWidth(u2 local);

char nativeCharAt(u2 local);
char nativeStringLength(u2 local);

char start();
char yield();
char currentThread();
char getPriority();
char setPriority(u2 local);
char interrupt();
char interrupted();
char isInterrupted();
char isDaemon();
char setDaemon(u2 local);
char  join();
char jointimeout(u2 local);

char notify();
char notifyAll();
char wait();
char waitTime(u2 local);

char javaExit(u2);
char currentTimeMillis();
char nativeParseFloat(u2);
char typeConvert(u2);
char floatToCharArray(u2);
#endif

