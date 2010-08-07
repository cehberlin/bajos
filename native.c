/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
// remember
// invokespecial Operand Stack
// ..., objectref, [arg1, [arg2 ...]] -> ...
// invokestatic: Operand Stack
// ..., [arg1, [arg2 ...]] -> ...

#include <stdio.h>
#include <stdlib.h>
#ifdef LINUX
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#endif
#include <string.h>
#include "definitions.h"
#include "bajosavr.h"
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
// all native methods have a return value
// 0-> is a Java native void function
// 1-> is an Java native function with anyone return value (in stack)
// alle native methoden haben einen eindeutigen namen!!
// fill this arrays
char*	nativePlatForm[]={"platform/PlatForm",
		"nativeCharOut",
		"nativeCharIn",
		"getButtons",
		"setOnBoardLEDs",
		"charLCDOut", 
		"controlLCD",
		"currentTimeMillis",
		"exit",NULL}
;char* nativeString[]={"java/lang/String",
		"nativeCharAt",
		"nativeStringLength",NULL};
char*	nativeThread[]={"java/lang/Thread",
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
char*	nativeObject[]={"java/lang/Object",
		"notify",
		"notifyAll",
		"wait",
		"waitTime",
		"getDataAddress",NULL};
char*	nativeFloat[]={"java/lang/Float",
		"nativeParseFloat",
		"floatToIntBits",
		"intBitsToFloat",
		"floatToCharArray",NULL};

// fill this array
char**	nativeNames[]={	nativePlatForm,
			nativeString,
			nativeThread,
			nativeObject,
			nativeFloat};

char ** nativeName;
//(sizeof(word)/sizeof(*word));
u2	numNativeClasses=sizeof(nativeNames)/sizeof(*nativeNames);
u2 numMethods=0;
u1*	nativeCNMN;
int numEntryNativeCNMN=80; //???
u2 i;

char	nativeDispatch( u2 cN, u2 mN,u2 local)	{
//printf("nummezthods %d:\n",numMethods);
for(i=0;i<numMethods;i++)	{
//printf(": %d %x %x  %x\n",i,(((u2)*(nativeCNMN+2*i)<<8)+(u2)(*(nativeCNMN+2*i+1))),cN,mN);
if ((((u2)*(nativeCNMN+2*i)<<8)+(*(nativeCNMN+2*i+1)))==(((u2)(cN<<8))+mN))break;
}
//printf(": %d %x %x  %x\n",i,(((u2)*(nativeCNMN+2*i)<<8)+(u2)(*(nativeCNMN+2*i+1))),cN,mN);

switch(i)					{
	case	0: return nativeCharOut(local);
	case	1: return nativeCharIn();
	case	2: return getButtons();
	case	3: return setOnBoardLEDs(local);
	case	4: return charLCDOut(local);
	case	5: return controlLCD(local); 
	case	6: return currentTimeMillis();
	case	7: return javaExit(local);
	case	8: return nativeCharAt(local);
	case	9: return nativeStringLength(local);
	case	10: return start(local);
	case	22: return notify(local);
	case	23: return notifyAll(local);
	case	24: return wait(local);	// ???
	case	27: return nativeParseFloat(local);
	case 	28: return typeConvert(local);
	case	29: return typeConvert(local);
	case	30: return floatToCharArray(local);
	default: printf(" native method not found: %d %d %d %d %d ",i,cN,mN,numMethods,numNativeClasses);
			   exit(-1);		}							}

void	initNativeDispatch()	{
u1* methodDescr=NULL;	// ohne signature arbeiten!!
u2 methodDescrLength=0;
if ((nativeCNMN=(u1*)malloc(2*numEntryNativeCNMN*sizeof(u1)))==NULL) exit(-1);
for(i=0;i < numNativeClasses;i++)		   {
	for(cN=0; cN < numClasses;cN++)			{
		nativeName=nativeNames[i];
		if (strncmp( *nativeName,
getAddr(cs[cN].constant_pool[getU2(cs[cN].constant_pool[getU2(cs[cN].this_class)]+1)]+3),
		strlen(*nativeName))==0)	 	{
			while((*(++nativeName))!=NULL)	{
		//	printf("%s %d\n",*nativeName,strlen(*nativeName));
			methodDescr=NULL;
			if (findMethodByName((u1*)*nativeName,strlen(*nativeName),methodDescr,methodDescrLength)) 	{
				*(nativeCNMN+numMethods++)=(u1)cN;
				*(nativeCNMN+numMethods++)=(u1)mN;	
		//		printf("%s %d %d    %d\n",	*nativeName,cN,mN,numMethods/2	);										
		}
												}	
											}	
												
												}	}
numMethods/=2;				}

//"actplatform/Serial","nativeIntOut","nativeFloatOut","println","nativeCharOut","nativeCharIn"
// 0
char nativeCharOut(u2 local)		{
char val=opStackGetValue(local+1).UInt;
	printf("%c",val);
	return 0;							}
// 1
#ifdef LINUX
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

char charLCDOut(u2 local)	{
LCD_Putch(opStackGetValue(local+1).UInt);
if (rowLCD == LCDRowLength){
rowLCD=0;
columnLCD=(columnLCD+1)&0xf7;
}
else rowLCD++;
return 0;					}

char controlLCD(u2 local)	{
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
//"java/lang/Thread","start"/*5*/,"yield","currentThread","getPriority()","setPriority","interrupt","interrupted",
//"isInterrupted","isDaemon","setDaemon","join"," jointimeout"
// 5

char nativeStringLength(u2 local)	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.magic==CPSTRINGMAGIC)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.pos >>8);
getU2(CP(cN,getU2(CP(cN,mySlot.stackObj.pos&0xff)+1 )) +1 );
opStackPush((slot)(u4)0);
opStackPush((slot)(u4)getU2(CP(cN,getU2(CP(cN,mySlot.stackObj.pos&0xff)+1 )) +1 ));
cN=methodStackPop();
}
else	
opStackPush((slot)(u4)0xffff);
return 1;
}

char nativeCharAt(u2 local)	{
slot mySlot=opStackGetValue(local);
if (mySlot.stackObj.magic==CPSTRINGMAGIC)	{
methodStackPush(cN);
cN=(u1)(mySlot.stackObj.pos >>8);
opStackPush((slot)(u4)getU1(CP(cN,getU2(CP(cN,mySlot.stackObj.pos&0x00ff) + 1))+3+(u2)opStackGetValue(local+1).UInt));
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
char setPriority(u2 local){return 0; }
//10
char interrupt(){return 0; }
//11
char interrupted(){return 1; }
//12
char isInterrupted(){return 1; }
//13
char isDaemon(){return 1; }
//14
char setDaemon(u2 local){return 0; }
char  join(){return 0; }
//15
char jointimeout(u2 local){return 0; }

// "java/lang/Object","notify","notifyAll","wait","waitTime","getDataAddress"
char notify(){	// not tested yet aug2007
u1 i;
ThreadControlBlock* cb=actualThreadCB;
//printf("notify %04x\n",opStackGetValue(local));
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(253);};
//darf nicht sein ->IllegalMonitorStateException
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->status==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))
cb->status=THREADWAITAWAKENED;
break;
}
return 0; }

char notifyAll() {
u1 i;
return 0;
ThreadControlBlock* cb=actualThreadCB;
//printf("notifyall %04x\n",opStackGetValue(local));
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{exit(249); };
//darf nicht sein ->IllegalMonitorStateException
for (i=1; i < numThreads;i++)	{
cb=cb->succ;
if ((cb->status==THREADWAITBLOCKED)&&((cb->isMutexBlockedOrWaitingForObject).UInt==opStackGetValue(local).UInt))//stackObj.pos)) //!!!bh
//cb->status=THREADNOTBLOCKED;
cb->status=THREADWAITAWAKENED;
}
return 0; }


char wait() {
//slot mySlot=opStackPop();
u1 i;

//printf("wait %04x numthr %d\n",opStackGetValue(local), numThreads);
if (HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex!=MUTEXBLOCKED)	{ exit(254);};
//darf nicht sein ->IllegalMonitorStateException
HEAPOBJECTMARKER(opStackGetValue(local).stackObj.pos).mutex=MUTEXNOTBLOCKED; // lock abgeben
actualThreadCB->isMutexBlockedOrWaitingForObject=opStackGetValue(local);
actualThreadCB->status=THREADWAITBLOCKED;
ThreadControlBlock* myTCB=actualThreadCB;
						for (i=1; i < numThreads; i++)	{	// alle blocked for object wecken!
						myTCB=myTCB-> succ;
						if  ((myTCB->isMutexBlockedOrWaitingForObject.UInt==opStackGetValue(local).UInt)&&
											(myTCB->status==THREADMUTEXBLOCKED))	{
							myTCB->status=THREADNOTBLOCKED; //!!
							myTCB->isMutexBlockedOrWaitingForObject=NULLOBJECT;		}
														}

/*for (i=0; i< MAXLOCKEDTHREADOBJECTS;i++)
if ((actualThreadCB->hasMutexLockForObject[i]).UInt == opStackGetValue(local).UInt )break;
if (i==MAXLOCKEDTHREADOBJECTS)exit(253); // was dann?
//printf("::: %d %d\n",i,actualThreadCB->lockCount[i]);
actualThreadCB->lockCount[i]-=1;	// count decrementieren
if (actualThreadCB->lockCount[i]==0)actualThreadCB->hasMutexLockForObject[i]=NULLOBJECT;
*/

return 0; }

char waitTime(u2 local){return 0; }
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
char	setOnBoardLEDs(u2 local) {
	char state = opStackGetValue(local+1).UInt;
	int LEDs[8] = {LED0_GPIO, LED1_GPIO, LED2_GPIO, LED3_GPIO, LED4_GPIO, LED5_GPIO, LED6_GPIO, LED7_GPIO};
	int n = 0;

	(state & 0x80) ? gpio_clr_gpio_pin(LEDs[0]) : gpio_set_gpio_pin(LEDs[0]);
	for ( n = 1 ; n <= 7 ; ++n) {
		((state << n) & 0x80) ? gpio_clr_gpio_pin(LEDs[n]) : gpio_set_gpio_pin(LEDs[n]);
	}

	return 0;
}

char charLCDOut(u2 local) {
	char c = opStackGetValue(local+1).UInt;
	dip204_write_data(c);
	return 0;
}

char controlLCD(u2 local) {
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
asm	 (INLINEASM(jmp,0xf002));
}
#endif

#ifdef AVR32UC3A
void exit(int n)	{
goto *0x80000000;
}
#endif

#ifdef NGW100
char currentTimeMillis(){return 0;};
char controlLCD(u2 local){ return 0;}
char charLCDOut(u2 local){return 0;}
void exit(int status)	{
asm("  lda.w   pc, 0");
//goto *0x00000000;
}
#endif
#if LINUX || AVR8 || NGW100
char getButtons()	{

return 1;
}


char setOnBoardLEDs(u2 local)	{

return 0;
}
#endif



char typeConvert(u2 local)	{
opStackPush(opStackGetValue(local));
return 1;
}


// char arr to float
char nativeParseFloat(u2 local)	{
slot mySlot=opStackGetValue(local); // the char array
u1 buf[mySlot.stackObj.arrayLength];
f4 f;
for (i=0;i<mySlot.stackObj.arrayLength;i++) 
buf[i]= (u1)heapGetElement(mySlot.stackObj.pos+i+1).UInt;
buf[mySlot.stackObj.arrayLength]=0;
scanf(buf,"%f",&f);
opStackPush((slot)f);
return 1;
}

char floatToCharArray(u2 local)	{
slot mySlot;
f4 f=opStackGetValue(local).Float;	// the float
u1 buf[8];
snprintf(buf,8,"%8f",f);
heapPos=getFreeHeapSpace(8+ 1);	// char arr length + marker
mySlot.stackObj.pos=heapPos;
mySlot.stackObj.magic=OBJECTMAGIC;
//mySlot.stackObj.type=STACKNEWARRAYOBJECT;
mySlot.stackObj.arrayLength=(u1)8;// char array length
opStackPush(mySlot);
HEAPOBJECTMARKER(heapPos).status=HEAPALLOCATEDARRAY;
HEAPOBJECTMARKER(heapPos).magic=OBJECTMAGIC;
HEAPOBJECTMARKER(heapPos++).mutex = MUTEXNOTBLOCKED;
for(i=0; i<8; i++)		heapSetElement(( slot)(u4)(*(buf+i)),heapPos++);
return 1;
}


char javaExit(u2 local)	{
exit(opStackGetValue(local).UInt);
return 0;
}




#ifdef LINUX
char charLCDOut(u2 local){return 0;}
char controlLCD(u2 local){return 0;}
void timer_Init()	{}
char currentTimeMillis()	{
struct timeval start;
gettimeofday(&start,NULL);
opStackPush((slot)(u4)((start.tv_sec*1000+start.tv_usec/1000)&0xFFFFFFFF));
return 1;
}
#endif

