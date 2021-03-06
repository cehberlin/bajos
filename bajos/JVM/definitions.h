/*
 * HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
 * See the file "license.terms" for information on usage and redistribution of this file.
 */

#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#ifdef AVR8
#include <avr/pgmspace.h>
#endif

#include "typedefinitions.h"
// RAMPZ ever 1 -> therefore 0x8000+0x3000=0xb000 (words) java class base
// 0x10000 + 0x06000 in bytes
// bamo128 -> 0xF000 to 0xFFFF in words
#define AVR8_FLASH_JAVA_BASE    0x0000            // +rampz;16k boot classes in flash	-> 0x8000 in words
#define AVR8_FLASH_APP_BASE     0xcc00            // +rampz;6k application classes	-> 0xe600
// UC3A is evk1100
#define UC3A_FLASH_BASE     0x80000000            // internal 512 KB flash
//#define UC3A_FLASH_JAVA_BASE	0x80040000	// boot classes in flash
#define UC3A_FLASH_JAVA_BASE    0x80038000        // boot classes in flash
#define UC3A_SDRAM_BASE     0xD0000000
#define UC3A_SDRAM_JAVA_BASE    0xD0200000        // application classes, heap, opstack methodstack
#define STK1000_FLASH_BASE  0                     // external 8 MB flash
#define STK1000_FLASH_JAVA_BASE 0x00040000        // boot classes in flash
#define STK1000_SDRAM_BASE  0x10000000            // external SDRAM 8MB
#define STK1000_SDRAM_JAVA_BASE (STK1000_SDRAM_BASE+0x00600000)

#define STK1000_SDRAM_LCD_BASE  STK1000_SDRAM_BASE

#define NGW_FLASH_BASE      0                     // external 8 MB flash
#define NGW_FLASH_JAVA_BASE 0x00040000            // boot classes in flash
#define NGW_SDRAM_BASE      0x10000000            // external SDRAM
#define NGW_SDRAM_JAVA_BASE     (NGW_SDRAM_BASE+0x00600000)

#define HEAPFREESPACE           0
#define HEAPALLOCATEDSTATICCLASSOBJECT  1
#define HEAPALLOCATEDNEWOBJECT      2
#define HEAPALLOCATEDARRAY      3
#define OBJECTMAGIC         0xA
#define CPSTRINGMAGIC           (OBJECTMAGIC+1)
#define MUTEXNOTBLOCKED         1
#define MUTEXBLOCKED            0

#define THREADNOTBLOCKED        0
#define THREADMUTEXBLOCKED      1
#define THREADWAITBLOCKED       2
#define THREADWAITAWAKENED      3
#define GARBAGEFUSIONTRIALS     20

#ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
    #ifdef TINYBAJOS
	#ifndef TINYBAJOS_PRINTF
	    #define	PRINTF(format, ...)
	#else
	    #define PRINTF(format, ...) printf_P(PSTR(format),  ## __VA_ARGS__)
	#endif
    #else
	#define PRINTF(format, ...) printf_P(PSTR(format),  ## __VA_ARGS__)
    #endif
#else
    #define PRINTF(format, ...) printf(format,  ## __VA_ARGS__)
#endif

#ifdef DEBUG
    #define DEBUGPRINTF(format, ...) PRINTF(format, ## __VA_ARGS__)
#else
    #define DEBUGPRINTF(format, ...)
#endif

#ifdef TINYBAJOS_ERROREXIT
    #define ERROREXIT(nr, format, ...) exit((nr))
#else
    #ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
	#define ERROREXIT(nr, format, ...) errorExitFunction ((nr), PSTR((format)),  ## __VA_ARGS__)
    #else
	#define ERROREXIT(nr, format, ...) errorExitFunction ((nr), (format),  ## __VA_ARGS__)
    #endif
#endif

#ifdef AVR8
    #define STRNCMPRAMFLASH strncmpRamFlash
    #define STRNCMPFLASHFLASH strncmpFlashFlash
#else
    #define STRNCMPRAMFLASH strncmp
    #define STRNCMPFLASHFLASH strncmp
#endif



#ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
    #define avr8Printf( format, ...) printf_P (PSTR((format)),  ## __VA_ARGS__)
#else
    #define avr8Printf( format, ...) printf ((format),  ## __VA_ARGS__)
#endif

#ifndef TINYBAJOS_ERROREXIT
#ifdef AVR8                                       // change all avr8 string to flash strings gives more data ram space for java!!
#define errorExit(nr, format, ...) errorExitFunction ((nr), PSTR((format)),  ## __VA_ARGS__)
#else
#define errorExit(nr, format, ...) errorExitFunction ((nr), (format),  ## __VA_ARGS__)
#endif
#else
#define errorExit(nr, format, ...) exit((nr))
#endif

#ifndef TINYBAJOS_PRINTF
#define verbosePrintf( format, ...) avr8Printf ((format),  ## __VA_ARGS__)
#else
#define verbosePrintf( format, ...)
#endif

#if (AVR32UC3A || AVR32AP7000)
#define NULLOBJECT          ((slot) (0xfffff000 |  ((u4)OBJECTMAGIC<<8)|((u4)OBJECTMAGIC<<4)|((u4)OBJECTMAGIC)))
#else
#define NULLOBJECT          ((slot) (0x000fffff |  ((u4)OBJECTMAGIC<<28)|((u4)OBJECTMAGIC<<24)|((u4)OBJECTMAGIC<<20)))
#endif

//mb jf
#ifndef TINYBAJOS_EXCEPTION
#define ARRAYINDEXOUTOFBOUNDSEXCEPTION raiseExceptionFromIdentifier("java/lang/ArrayIndexOutOfBoundsException", 40)
#define NEGATIVEARRAYSIZEEXCEPTION raiseExceptionFromIdentifier("java/lang/NegativeArraySizeException", 36)
#define NULLPOINTEREXCEPTION raiseExceptionFromIdentifier("java/lang/NullPointerException", 30)
#define ARITHMETICEXCEPTION raiseExceptionFromIdentifier("java/lang/ArithmeticException", 29)
#define CLASSCASTEXCEPTION raiseExceptionFromIdentifier("java/lang/ClassCastException", 28)
#define ILLEGALMONITORSTATEEXCEPTION raiseExceptionFromIdentifier("java/lang/IllegalMonitorStateException", 38)

#define DNOTSUPPORTED errorExit(-2, "Double precision primitive data types (double and long) are not supported.\n")
#define CLASSNOTFOUNDERR(classname,classnamelength) {errorExit(-3, "Class '%s' not found.\n", classname);}
#define UNHANDLEDEXCEPTIONERR(exceptionname) errorExit(-4, "Unhandled exception of type '%s'.\n", exceptionname)
#define FIELDNOTFOUNDERR(fieldname, classname) errorExit(-5, "Field '%s' in class '%s' not found.\n", fieldname, classname)
#define METHODNOTFOUNDERR(methodname, classname) errorExit(-6, "Method '%s' in class '%s' not found.\n", methodname, classname)
#define MALLOCERR(count, target) errorExit(-7, "Malloc error while trying to allocate %d bytes for %s\n", count, target)
#else
#define ARRAYINDEXOUTOFBOUNDSEXCEPTION errorExit(-8, "ARRAYINDEXOUTOFBOUNDS\n")
#define NEGATIVEARRAYSIZEEXCEPTION errorExit(-2, "NEGATIVEARRAYSIZE\n")
#define NULLPOINTEREXCEPTION errorExit(-2, "NULLPOINTER\n")
#define ARITHMETICEXCEPTION errorExit(-2, "ARITHMETIC\n")
#define CLASSCASTEXCEPTION errorExit(-2, "CLASSCAST\n")
#define ILLEGALMONITORSTATEEXCEPTION  errorExit(-2, "ILLEGALMONITORSTATE\n")
#define DNOTSUPPORTED errorExit(-2, "Double precision primitive data types (double and long) are not supported.\n")
#define CLASSNOTFOUNDERR(classname,classnamelength) {errorExit(-3, "Class '%s' not found.\n", classname);}
#define UNHANDLEDEXCEPTIONERR(exceptionname) errorExit(-4, "Unhandled exception of type '%s'.\n", exceptionname)
#define FIELDNOTFOUNDERR(fieldname, classname) errorExit(-5, "Field '%s' in class '%s' not found.\n", fieldname, classname)
#define METHODNOTFOUNDERR(methodname, classname) errorExit(-6, "Method '%s' in class '%s' not found.\n", methodname, classname)
#define MALLOCERR(count, target) errorExit(-7, "Malloc error while trying to allocate %d bytes for %s\n", count, target)
#endif

#define CASE                break;case
#define DEFAULT             break;default
#define CLASSSTA            (cs[cN].classFileStartAddress)
                                                  // constantpool entry n of class m
#define CP(m,n)             (cs[m].constant_pool[n])
#define BYTECODEREF         ((byte1 << 8) | byte2)
#define HEAPOBJECTMARKER(pos)       ((*(heapBase+(pos))).heapObjMarker)
#define HEAPOBJECT(pos)         (*(heapBase+pos))

#define METHODBASE(cN,mN)       (cs[cN].method_info[mN])
#define METHODACCESSFLAG(cN,mN)     getU2(METHODBASE(cN,mN))
#define METHODNAMESTR(cN,mN)        getAddr(CP(cN,getU2(METHODBASE(cN,mN)+2))  +3)
#define METHODNAMESTRLENGTH(cN,mN)  getU2(CP(cN,getU2(METHODBASE(cN,mN)+2))  +1)
#define METHODDESCRSTR(cN,mN)       getAddr(CP(cN,getU2(METHODBASE(cN,mN)+4))  +3)
#define METHODDESCRSTRLENGTH(cN,mN) getU2(CP(cN,getU2(METHODBASE(cN,mN)+4))  +1)
                                                  // at length(u2)
#define METHODATTRIBUTEBASE(cN,mN)  (METHODBASE(cN,mN)+6)
#define FIELDBASE(cN,fN)        (cs[cN].field_info[fN])

// change pc
#define METHODCODEBASE(cN,mN)       (getStartPC()-14)
                                                  //start at length(u2)
#define METHODCODEEXCEPTIONBASE(cN,mN)  (getStartPC()+getU4(getStartPC()-4))
#define METHODEXCEPTIONBASE(cN,mN)  (cs[cN].method_info[mN,3])

#define   CONSTANT_Class                    7
#define   CONSTANT_Fieldref                 9
#define   CONSTANT_Methodref                10
#define   CONSTANT_InterfaceMethodref   11
#define   CONSTANT_String                   8
#define   CONSTANT_Integer                  3
#define   CONSTANT_Float                    4
#define   CONSTANT_Long                 5
#define   CONSTANT_Double                   6
#define   CONSTANT_NameAndType          12
#define   CONSTANT_Utf8                     1

#define     ACC_PUBLIC              0x0001        //Declared public; may be accessed from outside its package.
#define     ACC_PRIVATE         0x0002            //Declared private; accessible only within the defining class.
#define     ACC_PROTECTED   0x0004                //Declared protected; may be accessed within subclasses.
#define     ACC_STATIC              0x0008        //Declared static.
#define     ACC_FINAL               0x0010        //Declared final; may not be overridden.
#define     ACC_SYNCHRONIZED 0x0020               //Declared synchronized; invocation is wrapped in a monitor lock.
#define     ACC_NATIVE              0x0100        //Declared native; implemented in a language other than Java.
#define     ACC_ABSTRACT        0x0400            //Declared abstract; no implementation is provided.
#define     ACC_STRICT              0x0800        //Declared strictfp; floating-point mode is FP-strict
//mb jf
#define     T_BOOLEAN           0x4               // aaray type boolean
#define     T_CHAR              0x5               // aaray type boolean
#define     T_FLOAT             0x6               // aaray type boolean
#define     T_DOUBLE            0x7               // aaray type boolean
#define     T_BYTE              0x8               // aaray type boolean
#define     T_SHORT             0x9               // aaray type boolean
#define     T_INT                   0xa           // aaray type boolean
#define     T_LONG              0xb               // aaray type boolean
#ifdef DEBUG
#define OUTSTREAM stdout

#define DEBUGPRINT(format, ...) avr8Printf((format),  ## __VA_ARGS__)
#define DEBUGPRINTLN(format, ...) avr8Printf((format),  ## __VA_ARGS__); avr8Printf("\n");

#define DEBUGPRINTE(x,f)    avr8Printf(#x ": " "%"#f" ",x)
#define PRINTE(x,f)  avr8Printf(#x ": " "%"#f" ",x)
#define DEBUGPRINTSTACK {\
    int i;\
    for (i= opStackGetSpPos() > 8 ? -8 : -opStackGetSpPos() ; i < 0 ; i++) \
    { \
        avr8Printf("%08x ",((opStackGetValue(((opStackGetSpPos()+i)<0)?0:(opStackGetSpPos()+i)))).UInt);\
    } \
    ; avr8Printf(":|_|stack\n"); \
}
/*
 VT102Attribute('0', COLOR_WHITE);avr8Printf(" ");\ 
                if (i==(opStackGetSpPos()))VT102Attribute ('4', COLOR_GREEN); else VT102Attribute('0', COLOR_WHITE);\ 
*/
#define DEBUGPRINTLOCALS {\
    u1 i;\
    avr8Printf("|.| local:");\
    for (i=0; i < 8 && i < local; i++)\
    avr8Printf(" %08x",opStackGetValue(local+i).UInt);\
    avr8Printf("\n"); \
}

#define DEBUGPRINTHEAP
/*{\ 
            u2 i,j;\ 
            avr8Printf("|#|  heap:\n");\ 
for (j=0; j <33; j+=8){\ 
for (i=0; i < 8; i++)\ 
avr8Printf(" %8x",(*(heapBase+i+j)).UInt);avr8Printf("\n");}\ 
} avr8Printf("\n")
*/

#define DEBUGPRINTSTRING(p,l) {\
    u2 _i;\
    for (_i=0; _i < (l); _i++)\
    avr8Printf("%c",*((u1*)(p)+_i));\
    avr8Printf(" "); \
}

#define DEBUGPRINTLNSTRING(p,l) {DEBUGPRINTSTRING(p,l); avr8Printf("\n");}
#else

#define DEBUGPRINT(format, ...)
#define DEBUGPRINTLN(format, ...)
#define DEBUGPRINTE(x,f)
#define DEBUGPRINTSTACK
#define DEBUGPRINTHEAP
#define DEBUGPRINTLOCALS
#define DEBUGPRINTSTRING(p,l)
#define DEBUGPRINTLNSTRING(p,l)
#define OUTSTREAM stderr
#endif

#define PRINT(format, ...)
#define PRINTLN(format, ...)

#define PRINTEXITTHREAD(a,b) {avr8Printf(a,b);\
    if (numThreads==1) \
    { \
        avr8Printf("Bajos terminated\n"); \
        exit(0); \
    }\
    else {  deleteThread();} \
}

#define  COLOR_BLACK     0                        /* VT102 Color Codes                                  */
#define  COLOR_RED       1
#define  COLOR_GREEN     2
#define  COLOR_BROWN     3
#define  COLOR_BLUE      4
#define  COLOR_MAGENTA   5
#define  COLOR_CYAN      6
#define  COLOR_WHITE     7

#ifdef AM
#define NUMBEROFINTERRUPTS  57                    //SIZE of interruptvectorarray
#else
#define NUMBEROFINTERRUPTS  1
#endif

//end of file
#endif
