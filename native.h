/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
#ifndef __NATIVE__
#define __NATIVE__
#include "bajosavr.h"
#include "stack.h"
void	initNativeDispatch();
char	nativeDispatch( u2 cN, u2 mN,u2 local);
char nativeIntOut(u2 local);
char nativeFloatOut(u2 local);
char nativeStringOut(u2 local);
char nativeCharOut(u2 local);
char nativeCharIn();
char nativeStringLength(u2 local);

char nativeCharAt(u2 local);

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

char exitVM(u2 local);
char currentTimeMillis();

#endif

