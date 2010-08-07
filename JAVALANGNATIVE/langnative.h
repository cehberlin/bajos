/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
//String
char nativeCharAt();
char nativeStringLength();

// Thread
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
char join();
char jointimeout();
char nativeSleep();

// Object
char nativeWait();
char notify();
char notifyAll();
char wait();
char waitTime();
char getDataAddress();

// Float
char nativeParseFloat();
char typeConvert();
char floatToCharArray();
