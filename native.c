/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
// remember:
// invokespecial Operand Stack
// ..., objectref, [arg1, [arg2 ...]] -> ...
// invokestatic: Operand Stack
// ..., [arg1, [arg2 ...]] -> ...

#include <stdio.h>
#include <stdlib.h>
#if LINUX||AVR32LINUX
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#endif
#include <string.h>
#include "definitions.h"
#include "bajvm.h"
#include "stack.h"
#include "classfile.h"
#include "scheduler.h"
#include "heap.h"
#include "native.h"
#if (AVR32UC3A||AVR32AP7000)
#include "EVK1100/gpiouc3a.h"
#include "EVK1100/evk1100.h"
#include "EVK1100/dip204.h"
#include "EVK1100/countuc3a.h"
#include "EVK1100/rtcuc3a.h"
#include "EVK1100/intcuc3a.h"
#include "iobinding.h"
#endif 
#ifdef AVR8
#include "AVR8/lcd.h"
#include <avr/interrupt.h>
#endif
#ifdef STK1000
#include "STK1000/lib2d.h"
#include "STK1000/bmplib.h"
#include "STK1000/fontlib.h"
#endif
// all native methods have a return value
// 0-> is a Java native void function
// 1-> is an Java native function with anyone return value (in stack)
// native methods have an unique name!!
// fill this arrays
const char*	nativePlatForm[]={
		"platform/PlatForm",
		"nativeCharOut",
		"nativeCharIn",
		"getButtons",
		"setOnBoardLEDs",
		"charLCDOut", 
		"controlLCD",
		"currentTimeMillis",
		"exit",
		"getTemperature",
#ifndef AVR8
		"drawPointRGB",
		"drawFillRectRGB",
		"drawRectangleRGB",
		"clearScreenRGB",
		"drawRoundRectRGB",
		"drawEllipseRGB",
		"drawCircleRGB",
		"drawLineRGB",
		"drawPointHSB",
		"drawLineHSB",
		"drawTriangleFill",
		"clearZBuffer",
		"drawPointHSBZBuffer",
		"drawLineHSBZBuffer",
		"setFont",
		"setFontWindow",
		"setFontAutoLineFeed",
		"setFontFixedFont",
		"setFontCursor",
		"setFontColor",
		"drawChar",
		"drawCharAt",
		"getCharWidth",
#endif
NULL};

const char*	nativeString[]={"java/lang/String",
		"nativeCharAt",
		"nativeStringLength",NULL};

const char*	nativeThread[]={"java/lang/Thread",
		"start",
		"yield",
		"sleep",
		"currentThread",
		"getPriority",
		"setPriority",
		"interrupt",
		"interrupted",
		"isInterrupted",
		"isDaemon",
		"setDaemon",
		"join",NULL};

const char*	nativeObject[]={"java/lang/Object",
		"notify",
		"notifyAll",
		"wait",
		"waitTime",
		"getDataAddress",NULL};

const char*	nativeFloat[]={"java/lang/Float",
		"nativeParseFloat",
		"floatToIntBits",
		"intBitsToFloat",
		"floatToCharArray",NULL};

// fill this array
const char**	nativeNames[]={	
			nativePlatForm,
			nativeString,
			nativeThread,
			nativeObject,
			nativeFloat	};

const char**	nativeName;
u2	numNativeClasses=sizeof(nativeNames)/sizeof(*nativeNames);
u2	numMethods=0;
u1*	nativeCNMN;
u2	numEntryNativeCNMN=MAXNATIVEMETHODS;
u2 	i;

// count the native methods in platform
#ifdef AVR8
#define NUMNATIVPLATFORMMETHODS	9
#else
#define NUMNATIVPLATFORMMETHODS	32
#endif

char	nativeDispatch()	{
for(i=0;i<numMethods;i++)	
	if ((((u2)*(nativeCNMN+2*i)<<8)+(*(nativeCNMN+2*i+1)))==(((u2)(cN<<8))+mN))break;
switch(i)					{
	case	0: return nativeCharOut();
	case	1: return nativeCharIn();
	case	2: return getButtons();
	case	3: return setOnBoardLEDs();
	case	4: return charLCDOut();
	case	5: return controlLCD(); 
	case	6: return currentTimeMillis();
	case	7: return javaExit();
	case	8: return getTemperature();
#ifndef AVR8
	case	9: return drawPointRGB();
	case	10: return drawFillRectRGB();
	case	11: return drawRectangleRGB();
	case	12: return clearScreenRGB();
	case	13: return drawRoundRectRGB();
	case	14: return drawEllipseRGB();
	case	15: return drawCircleRGB();
	case	16: return drawLineRGB();
	case	17: return drawPointHSB();
	case	18: return drawLineHSB();
	case	19: return drawTriangleFill();
	case	20: return clearZBuffer();
	case	21: return drawPointHSBZBuffer();
	case	22: return drawLineHSBZBuffer();
	case	23: return setFont();
	case	24: return setFontWindow();
	case	25: return setFontAutoLineFeed();
	case	26: return setFontFixedFont();
	case	27: return setFontCursor();
	case	28: return setFontColor();
	case	29: return drawChar();
	case	30: return drawCharAt();
	case	31: return getCharWidth();
#endif
	case	NUMNATIVPLATFORMMETHODS+0: return nativeCharAt(local);
	case	NUMNATIVPLATFORMMETHODS+1: return nativeStringLength(local);
	case	NUMNATIVPLATFORMMETHODS+2: return start(local);
	/*	3	"yield",
		4	"sleep",
		5	"currentThread",
		6	"getPriority",
		7	"setPriority",
		8	"interrupt",
		9	"interrupted",
		10	"isInterrupted",
		11	"isDaemon",
		12	"setDaemon",
		13	"join",*/
	case	NUMNATIVPLATFORMMETHODS+14: return notify(local);
	case	NUMNATIVPLATFORMMETHODS+15: return notifyAll(local);
	case	NUMNATIVPLATFORMMETHODS+16: return wait(local);	// ???
	/*	17	"waitTime",
		18	"getDataAddress",NULL};*/ //51
	case	NUMNATIVPLATFORMMETHODS+19: return nativeParseFloat(local);
	case 	NUMNATIVPLATFORMMETHODS+20: return typeConvert(local);
	case	NUMNATIVPLATFORMMETHODS+21: return typeConvert(local);
	case	NUMNATIVPLATFORMMETHODS+22: return floatToCharArray(local);

	default: errorExit(-1," native method not found: %d %d %d %d %d ",i,cN,mN,numMethods,numNativeClasses);		}							}


void	initNativeDispatch()	{
char* methodDescr=NULL;	// without signature
u2 methodDescrLength=0;
u2 i;
if ((nativeCNMN=(u1*)malloc(2*numEntryNativeCNMN*sizeof(u1)))==NULL) exit(-1);
for(i=0;i < numNativeClasses;i++)		   {
	for(cN=0; cN < numClasses;cN++)			{
		nativeName=nativeNames[i];
		if (strncmp( *nativeName,
getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3),
		strlen(*nativeName))==0)	 	{
			while((*(++nativeName))!=NULL)	{
			methodDescr=NULL;
			if (findMethodByName(*nativeName,strlen(*nativeName),methodDescr,methodDescrLength)) 	{
				*(nativeCNMN+numMethods++)=(u1)cN;
				*(nativeCNMN+numMethods++)=(u1)mN;											
		}
												}	
											}	
												
												}	}
numMethods/=2;				}

//"actplatform/Serial","nativeIntOut","nativeFloatOut","println","nativeCharOut","nativeCharIn"
// 0
char nativeCharOut()		{
char val=opStackGetValue(local+1).UInt;
	printf("%c",val);
	return 0;							}
// 1
#if LINUX||AVR32LINUX
char	nativeCharIn()	{
struct termios oldt,newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
opStackPush((slot)(u4)ch);
return 1;	
}
#endif

#if ( AVR32UC3A|| AVR32AP7000)
char	nativeCharIn()	{
char ch=conIn();
opStackPush((slot)(u4)ch);
return 1;}
#endif

#ifdef AVR8
char (*conIn)()=(void*)(0xF004);
char	nativeCharIn()	{
char ch=conIn();
opStackPush((slot)(u4)ch);
return 1;}
// added 2008 by: Stephan Bauer, FHW-BA Berlin
// Bayer Schering Pharma AG
u1 rowLCD=0;
u1 columnLCD=0;
#define LCDRowLength 15

char charLCDOut()	{
LCD_Putch(opStackGetValue(local+1).UInt);
if (rowLCD == LCDRowLength){
rowLCD=0;
columnLCD=(columnLCD+1)&0xf7;
}
else rowLCD++;
return 0;					}

char controlLCD()	{
u4 val=opStackGetValue(local+1).UInt;
// 0x00000000 clear
// 0x0100yyxx cursor at x y
switch ((val&0xff000000)>>24)	{
case 0x00: LCD_SendCmd(1);
CASE 0x01:	if (val&0x0000ff00)LCD_SendCmd(0xc0+(val&0x000000ff));
			else LCD_SendCmd(0x80+(val&0x000000ff));
}
return 0; 					}

#endif

// 5
char nativeStringLength()	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.magic==CPSTRINGMAGIC)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.classNumber);
opStackPush((slot)(u4)getU2(CP(cN,getU2(CP(cN,mySlot.stackObj.pos)+1 )) +1 ));
cN=methodStackPop();
}
else	
opStackPush((slot)(u4)0xffff);
return 1;
}

char nativeCharAt()	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.magic==CPSTRINGMAGIC)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.classNumber);
opStackPush((slot)(u4)getU1(CP(cN,getU2(CP(cN,mySlot.stackObj.pos) + 1))+3+(u2)opStackGetValue(local+1).UInt));
cN=methodStackPop();
}
else	
opStackPush((slot)(u4)0);
return 1;
}

char start()	{
cN=opStackGetValue(local).stackObj.classNumber;
createThread();
return 0;
}
//6
char yield()	{return 0; }
//7
char currentThread(){return 1; }
//8
char getPriority(){return 1; }
//9
char setPriority(){return 0; }
//10
char interrupt(){return 0; }
//11
char interrupted(){return 1; }
//12
char isInterrupted(){return 1; }
//13
char isDaemon(){return 1; }
//14
char setDaemon(){return 0; }
char  join(){return 0; }
//15
char jointimeout(){return 0; }

// "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"
char notify(){	// not tested yet aug2007
u1 i;
ThreadControlBlock* cb=actualThreadCB;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(253);};
//can not be ->IllegalMonitorStateException
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->state==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))
cb->state=THREADWAITAWAKENED;
break;
}
return 0; }

char notifyAll() {
u1 i;
return 0;
ThreadControlBlock* cb=actualThreadCB;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{exit(249); };
//can not be ->IllegalMonitorStateException
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->state==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))
cb->state=THREADWAITAWAKENED;
}
return 0; }


char wait() {
u1 i;
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(254);};
//can not be ->IllegalMonitorStateException
HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXNOTBLOCKED; // lock abgeben
actualThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
actualThreadCB->state=THREADWAITBLOCKED;
ThreadControlBlock* myTCB=actualThreadCB;
	for (i=1; i < numThreads; i++)	{	// alle blocked for object wecken!
		myTCB=myTCB-> succ;
		if  ((myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(local).UInt)&&
				(myTCB->state==THREADMUTEXBLOCKED))	{
			myTCB->state=THREADNOTBLOCKED; //!!
			myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;		}
					}
return 0; }

char waitTime(){return 0; }
char getDataAddress (u4 obj)	{
return 1;	// ret val is  on Stack !!
}


#ifdef AVR32UC3A
// its the evk1100
/*
## Author: Adrian Lang, Fritz-Haber-Institut, IT 2006
## Method: byte getButtons();
## Parameters: none
## Return Value:
##     Button State in 8 Bit array (0: Button not pressed; 1: Button pressed)
##     MSB                        ..                      LSB
##     PB0  PB1  PB2 JoystickPress JSUp JSRight JSDown JSLeft
*/
char	getButtons() {
	int buttons[8] = {GPIO_PUSH_BUTTON_0, GPIO_PUSH_BUTTON_1, GPIO_PUSH_BUTTON_2, GPIO_JOYSTICK_PUSH, GPIO_JOYSTICK_UP, GPIO_JOYSTICK_RIGHT, GPIO_JOYSTICK_DOWN, GPIO_JOYSTICK_LEFT};
	u1 n = 0;
	u1 ch = 0;
	for (; n < 8 ; ++n) 
		gpio_enable_pin_glitch_filter(buttons[n]);

	for (n = 0 ; n < 8 ; ++n) 
		ch = (ch << 1) | !gpio_get_pin_value(buttons[n]);

	opStackPush((slot) (u4) ch);
	return 1;
}

/*
## Author: Adrian Lang, Fritz-Haber-Institut, IT 2006
## Method: void setOnBoardLEDs(char);
## Parameters:
##     char state which is to be set
##     MSB                          ..                         LSB
##     LED1 LED2 LED3 LED4 LED5_red LED5_green LED6_red LED6_green
## Return Value: void
*/
char	setOnBoardLEDs() {
	char state = opStackGetValue(local+1).UInt;
	int LEDs[8] = {LED0_GPIO, LED1_GPIO, LED2_GPIO, LED3_GPIO, LED4_GPIO, LED5_GPIO, LED6_GPIO, LED7_GPIO};
	int n = 0;

	(state & 0x80) ? gpio_clr_gpio_pin(LEDs[0]) : gpio_set_gpio_pin(LEDs[0]);
	for ( n = 1 ; n <= 7 ; ++n) {
		((state << n) & 0x80) ? gpio_clr_gpio_pin(LEDs[n]) : gpio_set_gpio_pin(LEDs[n]);
	}

	return 0;
}

char charLCDOut() {
	char c = opStackGetValue(local+1).UInt;
	dip204_write_data(c);
	return 0;
}

char controlLCD() {
	int control = opStackGetValue(local+1).UInt;
	switch (control)
	{
		case 0x0000: dip204_clear_display(); 
		CASE 0x0001: dip204_show_cursor();
		CASE 0x0002: dip204_hide_cursor(); break;
		/* Parameters of set_cursor_position: lower 2 Byte and higher 2 Byte of control */
		default: dip204_set_cursor_position(control % 0xFFFF, control / 0xFFFF); break;
	};
	return 0;
}


/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: int currentTimeMillis(void);
## Parameters: none
## Return Value: int, get the value of the timer
*/
char currentTimeMillis() {
	opStackPush((slot) (u4) timerMilliSec);
	return 1;
}
#endif


#ifdef AVR8
#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
// atmega128 Monitor functions
#define		loadInSRam1		(2*(0xf000+26))
void exit(int n)	{
goto *0xf002;	//asm	 (INLINEASM(jmp,0xf002));
}
#endif

#ifdef EVK1100
void exit(int n)	{
goto *0x80000000;
}
#endif



#if NGW100 || STK1000
char currentTimeMillis(){return 0;};
char controlLCD(){ return 0;}
char charLCDOut(){return 0;}
void exit(int status)	{
asm("  lda.w   pc, 0");
//goto *0x00000000;
}
#endif

#if LINUX || AVR32LINUX || AVR8 || NGW100 || STK1000
/* the following methods are not supported by these platforms */

char getButtons()	{
	opStackPush((slot)(u4)0);
	return 1;
}

char setOnBoardLEDs()	{
	return 0;
}
#endif

char typeConvert()	{
opStackPush(opStackGetValue(local));
return 1;
}


// char arr to float
char nativeParseFloat()	{
slot mySlot=opStackGetValue(local); // the char array
char buf[mySlot.stackObj.arrayLength];
f4 f;
u4 i;
for (i=0;i<mySlot.stackObj.arrayLength;i++) 
buf[i]= (u1)heapGetElement(mySlot.stackObj.pos+i+1).UInt;
buf[mySlot.stackObj.arrayLength]=0;
sscanf(buf,"%f",&f);
opStackPush((slot)f);
return 1;
}

char floatToCharArray()	{
	slot mySlot;
	f4 f=opStackGetValue(local).Float;	// the float
	char buf[8];
	u1 i;
	for(i=0; i<8; ++i) {buf[i]=0;}
	snprintf(buf,7,"%f",f);
	u2 heapPos=getFreeHeapSpace(8+ 1);	// char arr length + marker
	mySlot.stackObj.pos=heapPos;
	mySlot.stackObj.magic=OBJECTMAGIC;
	//mySlot.stackObj.type=STACKNEWARRAYOBJECT;
	mySlot.stackObj.arrayLength=(u1)8;// char array length
	opStackPush(mySlot);
	HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
	HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
	HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
	for(i=0; i<8; i++){	heapSetElement(( slot)(u4)(*(buf+i)),heapPos++);}
	return 1;
}


char javaExit()	{
exit(opStackGetValue(local).UInt);
return 0;
}




#if LINUX||AVR32LINUX
char charLCDOut(){return 0;}
char controlLCD(){return 0;}
void timer_Init()	{}

char currentTimeMillis()	{
struct timeval timerstart;
gettimeofday(&timerstart,NULL);
opStackPush((slot)(u4)((timerstart.tv_sec*1000+timerstart.tv_usec/1000)&0xFFFFFFFF));
return 1;
}
#endif
#ifdef STK1000

char drawPointRGB()
{
	//public native void drawPointRGB(int x,int y, int color);
	lcdSetPixelRGB(opStackGetValue(local+1).UInt, 
					opStackGetValue(local+2).UInt, 
					opStackGetValue(local+3).UInt);
	return 0;
}

char drawFillRectRGB()
{
	//public native void drawFillRectRGB(int x1, int y1, int x2, int y2, int color);
	lcdFillRect(opStackGetValue(local+1).UInt, 
				opStackGetValue(local+2).UInt, 
				opStackGetValue(local+3).UInt, 
				opStackGetValue(local+4).UInt,
				opStackGetValue(local+5).UInt);
	
	return 0;
}

char drawRectangleRGB()
{
	//public native void drawRectangleRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
	lcdRectangle(opStackGetValue(local+1).UInt, 
				opStackGetValue(local+2).UInt,
				opStackGetValue(local+3).UInt, 
				opStackGetValue(local+4).UInt,
				opStackGetValue(local+5).UInt,
				opStackGetValue(local+6).UInt);
	return 0;
}

char clearScreenRGB()
{
	//public native void clearScreenRGB(int color);
	lcdClearScreen(opStackGetValue(local+1).UInt);
	return 0;
}

char drawRoundRectRGB()
{
	//public native void drawRoundRectRGB(int x1, int y1, int x2, int y2, int a, int b, int fgcolor, int bkcolor);
	lcdRoundRect(opStackGetValue(local+1).UInt,
					opStackGetValue(local+2).UInt, 
					opStackGetValue(local+3).UInt,
					opStackGetValue(local+4).UInt,
					opStackGetValue(local+5).UInt,
					opStackGetValue(local+6).UInt,
					opStackGetValue(local+7).UInt,
					opStackGetValue(local+8).UInt);
	return 0;
}

char drawEllipseRGB()
{
	//public native void drawEllipseRGB(int x1, int y1, int x2, int y2, int fgcolor, int bkcolor);
	lcdEllipse(opStackGetValue(local+1).UInt,
				opStackGetValue(local+2).UInt,
				opStackGetValue(local+3).UInt,
				opStackGetValue(local+4).UInt,
				opStackGetValue(local+5).UInt,
				opStackGetValue(local+6).UInt);
	return 0;
}

char drawCircleRGB()
{
	//public native void drawCircleRGB(int x, int y, int r, int fgcolor, int bkcolor);
	lcdCircle(opStackGetValue(local+1).UInt,
				opStackGetValue(local+2).UInt,
				opStackGetValue(local+3).UInt,
				opStackGetValue(local+4).UInt,
				opStackGetValue(local+5).UInt);
	return 0;
}

char drawLineRGB()
{
	//public native void drawLineRGB(int x1, int y1, int x2, int y2, int color);
	lcdLine(opStackGetValue(local+1).UInt,
			opStackGetValue(local+2).UInt,
			opStackGetValue(local+3).UInt,
			opStackGetValue(local+4).UInt,
			opStackGetValue(local+5).UInt);
	return 0;
}
char drawPointHSB()
{
	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
                                                    // otherwise local+1
                                                    // mySlot is a reference to heap object (Point p)
	
	lcdSetPixelHSB(heapGetElement(mySlot.stackObj.pos+1).Float,	//x
			heapGetElement(mySlot.stackObj.pos+2).Float,	//y
			heapGetElement(mySlot.stackObj.pos+4).Float,	//h
			heapGetElement(mySlot.stackObj.pos+5).Float,	//s
			heapGetElement(mySlot.stackObj.pos+6).Float);	//b
			
	return 0;
}

char drawLineHSB()
{
	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
        					      // otherwise local+1
                                                    // mySlot is a reference to heap object (Point p)

	lcdLinePoint_t p1;
	p1.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p1.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p1.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p1.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p1.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

	mySlot=opStackGetValue(local+2);
	lcdLinePoint_t p2;
	p2.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p2.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p2.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p2.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p2.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b
	
	lcdDDALine(p1, p2);
	return 0;
}

char drawTriangleFill()
{
	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
        					      // otherwise local+1
                                                    // mySlot is a reference to heap object (Point p)

	lcdLinePoint_t p1;
	p1.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p1.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p1.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p1.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p1.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

	mySlot=opStackGetValue(local+2);
	lcdLinePoint_t p2;
	p2.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p2.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p2.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p2.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p2.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

	mySlot=opStackGetValue(local+3);
	lcdLinePoint_t p3;
	p3.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p3.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p3.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p3.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p3.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b
	
	lcdTriangleFill(p1, p2, p3);

	return 0;
}

char clearZBuffer()
{
	lcdInitZBuffer();
	return 0;
}

char drawPointHSBZBuffer()
{
	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
        					      // otherwise local+1
                                                    // mySlot is a reference to heap object (Point p)

	lcdLinePoint_t p;
	p.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p.z=heapGetElement(mySlot.stackObj.pos+3).Float;	//z
	p.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

	lcdDrawZBufferPoint(p);
	return 0;
}
char drawLineHSBZBuffer()
{
	slot mySlot=opStackGetValue(local+1);       // in static methods the first Arg on Stack
        					      // otherwise local+1
                                                    // mySlot is a reference to heap object (Point p)

	lcdLinePoint_t p1;
	p1.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p1.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p1.z=heapGetElement(mySlot.stackObj.pos+3).Float;	//z
	p1.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p1.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p1.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b

	mySlot=opStackGetValue(local+2);
	lcdLinePoint_t p2;
	p2.x=heapGetElement(mySlot.stackObj.pos+1).Float;	//x
	p2.y=heapGetElement(mySlot.stackObj.pos+2).Float;	//y
	p2.z=heapGetElement(mySlot.stackObj.pos+3).Float;	//z
	p2.h=heapGetElement(mySlot.stackObj.pos+4).Float;	//h
	p2.s=heapGetElement(mySlot.stackObj.pos+5).Float;	//s
	p2.b=heapGetElement(mySlot.stackObj.pos+6).Float;	//b
	
	lcdDDALineZBuffer(p1, p2);
	return 0;
}

char setFont()
{
	lcdSelectFontFromArray(opStackGetValue(local+1).UInt);
	return 0;
}

char setFontWindow()
{
	lcdSetRect(lcd_Window, 
		opStackGetValue(local+1).UInt,
		opStackGetValue(local+2).UInt, 
		opStackGetValue(local+3).UInt, 
		opStackGetValue(local+4).UInt); //Schrift-Fenster
	return 0;
}

char setFontAutoLineFeed()
{
	if (opStackGetValue(local+1).UInt>0)
	{	
		lcd_Flags.AutoLineFeed=1;
	}
	else
	{
		lcd_Flags.AutoLineFeed=0;
	}
	return 0;
}

char setFontFixedFont()
{
	if (opStackGetValue(local+1).UInt>0)
	{	
		lcd_Flags.FixedFont=1;
	}
	else
	{
		lcd_Flags.FixedFont=0;
	}
	return 0;
}

char setFontCursor()
{
	lcd_Cursor.X = opStackGetValue(local+1).UInt;
	lcd_Cursor.Y = opStackGetValue(local+2).UInt;

	return 0;
}

char setFontColor()
{
	//public native void setFontColor(int fgcolor, int bkcolor);
	lcd_Colors[0] = opStackGetValue(local+2).UInt;		//Hintergrundfarbe, NONE = Transparent
	lcd_Colors[1] = opStackGetValue(local+1).UInt;		//Vordergrundfarbe
	return 0;
}

char drawChar()
{
	opStackPush((slot) (u4) lcdDrawChar(opStackGetValue(local+1).UInt));
	return 0;
}

char drawCharAt()
{
	opStackPush((slot) (u4) lcdDrawCharAt(opStackGetValue(local+1).UInt,
						opStackGetValue(local+2).UInt,
						opStackGetValue(local+3).UInt,
						opStackGetValue(local+4).UInt,
						opStackGetValue(local+5).UInt));
	return 0;
}

char getCharWidth()
{
	opStackPush((slot) (u4) lcdCharWidth(opStackGetValue(local+1).UInt));
	return 0;
}

#endif

#if AVR8||NGW100||LINUX||AVR32LINUX||EVK1100

//Grafikmethoden nur fuer STK1000 implementiert
char drawPointRGB(){ printf("native drawPointRGB\n"); return 0;}
char drawFillRectRGB(){printf("native drawFillRectRGB\n"); return 0;}
char drawRectangleRGB(){printf("native drawRectangleRGB\n"); return 0;}
char clearScreenRGB()
{
	printf("native clearScreenRGB\n"); 
	printf("Screen Color:: %d\n",opStackGetValue(local+1).UInt);
	return 0;
}
char drawRoundRectRGB(){printf("native drawRoundRectRGB\n"); return 0;}
char drawEllipseRGB(){printf("native drawEllipseRGB\n"); return 0;}
char drawCircleRGB(){printf("native drawCircleRGB\n"); return 0;}
char drawLineRGB(){printf("native drawLineRGB\n"); return 0;}

char drawPointHSB()
{
	printf("native drawPointHSB\n"); 

	slot mySlot=opStackGetValue(local+1);         // in static methods the first Arg on Stack
                                                    // otherwise local+1
                                                    // mySlot is a reference to heap object (Point pa)
	f4 x=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x);
	f4 y=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y);
	f4 z=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z);
	f4 h=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h);
	f4 s=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",s);
	f4 b=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b);
	
/*mySlot=opStackGetValue(local+1);         // in static methods the second Arg on Stack
                                                              // otherwise local+2
// mySlot is a reference to heap object (Point pe)
x=heapGetElement(mySlot.stackObj.pos+1).Float;
printf("point pe, x koordinate: %f",x);*/
	return 0;
}
char drawLineHSB()
{
	printf("native drawLineHSB\n"); 

	slot mySlot=opStackGetValue(local+1);         // in static methods the first Arg on Stack
                                                    // otherwise local+1
                                                    // mySlot is a reference to heap object (Point pa)
	f4 x=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x);
	f4 y=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y);
	f4 z=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z);
	f4 h=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h);
	f4 s=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",s);
	f4 b=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b);
	
	mySlot=opStackGetValue(local+2);         // in static methods the second Arg on Stack
                                                              // otherwise local+2
	f4 x2=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x2);
	f4 y2=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y2);
	f4 z2=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z2);
	f4 h2=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h2);
	f4 _s2=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",_s2);
	f4 b2=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b2);

	return 0;
}
char drawTriangleFill()
{
	printf("native drawTriangleFill\n"); 

	slot mySlot=opStackGetValue(local+1);         // in static methods the first Arg on Stack
                                                    // otherwise local+1
                                                    // mySlot is a reference to heap object (Point pa)
	f4 x=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x);
	f4 y=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y);
	f4 z=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z);
	f4 h=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h);
	f4 s=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",s);
	f4 b=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b);
	
	mySlot=opStackGetValue(local+2);         // in static methods the second Arg on Stack
                                                              // otherwise local+2
	f4 x2=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x2);
	f4 y2=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y2);
	f4 z2=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z2);
	f4 h2=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h2);
	f4 _s2=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",_s2);
	f4 b2=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b2);

	mySlot=opStackGetValue(local+3);         // in static methods the second Arg on Stack
                                                              // otherwise local+2
	f4 x3=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x3);
	f4 y3=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y3);
	f4 z3=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z3);
	f4 h3=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h3);
	f4 s3=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",s3);
	f4 b3=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b3);

	return 0;
}
char clearZBuffer(){printf("native clearZBuffer\n"); return 0;}
char drawPointHSBZBuffer()
{
	printf("native drawPointHSBZBuffer\n"); 
	slot mySlot=opStackGetValue(local+1);         // in static methods the first Arg on Stack
                                                    // otherwise local+1
                                                    // mySlot is a reference to heap object (Point pa)
	f4 x=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x);
	f4 y=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y);
	f4 z=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z);
	f4 h=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h);
	f4 s=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",s);
	f4 b=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b);
	return 0;
}
char drawLineHSBZBuffer()
{
	printf("native drawLineHSBZBuffer\n"); 
	
	slot mySlot=opStackGetValue(local+1);         // in static methods the first Arg on Stack
                                                    // otherwise local+1
                                                    // mySlot is a reference to heap object (Point pa)
	f4 x=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x);
	f4 y=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y);
	f4 z=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z);
	f4 h=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h);
	f4 s=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",s);
	f4 b=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b);
	
	mySlot=opStackGetValue(local+2);         // in static methods the second Arg on Stack
                                                              // otherwise local+2
	f4 x2=heapGetElement(mySlot.stackObj.pos+1).Float; // the first field (variable) of Object
	printf("point, x koordinate: %f\n",x2);
	f4 y2=heapGetElement(mySlot.stackObj.pos+2).Float;        // the second
	printf("point, y koordinate: %f\n",y2);
	f4 z2=heapGetElement(mySlot.stackObj.pos+3).Float;
	printf("point, z koordinate: %f\n",z2);
	f4 h2=heapGetElement(mySlot.stackObj.pos+4).Float;
	printf("point, h koordinate: %f\n",h2);
	f4 _s2=heapGetElement(mySlot.stackObj.pos+5).Float;
	printf("point, s koordinate: %f\n",_s2);
	f4 b2=heapGetElement(mySlot.stackObj.pos+6).Float;
	printf("point, b koordinate: %f\n",b2);
	return 0;
}

char setFont()
{
	printf("native setFont\n");
	printf("%d\n", opStackGetValue(local+1).UInt);
	return 0;
}
char setFontWindow()
{
	printf("native setFontWindow\n"); 
	printf("%d\n", opStackGetValue(local+1).UInt);
	printf("%d\n", opStackGetValue(local+2).UInt);
	printf("%d\n", opStackGetValue(local+3).UInt);
	printf("%d\n", opStackGetValue(local+4).UInt);
	return 0;
}
char setFontAutoLineFeed(){printf("native setFontAutoLineFeed\n"); return 0;}
char setFontFixedFont(){printf("native setFontFixedFont\n"); return 0;}
char setFontCursor(){printf("native setFontCursor\n"); return 0;}
char setFontColor(){printf("native setFontColor\n"); return 0;}
char drawChar()
{
	printf("native drawChar\n");
	char val=opStackGetValue(local+1).UInt;
	printf("%c\n",val);
	return 0;
}
char drawCharAt(){printf("native drawCharAt\n"); return 0;}
char getCharWidth(){printf("native getCharWidth\n"); return 0;}

#endif

#ifdef AVR8
// added 2009 by: Hannes Walz, FHW-BA Berlin
// Berliner Stadtreinigung
char getTemperature() 
{		
	float t = NativeGetTemperature();	
	printf("%f is kalt\n", t);
	opStackPush((slot) (f4) NativeGetTemperature());
	return 1;
}
#else
char getTemperature() { return 0; }
#endif