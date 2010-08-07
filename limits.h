/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#ifndef __LIMITS__
#define __LIMITS
// denke gut für charonII mit 32K SRam
	#define MAXCLASSES 			40 //30
	#define MAXBYTECODE 			(36384)		// 16384 !!! class file bytes < 16 kB	
	#define MAXHEAP				(/*1024*/6000)			// number of u4 entries for heap-elements	
	#define MAXHEAPOBJECTLENGTH 		6096
	#define MAXTHREADS			46
	#define MAXLOCKEDTHREADOBJECTS 		24
	#define OPSTACKSIZE 			((328+96))	// u4	896 bytes
	#define METHODSTACKSIZE			((164))			// u2	128 bytes
#endif
