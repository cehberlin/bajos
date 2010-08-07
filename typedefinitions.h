/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
#ifndef __TYPEDEFINITIONS__
#define __TYPEDEFINITIONS__
#include "limits.h"
	typedef unsigned char	u1;
	typedef signed char	s1;
	typedef float		f4;
	#if (LINUX || AVR32UC3A|| AVR32AP7000)
	#if ( __i386__ || AVR32UC3A|| AVR32AP7000 ) 
		typedef  unsigned short		u2;
		typedef  unsigned int		u4;
		typedef  unsigned long	long	u8;	
		typedef  signed short		s2;
		typedef  signed int		s4;
	#endif
	#ifdef __x86_64__
		typedef  unsigned short	u2;
		typedef  unsigned int		u4;
		typedef  unsigned long		u8;	
		typedef  signed short		s2;
		typedef  signed int			s4;
	#endif
	#endif
	#ifdef AVR8
		typedef  unsigned int		u2;
		typedef  unsigned long		u4;
		typedef  unsigned long long	u8;
		typedef  signed int			s2;		//mb added "int"
		typedef  signed long 		s4;
	#endif

typedef struct 	{
	u4 status:2;				// 0 -> free for further use; 1 -> allcated static class object; 2-> allocated other object; 3-> allocated arrays
	u4 rootCheck:1;			// 0 -> 1-> root object (opstack or static) or free or "object in object" !!!
	u4	mutex:1;				// mutex for object
	u4 length:16;				// max 8k 4 byte alloated slots for object!!elements for an object,array on the heap, including heapObjMarker
								// expandable 
	u4 magic:4;				//  um Objects und prim types auf dem heap zu unterscheiden !!!!!

	u4 xxx:8;
								// reducable
					}  heapObjectMarker ;

#define classNumber arrayLength

typedef struct	{
	u4	pos:16;			// on heap (reataive to base) or for CP-string cN refbyte in CP
	u4	type:4; // 0-normal object, 1- normal staticobj, 2-normalarray 3- string on heap 3-string in cp on heap or in CP (String) or array on heap
	u4 classNumber:8;	// or arrayLength for arrayobjects
	u4 magic:4;
} stackObjectInfo; 

typedef union	{
	u4 UInt;
	s4 Int;
	f4 Float;
	u1 bytes[4];
	u2 characters[2];
	stackObjectInfo stackObj;
	heapObjectMarker heapObjMarker;
}  slot;
/*
typedef struct	{
u1	numEntries;
//u2 occupiedFromObject;
u1	threadNumber[MAXTHREADSINSET];
} threadSet;
*/
typedef struct	ThreadControlBlock {
slot*	opStackBase;
u2*	methodStackBase;	
u2		methodSpPos;	// relative to methodStackBase
u1		tid;
u1		priority;
u1		numTicks;
u1		status;		// NOTBLOCKED, MUTEXBLOCKED, WAITBLOCKED, WAITAWEAKENED
struct ThreadControlBlock* pred;
struct ThreadControlBlock* succ;
//u2		isMutexBlockedForObject;		// wird nicht geschedult
//u2		isWaitingForObject; // vielleicht überflüssig
slot		isMutexBlockedOrWaitingForObject;
slot		hasMutexLockForObject[MAXLOCKEDTHREADOBJECTS];
u1			lockCount[MAXLOCKEDTHREADOBJECTS]; // objectNumber, counter
} 	ThreadControlBlock;

#define methodStartPC methodInfoStart+6


#define	MAGIC(cN)							((u2) *cs[cN].classFileStartAddress)
#define	CONSTANTPOOLCOUNT(cN)	((u2) *cs[cN].classFileStartAddress+8)

typedef struct 	{
		u1* classFileStartAddress;		// Speicheradresse fuer classfile-bytes
		u2  classFileLength;				// << 64K
		
		u4 magic;							// kann weg
		u2 minor_version;				// kann weg
		u2 major_version;				// kann weg
		u2 constant_pool_count;		// kann weg
		u2* constant_pool;				//cp_info constant_pool[constant_pool_count-1];
		u2 access_flags;
		u2 this_class;					// kann weg
		u2 super_class;					// kann weg
		u2 interfaces_count;				// kann weg
		u2* interfaces;					//u2 interfaces[interfaces_count];
		u2 fields_count;
		u2* field_info;					// field_info fields[fields_count];
		// nur ConstantValue attribute
		u2 methods_count;
		u2* method_info;				//method_info methods[methods_count];
		u2 attributes_count;
		u2* attribute_info;			//attribute_info attributes[attributes_count];
		slot classInfo;
//		u2	classObjectPos;				// pos des evtl. class objects on heap
	} 		classStructure ;
#endif
