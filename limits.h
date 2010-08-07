/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#ifndef __LIMITS__
#define __LIMITS
// denke gut für charonII mit 32K SRam
	#define MAXCLASSES 			24
	#define MAXBYTECODE 			(16384)		// 16384 !!! class file bytes < 16 kB	
	#define MAXHEAP				(/*1024*/400)			// number of u4 entries for heap-elements	
	#define MAXHEAPOBJECTLENGTH 		4096
	#define MAXTHREADS			16
	#define MAXLOCKEDTHREADOBJECTS 		8
	#define OPSTACKSIZE 			((128+96))	// u4	896 bytes
	#define METHODSTACKSIZE			((64))			// u2	128 bytes
#endif
