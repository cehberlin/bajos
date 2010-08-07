/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...

#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include "typedefinitions.h"
// UC3A is evk1100
#define UC3A_FLASH_BASE		0x80000000	// internal 512 KB flash
#define UC3A_FLASH_JAVA_BASE	0x80040000	// boot classes in flash
#define UC3A_SDRAM_JAVA_BASE	0xD0000000	// application classes, heap, opstack methodstack
#define STK1000_FLASH_BASE	0		// external 8 MB flash
#define STK1000_FLASH_JAVA_BASE	0x00040000	// boot classes in flash
#define STK1000_SDRAM_BASE	0x10000000	// external SDRAM 8MB
#define STK1000_SDRAM_JAVA_BASE (STK1000_SDRAM_BASE+0x00600000)
#define STK1000_SDRAM_LCD_BASE	STK1000_SDRAM_BASE
#define NGW_FLASH_BASE		0		// external 8 MB flash
#define NGW_FLASH_JAVA_BASE	0x00040000	// boot classes in flash
#define NGW_SDRAM_BASE 		0x10000000	// external SDRAM

#define	HEAPFREESPACE			0
#define	HEAPALLOCATEDSTATICCLASSOBJECT	1
#define	HEAPALLOCATEDNEWOBJECT		2
#define	HEAPALLOCATEDARRAY		3
#define OBJECTMAGIC			0xA	
#define CPSTRINGMAGIC 			(OBJECTMAGIC+1)
#define MUTEXNOTBLOCKED			1
#define MUTEXBLOCKED			0

#define THREADNOTBLOCKED		0
#define THREADMUTEXBLOCKED		1
#define THREADWAITBLOCKED		2
#define THREADWAITAWAKENED		3
#define GARBAGEFUSIONTRIALS		20


#if (AVR32UC3A || AVR32AP7000)
#define NULLOBJECT			((slot) (0xfffff000 |  ((u4)OBJECTMAGIC<<8)|((u4)OBJECTMAGIC<<4)|((u4)OBJECTMAGIC)))
#else
#define NULLOBJECT			((slot) (0x000fffff |  ((u4)OBJECTMAGIC<<28)|((u4)OBJECTMAGIC<<24)|((u4)OBJECTMAGIC<<20)))
#endif

//mb jf
#define ARRAYINDEXOUTOFBOUNDSEXCEPTION raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 40)
#define NEGATIVEARRAYSIZEEXCEPTION raiseExceptionFromIdentifier("java/lang/NegativeArraySizeException", 36)
#define NULLPOINTEREXCEPTION raiseExceptionFromIdentifier("java/lang/NullPointerException", 30)
#define ARITHMETICEXCEPTION raiseExceptionFromIdentifier("java/lang/ArithmeticException", 29)
#define CLASSCASTEXCEPTION raiseExceptionFromIdentifier("java/lang/ClassCastException", 28)
#define ILLEGALMONITORSTATEEXCEPTION raiseExceptionFromIdentifier("java/lang/IllegalMonitorStateException", 38)

#define DNOTSUPPORTED errorExit(-2, "Double precision primitive data types (double and long) are not supported.\n")
#define CLASSNOTFOUNDERR(classname) errorExit(-3, "Class '%s' not found.\n", classname)
#define UNHANDLEDEXCEPTIONERR(exceptionname) errorExit(-4, "Unhandled exception of type '%s'.\n", exceptionname)
#define FIELDNOTFOUNDERR(fieldname, classname) errorExit(-5, "Field '%s' in class '%s' not found.\n", fieldname, classname)
#define METHODNOTFOUNDERR(methodname, classname) errorExit(-6, "Method '%s' in class '%s' not found.\n", methodname, classname)
#define MALLOCERR(count, target) errorExit(-7, "Malloc error while trying to allocate %d bytes for %s\n", count, target)

#define PRINTSTRING(p,l) {\
			int _i;\
			for (_i=0; _i < (l); _i++)\
			printf("%c",*((u1*)(p)+_i));\
			} printf("\n")

#define CASE				break;case
#define DEFAULT				break;default
#define CLASSSTA			(cs[cN].classFileStartAddress)
#define CP(m,n)				(cs[m].constant_pool[n]) // constantpool entry n of class m
#define BYTECODEREF			((byte1 << 8) | byte2)
#define HEAPOBJECTMARKER(pos)		((*(heapBase+(pos))).heapObjMarker)
#define HEAPOBJECT(pos)			(*(heapBase+pos))



#define METHODBASE(cN,mN)		(cs[cN].method_info[mN])
#define METHODACCESSFLAG(cN,mN)		getU2(METHODBASE(cN,mN))
#define METHODNAMESTR(cN,mN)		getAddr(CP(cN,getU2(METHODBASE(cN,mN)+2))  +3)
#define METHODNAMESTRLENGTH(cN,mN)	getU2(CP(cN,getU2(METHODBASE(cN,mN)+2))  +1)
#define METHODDESCRSTR(cN,mN)		getAddr(CP(cN,getU2(METHODBASE(cN,mN)+4))  +3)
#define METHODDESCRSTRLENGTH(cN,mN)	getU2(CP(cN,getU2(METHODBASE(cN,mN)+4))  +1)
#define METHODATTRIBUTEBASE(cN,mN) 	(METHODBASE(cN,mN)+6) 		// at length(u2)

#define FIELDBASE(cN,fN)		(cs[cN].field_info[fN])

// change pc
#define METHODCODEBASE(cN,mN)		(getStartPC(0)-14)
#define METHODCODEEXCEPTIONBASE(cN,mN)	(getStartPC(0)+getU4(getStartPC(0)-4)) //start at length(u2)
#define METHODEXCEPTIONBASE(cN,mN)	(cs[cN].method_info[mN,3])

#define   CONSTANT_Class    				7 
#define   CONSTANT_Fieldref    				9 
#define   CONSTANT_Methodref    			10 
#define   CONSTANT_InterfaceMethodref	11 
#define   CONSTANT_String    				8 
#define   CONSTANT_Integer    				3 
#define   CONSTANT_Float    				4 
#define   CONSTANT_Long    				5 
#define   CONSTANT_Double    				6 
#define   CONSTANT_NameAndType  		12 
#define   CONSTANT_Utf8    					1

#define		ACC_PUBLIC    			0x0001    //Declared public; may be accessed from outside its package. 
#define		ACC_PRIVATE    		0x0002    //Declared private; accessible only within the defining class. 
#define		ACC_PROTECTED 	0x0004    //Declared protected; may be accessed within subclasses. 
#define		ACC_STATIC    			0x0008    //Declared static. 
#define		ACC_FINAL    			0x0010    //Declared final; may not be overridden. 
#define		ACC_SYNCHRONIZED 0x0020    //Declared synchronized; invocation is wrapped in a monitor lock. 
#define		ACC_NATIVE    			0x0100    //Declared native; implemented in a language other than Java. 
#define		ACC_ABSTRACT    	0x0400    //Declared abstract; no implementation is provided. 
#define		ACC_STRICT    			0x0800    //Declared strictfp; floating-point mode is FP-strict 
	
	//mb jf
#define		T_BOOLEAN    		0x4    // aaray type boolean
#define		T_CHAR    			0x5    // aaray type boolean
#define		T_FLOAT    			0x6    // aaray type boolean
#define		T_DOUBLE    		0x7    // aaray type boolean
#define		T_BYTE    			0x8    // aaray type boolean
#define		T_SHORT    			0x9    // aaray type boolean
#define		T_INT    				0xa    // aaray type boolean
#define		T_LONG    			0xb    // aaray type boolean		

#ifdef DEBUG
#define TR 0 //(actualThreadCB->tid)
		#define OUTSTREAM stdout
		#define	DEBUGPRINT1(s)	if (TR==actualThreadCB->tid)printf(s)
		#define	DEBUGPRINT2(f,a) if (TR==actualThreadCB->tid) printf(f,a)
		#define	DEBUGPRINT3(f,a,b) if (TR==actualThreadCB->tid) printf(f,a,b)
		#define	DEBUGPRINTLN1(s) if (TR==actualThreadCB->tid) {printf(s); printf("\n");}
		#define	DEBUGPRINTLN2(f,a) if (TR==actualThreadCB->tid) {printf(f,a); printf("\n");}
		#define	DEBUGPRINTLN3(f,a,b) if (TR==actualThreadCB->tid) {printf(f,a,b); printf("\n");}
		#define 	DEBUGPRINTE(x,f)	if (TR==actualThreadCB->tid) printf(#x ": " "%"#f" ",x)
		#define 	PRINTE(x,f)	if (TR==actualThreadCB->tid) printf(#x ": " "%"#f" ",x)
		#define DEBUGPRINTF(a,b,c)	if (TR==actualThreadCB->tid)printf(a,b,c)
		#define DEBUGPRINTSTACK if (TR==actualThreadCB->tid){\
				int i;\
				for (i= opStackGetSpPos() > 8 ? -8 : -opStackGetSpPos() ; i < 0 ; i++){\
				printf("%08x ",((opStackGetValue(((opStackGetSpPos()+i)<0)?0:(opStackGetSpPos()+i)))).UInt);\
		} ;	printf(":|_|stack\n");}
/*
				VT102Attribute('0', COLOR_WHITE);printf(" ");\
				if (i==(opStackGetSpPos()))VT102Attribute ('4', COLOR_GREEN); else VT102Attribute('0', COLOR_WHITE);\
*/
		#define DEBUGPRINTLOCALS if (TR==actualThreadCB->tid){\
				int i;\
				printf("|.| local:");\
				for (i=0; i < 8 && i < local; i++)\
				printf(" %08x",opStackGetValue(local+i).UInt);\
		 printf("\n");}
		
#define DEBUGPRINTHEAP 
/*{\
			int i,j;\
			printf("|#|  heap:\n");\
			for (j=0; j <33; j+=8){\
			for (i=0; i < 8; i++)\
				printf(" %8x",(*(heapBase+i+j)).UInt);printf("\n");}\
		} printf("\n")
*/
		
#define DEBUGPRINTSTRING(p,l) {\
			int _i;\
			for (_i=0; _i < (l); _i++)\
			printf("%c",*((u1*)(p)+_i));\
			 printf(" ");}

#define DEBUGPRINTLNSTRING(p,l)	if (TR==actualThreadCB->tid){DEBUGPRINTSTRING(p,l); printf("\n");}

#else

#define	DEBUGPRINT1(s)	
#define	DEBUGPRINT2(f,a)
#define	DEBUGPRINT3(f,a,b)
#define	DEBUGPRINTLN1(s)
#define	DEBUGPRINTLN2(f,a)
#define	DEBUGPRINTLN3(f,a,b)
#define	DEBUGPRINTE(x,f)
#define DEBUGPRINTF(a,b)
#define DEBUGPRINTSTACK
#define DEBUGPRINTHEAP
#define DEBUGPRINTLOCALS
#define DEBUGPRINTSTRING(p,l)
#define DEBUGPRINTLNSTRING(p,l)
#define OUTSTREAM stderr
#endif

#define PRINT1(f)		printf(f)
#define PRINT2(f,a)		printf(f,a)
#define PRINT3(f,a,b)		printf(f,a,b)
#define PRINTLN1(f)		printf(f)
#define PRINTLN2(f,a)		printf(f,a)
#define PRINTLN3(f,a,b)		printf(f,a,b)
#ifdef AVR8
#define PRINTEXITTHREAD(a,b) {printf(a,b);\
				if (numThreads==1)	{\
				printf("tschüssi\n");\
				goto *0xf002; \
				/*exit(0);*/}\
				else {	deleteThread();}\
							}
#else
#define PRINTEXITTHREAD(a,b) {printf(a,b);\
				if (numThreads==1)	{\
				printf("tschüssi\n");\
				exit(0);}\
				else {	deleteThread();}\
							}
#endif
#define  COLOR_BLACK     0     /* VT102 Color Codes                                  */
#define  COLOR_RED       1
#define  COLOR_GREEN     2
#define  COLOR_BROWN     3
#define  COLOR_BLUE      4
#define  COLOR_MAGENTA   5
#define  COLOR_CYAN      6
#define  COLOR_WHITE     7

#endif
