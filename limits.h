/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#ifndef __LIMITS__
#define __LIMITS
#ifdef AVR8
	#define MAXCLASSES 			15
	#define MAXBYTECODE 			16000 	//!!! class file bytes	
	#define MAXHEAP				800	// number of u4 entries for heap-elements	
	#define MAXHEAPOBJECTLENGTH 		60
	#define MAXTHREADS			5
	#define MAXLOCKEDTHREADOBJECTS 		4
	#define OPSTACKSIZE 			96	// u4	
	#define METHODSTACKSIZE			164	// u2	
	#define MAXNATIVEMETHODS		40
#else
	#define MAXCLASSES 			45 //30
	#define MAXBYTECODE 			52768	// 16384 !!! class file bytes < 16 kB	
	#define MAXHEAP				6000	// number of u4 entries for heap-elements	
	#define MAXHEAPOBJECTLENGTH 		6096
	#define MAXTHREADS			46
	#define MAXLOCKEDTHREADOBJECTS 		24
	#define OPSTACKSIZE 			(328+96)// u4
	#define METHODSTACKSIZE			164	// u2
	#define MAXNATIVEMETHODS		80
#endif
#endif
