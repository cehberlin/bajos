/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fhw/ba-berlin ein java interpreter fuer atmega128
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#include "typedefinitions.h"	
	#define	HEAPFREESPACE								0
	#define	HEAPALLOCATEDSTATICCLASSOBJECT	1
	#define	HEAPALLOCATEDNEWOBJECT				2
	#define	HEAPALLOCATEDARRAY						3
	
	#define	STACKNEWOBJECT						1
	#define	STACKSTATICCLASSOBJECT			2
	#define	STACKNEWARRAYOBJECT				3
	#define 	STACKANEWARRAYOBJECT			4
	#define	STACKCPSTRING							5
	#define	STACKNEWSTRINGOBJECT			6
	
	#define OBJECTMAGIC				0xA	
	
	#define MUTEXNOTBLOCKED			1
	#define MUTEXBLOCKED				0

	#define THREADNOTBLOCKED		0
	#define THREADMUTEXBLOCKED		1
	#define THREADWAITBLOCKED		2
	#define THREADWAITAWAKENED		3
#if (AVR32UC3A || AVR32AP7000)
#define NULLOBJECT				((slot) (0xfffff000 |  ((u4)OBJECTMAGIC<<8)|((u4)OBJECTMAGIC<<4)|((u4)OBJECTMAGIC)))
#else
	#define NULLOBJECT				((slot) (0x000fffff |  ((u4)OBJECTMAGIC<<28)|((u4)OBJECTMAGIC<<24)|((u4)OBJECTMAGIC<<20)))
#endif
	#define NULLTHREAD						MAXTHREADS-1
	#define NULLCLASS						0xff	

//mb jf
#define ARRAYINDEXOUTOFBOUNDEXCEPTION	PRINTEXIT("ArrayIndexOutOfBoundException %d\n",0)
#define NEGATIVEARRAYSIZEEXCEPTION	PRINTEXIT("NegativeArraySizeException %d\n",0)
#define NULLPOINTEREXCEPTION		 PRINTEXIT("NullPointerException %d\n",0)
#define ARITHMETHICEXCEPTION		PRINTEXIT("ArithmeticException Division by Zero ->  delete Thread: %d\n",actualThreadCB->tid)

	#define PRINTSTRING(p,l) {\
			int _i;\
			for (_i=0; _i < (l); _i++)\
				printf("%c",*((u1*)(p)+_i));\
		} printf("\n")

	#define CASE								break;case
	#define DEFAULT							break;default
	
	#define CLASSSTA							(cs[cN].classFileStartAddress)
	#define CP(m,n)								(cs[m].constant_pool[n]) 			// constantpool entry n of class m
	#define BYTECODEREF					((byte1 << 8) | byte2)

	#define HEAPOBJECTMARKER(pos)	((*(heapBase+(pos))).heapObjMarker)
	#define HEAPOBJECT(pos)				(*(heapBase+pos))


#define METHODBASE(cN,mN)							(cs[cN].method_info[mN])
#define METHODACCESSFLAG(cN,mN)				getU2(METHODBASE(cN,mN))
#define METHODNAMESTR(cN,mN)					getAddr(CP(cN,getU2(METHODBASE(cN,mN)+2))  +3)
#define METHODNAMESTRLENGTH(cN,mN)			getU2(CP(cN,getU2(METHODBASE(cN,mN)+2))  +1)
#define METHODDESCRSTR(cN,mN)					getAddr(CP(cN,getU2(METHODBASE(cN,mN)+4))  +3)
#define METHODDESCRSTRLENGTH(cN,mN)		getU2(CP(cN,getU2(METHODBASE(cN,mN)+4))  +1)
#define METHODATTRIBUTEBASE(cN,mN) 			(METHODBASE(cN,mN)+6) 						// at length(u2)


// change pc
#define METHODCODEBASE(cN,mN)					(getStartPC(0)-14)
#define METHODCODEEXCEPTIONBASE(cN,mN)	(getStartPC(0)+getU4(getStartPC(0)-4)) //start at length(u2)
#define METHODEXCEPTIONBASE(cN,mN)			(cs[cN].method_info[mN,3])

	
	
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
				for (i=-8; i < 0; i++){\
				printf("%08x ",((opStackGetValue(((opStackGetSpPos()+i)<0)?0:(opStackGetSpPos()+i)))).UInt);\
		} ;	printf(":|_|stack\n");}
/*
				VT102Attribute('0', COLOR_WHITE);printf(" ");\
				if (i==(opStackGetSpPos()))VT102Attribute ('4', COLOR_GREEN); else VT102Attribute('0', COLOR_WHITE);\
*/
		#define DEBUGPRINTLOCALS if (TR==actualThreadCB->tid){\
				int i;\
				printf("|.| local:");\
				for (i=0; i < 8; i++)\
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
		#define DEBUGPRINTSTRING
		#define DEBUGPRINTLNSTRING
		#define OUTSTREAM stderr
	#endif
#define PRINT1(f)				printf(f)
#define PRINT2(f,a)				printf(f,a)
#define PRINT3(f,a,b)			printf(f,a,b)
#define PRINTLN1(f)				printf(f)
#define PRINTLN2(f,a)				printf(f,a)
#define PRINTLN3(f,a,b)			printf(f,a,b)


#define PRINTSEXIT(s1,s2,n)	{	printf(s1"%s",s2);\
										exit(n);	}
#define PRINT1EXIT(f,a,n)		{	printf(f,a);\
										printf("tschüss\n");\
										exit(n);	}

#define PRINTEXIT(f,a) 		{	printf(f,a);\
										printf("tschüss\n");\
										exit(-1);}
										
#define PRINTEXITTHREAD(a,b) {printf(a,b);\
									 if (numThreads==1)	{\
										printf("tschüssi\n");\
										exit(0);}\
									else {	deleteThread();}\
									}


#define  COLOR_BLACK     0                       /* VT102 Color Codes                                  */
#define  COLOR_RED       1
#define  COLOR_GREEN     2
#define  COLOR_BROWN     3
#define  COLOR_BLUE      4
#define  COLOR_MAGENTA   5
#define  COLOR_CYAN      6
#define  COLOR_WHITE     7

#endif
