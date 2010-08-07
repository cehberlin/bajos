/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
// version 0.1 vom 1.10.07
/********************************************************************************************/
/* 2006 - Erweiterungen von Matthias Böhme und Jakob Fahrenkrug			*/
/* Studenten der Informatik an der FHW-Berlin/ Fachbereich Berufsakademie	*/
/* Ausbildungsbetrieb: Bayer-Schering AG					*/
/* 2008 - Erweiterungen durch				*/
/*******************************************************************************************/
// no double, no long
// no access flags evaluation
// no utf8 but ascii
// ignore some attributes
// no interfaces
// no inner classes
// no classloader
// static fields in classes must be the first elements in a class!!!!!!!!!!!!!!!!!!!!!
// no ...
// and errors ........................................................................
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef AVR8
#include <avr/io.h>
#include <avr/interrupt.h>
#include "AVR8/lcd.h"
#endif

#include "definitions.h"
#define __DEF_GLOBALS__
#include "bajosavr.h"
#include "heap.h"
#include "stack.h"
#include "classfile.h"
#include "interpreter.h"
#include "native.h"
#if (AVR32UC3A||AVR32AP7000)
#include "iobinding.h"
#endif 
#ifdef AVR32AP7000
#include "AVR32AP7000/usartap7000.h"
#ifdef STK1000
#include "STK1000/sdram.h"
#include "STK1000/mt481c2m32b2tg.h"
#endif
#ifdef NGW100
//#include "sdram.h"
#include "NGW100/hsdramc1.h"
#include <asm/io.h>
#include <asm/sdram.h>
#include <asm/arch/gpio.h>
#include <asm/arch/hmatrix2.h>
unsigned long sdram_init(const struct sdram_info *info);
#endif
#include <avr32/io.h>
#endif 
#include "scheduler.h"
// einmal durch 4.7.06 unix
// lief am 24.8.06 erstmals auf atmega128

#ifdef AVR8
#define		BYTES(word)			((word)*2)
#define		STRING(a,b)				#a" "#b
#define		INLINEASM(a,b)			STRING(a,b)

#define		LARGEBOOTSTART	0xf000
#define		BOARDRAMEND	0x8000
#define		MONSTART	LARGEBOOTSTART
#define 	MONRAM		CHARONRAMEND-0x100

// Monitorfunktionen
#define		mainLoop		BYTES(LARGEBOOTSTART+2)	// Ruecksprung in Monitor aus Programm mit goto
#define		saveCPU		BYTES(LARGEBOOTSTART+62)	//Time2Comp	// BOOTSTART+62		
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
SIGNAL(SIG_OUTPUT_COMPARE2) __attribute__ ((naked));
SIGNAL(SIG_OUTPUT_COMPARE2) {asm volatile  (INLINEASM(jmp,saveCPU));} // monitor für step-betrieb
/*
ISR(TIMER2_COMP_vect)	{ asm volatile	("\t	pop	r29 \n\
												\t	pop	r28\n\
												\t	pop	r0\n\
												\t	pop	r0\n\
												\t	pop	r1\n\
												jmp	2*(0xf03e)");}
*/
// das muss jedesmal ueberprueft werden
/*
void sendByte (char c) { asm volatile	("call	2*(0xf006)");	}
char getByte () { asm volatile	("call	2*(0xf004)");	}
void gotoMonitor () { asm volatile	("call	2*(0xf002)");	}
void saveCPU () { asm volatile	("call	2*(0xf03e)");	}
*/
/*
void my_init_sp (void) __attribute__ ((naked)) \
     __attribute__ ((section (".init2")));
void	my_init_sp (void)	{
         SPL = 0xff;
         SPH = 0x0c;			}
*/
#endif
#if !(LINUX || AVR8 || AVR32UC3A||AVR32AP7000)
#error einZielsystem muß es doch geben?
#endif


int main(int argc,char* argv[]){
	initHW();
	printf("Bajos starting\n");
	initVM(argc-1,argv);					// einlesen der classfiles (superclass first!!) und erzeugen von "class-object" für klassen mit static fields
										// initialisierung des native interface
	createThread();						// for main
	opStackBase		= actualThreadCB->opStackBase;
	opStackSetSpPos(0);

	methodStackBase	= actualThreadCB->methodStackBase;
	methodStackSetSpPos(0);	
#ifdef AVR8
	printf("SP: %x cFB: %x hB: %x oPSB: %x mSB: %x\n", 256*SPH+SPL,classFileBase, heapBase, opStackBase, methodStackBase);
#endif
	printf("start clinit\n");
	executeClInits();							// anfangs ausfuehren aller clinit-methoden!!		
	printf("<clinit> 's executed");
	if (findMain() == 0)		{printf("no main found %d %d\n",numClasses,cN);return 1;	}	// eine und nur eine main muss sein
												// wir suchen die Klasse mit main und fuehren main aus
												// allgemeiner Ausfuehren von main der Klasse X sucht in Klassenhierarchie die erste main
												//  no error handling!!
	printf("  -> run <main> :\n");
	opStackPush((slot) (u4)0);				// args parameter to main (should be a string array)
	opStackSetSpPos(findMaxLocals());
	run(); 									//  run main
	return 0;

}

void executeClInits()		{ 				// alle clinit in er eingelesenen reihenfolge
DEBUGPRINTSTACK;
DEBUGPRINTLOCALS;
DEBUGPRINTHEAP;
	for (cN=0; cN < numClasses;cN++)	
	if (findMethodByName((u1*)"<clinit>",8,(u1*)"()V",3))	{
			opStackPush(cs[cN].classInfo); // wenn <clinit>, dann gibt evtl. es static fields, die zu initialisieren sind
			opStackSetSpPos(findMaxLocals());	
			run();									};	
}
#ifdef AVR8
#define		loadInSRam1		(2*(0xf000+26))
u2 	loadInSram1(u1*) __attribute__ ((naked));

u2 loadInSram1(unsigned char* addr)	{
asm	 (INLINEASM(jmp,loadInSRam1));	
return 0;				}

#endif

// class file stehen in linux im DS
// im avr8 im sram
// in ap7000 und uc3a:
// bootclassen im flash
// anwendungsklassen im DS(Ram)
void initVM(int argc, char* argv[]){	// read, analyze classfiles and fill structures
	u2 length;
#ifdef AVR32UC3A
classFileBase=(u1*)0x80040000;
#endif
#ifdef AVR32AP7000
classFileBase=(u1*)0x40000;  // boot classes in flash
#endif
#if (LINUX||AVR8)
	if ((classFileBase=(u1*)malloc((size_t)MAXBYTECODE))==NULL) {printf("malloc error\n");exit(-1);}
	// Platz fuer classfiles -> fixed size
#endif
#if (AVR32UC3A || AVR32AP7000)
if ((apClassFileBase= (u1*) malloc(1000))==NULL){ printf("malloc error\n");exit(-1);}
#endif
//apClassFileBase=0x10000000;
	heapInit();
	length=0;
	#ifdef LINUX
		for (cN=0; cN < argc; cN++)			{
			cs[cN].classFileStartAddress=classFileBase+length;
			cs[cN].classFileLength=readClassFile((u1*)argv[cN+1],cs[cN].classFileStartAddress);
			analyzeClass(&cs[cN]);	
			length+=cs[cN].classFileLength;	}
	#endif
#if (AVR32UC3A       || AVR32AP7000)
// analysieren der bootklassen, welche mit jtag-programming schon im flash stehen
u1* addr;
numClasses=*classFileBase;	// erstmal die boot klassen
addr=classFileBase+4; // after numclasses
for (cN=0; cN<numClasses;cN++)	{
printf("Klasse: %d\n",cN);
cs[cN].classFileStartAddress=addr+4;	// after length of class
cs[cN].classFileLength=(u1)(*addr)+256*(u1)(*(addr+1));
			analyzeClass(&cs[cN]);	
addr+=cs[cN].classFileLength+4;
}
cN=numClasses;
// das waren die boot klassen
// jetzt die Anwenderklassen


cN--;
addr-=4;
addr=apClassFileBase;
length=0;
		do
		{
		printf("laden der Anwenderklassen-> Geben Sie w ein %d\n",cN);
			cN++;
			cs[cN].classFileStartAddress=apClassFileBase+length;
			cs[cN].classFileLength=readClassFile(NULL,cs[cN].classFileStartAddress);
			printf("\n");
//			for (length=0; length < 16;length++)
//			printf("%2x ",*(cs[cN].classFileStartAddress+length));
			length+=cs[cN].classFileLength;
//			if (cs[cN].classFileLength ==0) break;
			analyzeClass(&cs[cN]);
			printf("noch eine Klasse (y)");
			if (conIn()!='y') break;
		} 
		while(cs[cN].classFileLength !=0);
cN++;//!!

#endif

#ifdef AVR8
		printf("Laden der aller Bootklassen - Geben Sie ein  'w' ein %d\n",cN); 
// the damned holznagelsche protokoll zum laden eines bin files mit minikermit nachbilden
(*loadInSram1)(classFileBase);
u1* addr;
numClasses=*classFileBase;
addr=classFileBase+4; // after numclasses
for (cN=0; cN<numClasses;cN++)	{
cs[cN].classFileStartAddress=addr+4;	// after length
cs[cN].classFileLength=(u1)(*addr)+256*(u1)(*(addr+1));
			analyzeClass(&cs[cN]);	
			printf("\ndone %d",cN);
addr+=cs[cN].classFileLength+4;
}

cN--;
addr-=4;
		do
		{
			cN++;
			cs[cN].classFileStartAddress=addr+length;
			cs[cN].classFileLength=readClassFile(NULL,cs[cN].classFileStartAddress);
			length+=cs[cN].classFileLength;
			if (cs[cN].classFileLength ==0) break;
			analyzeClass(&cs[cN]);
					
		} 
		while(cs[cN].classFileLength !=0);
	
	#endif
			
	numClasses=cN;
	printf("laden von java Klassen: \n");
	DEBUGPRINTHEAP;
	initNativeDispatch();
	printf("initNativeDispatch\n");
}









void initHW(){
#ifdef AVR8
	stdout = stdin=stderr = &uart_str;	
LCD_Init();
// use timer 2 for real time clock
timer_Init();
}

	int uart_putchar(char c, FILE *stream){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	if (c=='\n') uart_putchar('\r',stream);
	return 0;								}

int uart_getchar(FILE *stream)	{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return (int)UDR0;	

#define		STRING(a,b)		#a" "#b
#define		INLINEASM(a,b)	STRING(a,b)
// atmega128 Monitor functions
void exit(int n)	{
asm	 (INLINEASM(jmp,0xf002));
}

#endif

#if  (AVR32UC3A|| AVR32AP7000)
usartInit();
stdIOInit();
#ifdef AVR32UC3A 
initTimer();
#endif
#endif

#ifdef STK1000
char c;
int addr;
int* sram;
int i,j;
#define STK1000_SDRAM_BASE 0xB0000000
  static sdram_info *info;
  unsigned long sdram_size, tmp;
  int noErrors=0;
  volatile unsigned long *sdram = (void *) STK1000_SDRAM_BASE;

  info->physical_address = STK1000_SDRAM_BASE;
  info->cols = mt481c2m32b2tg_cols;
  info->rows = mt481c2m32b2tg_rows;
  info->banks = mt481c2m32b2tg_banks;
  info->cas = mt481c2m32b2tg_cas;
  info->twr = mt481c2m32b2tg_twr;
  info->trc = mt481c2m32b2tg_trc;
  info->trp = mt481c2m32b2tg_trp;
  info->trcd = mt481c2m32b2tg_trcd;
  info->tras = mt481c2m32b2tg_tras;
  info->txsr = mt481c2m32b2tg_txsr;

  // Calculate sdram size 
  sdram_size = 1 << (info->rows + info->cols + info->banks + 2);

  printf("\nmy EXTERNAL SDRAM TEST\n\n");
  sdramc_init(info);
  mt481c2m32b2tg_init(info);

  printf("Initialization complete\nTest starting...\n");
  printf("Filling memoryspace %0x:(0x%lx)\n", sdram,sdram_size);

  for (i = 0; i < sdram_size; i++){
    sdram[i] = i;
    tmp = sdram[i];
    if (tmp != i) {
      printf("SDRAM verifcation failed at 0x%lx", STK1000_SDRAM_BASE + i);
      printf(". Read 0x%lx instead of expected 0x%lx\n", tmp, i);
      noErrors++;
    }
  }

  if(noErrors==0){
    printf("SDRAM test completed successfully\n");
  }else{
    printf("SDRAM test completed with %x errors", noErrors);
  }


// while(1)	{
printf("give an ram address: ");
while((c=conIn()) != 0x0d) { conOut(c);addr=16*addr+ascii2Hex(c);}
 conOut(0x0a);conOut(0x0d);
 sram=(int*)addr;
for (i=0; i < 256;i++)
*(sram+i)=i;
for (i=0; i < 256;i++)
*(sram+256 +i)=256*i;

//printf("%x\n",SDRAM);
printf("give an address: ");
 while((c=conIn()) != 0x0d) { conOut(c);addr=16*addr+ascii2Hex(c);}
 conOut(0x0a);conOut(0x0d);

for ( i=0; i < 16; i++)	{
wordConOut(addr+4*(8*i));
conOut(':');
conOut(' ');
for (j=0; j <8;j++)	
{wordConOut(*((int*)addr+8*i+j));conOut(' ');}
//for (j=0; j <32;j++)	
//{conOut(((*((char*)addr+32*i+j))>0x20)?(*((char*)addr+32*i+j)):'.');}
conOut(0x0a);conOut(0x0d);
}
#endif

#ifdef NGW100

#define CFG_SDRAM_BASE 0x10000000
static const struct sdram_info sdram = {
	.phys_addr	= CFG_SDRAM_BASE,
	.row_bits	= 13,
	.col_bits	= 9,
	.bank_bits	= 2,
	.cas		= 3,
	.twr		= 2,
	.trc		= 7,
	.trp		= 2,
	.trcd		= 2,
	.tras		= 5,
	.txsr		= 5,
};
	hmatrix2_writel(SFR4, 1 << 1);

	gpio_enable_ebi();
	gpio_enable_usart1();
#define CONFIG_MACB
#define CONFIG_MMC
#if defined(CONFIG_MACB)
	gpio_enable_macb0();
	gpio_enable_macb1();
#endif
#if defined(CONFIG_MMC)
	gpio_enable_mmci();
#endif
sdram_init(&sdram);

volatile unsigned long *sram = (void *) CFG_SDRAM_BASE;
  // Calculate sdram size 
int  sdram_size = 1 << (sdram.row_bits + sdram.col_bits + sdram.bank_bits + 2);
int j;


int noErrors=0;
  printf("Initialization complete\nTest starting...\n");
  printf("Filling memoryspace %0x:(0x%lx)\n", sram,sdram_size);
int tmp,i;
  for (i = 0; i < sdram_size; i++){
    sram[i] = i;
    tmp = sram[i];
    if (tmp != i) {
      printf("SDRAM verifcation failed at 0x%lx", CFG_SDRAM_BASE + i);
      printf(". Read 0x%lx instead of expected 0x%lx\n", tmp, i);
      noErrors++;
    }
  }
  if(noErrors==0){
    printf("SDRAM test completed successfully\n");
  }else{
    printf("SDRAM test completed with %x errors", noErrors);
  }

// while(1)	{
printf("give an ram address:(write) ");
char c;
int addr;
while((c=conIn()) != 0x0d) { conOut(c);addr=16*addr+ascii2Hex(c);}
 conOut(0x0a);conOut(0x0d);
 sram=(int*)addr;
for (i=0; i < 256;i++)
*(sram+i)=i;
for (i=0; i < 256;i++)
*(sram+256 +i)=256*i;

//printf("%x\n",SDRAM);
printf("give an address:(read) ");
 while((c=conIn()) != 0x0d) { conOut(c);addr=16*addr+ascii2Hex(c);}
 conOut(0x0a);conOut(0x0d);

for ( i=0; i < 16; i++)	{
wordConOut(addr+4*(8*i));
conOut(':');
conOut(' ');
for (j=0; j <8;j++)	
{wordConOut(*((int*)addr+8*i+j));conOut(' ');}
//for (j=0; j <32;j++)	
//{conOut(((*((char*)addr+32*i+j))>0x20)?(*((char*)addr+32*i+j)):'.');}
conOut(0x0a);conOut(0x0d);
}

#endif





}
		
void VT102Attribute (u1 fgcolor, u1 bgcolor)	{
    printf("%c",0x1b);
      printf("%c",'[');
//            printf("%c",'4');
     printf("%c",fgcolor);
      //printf("%c",';');
      //printf("%c",40 + bgcolor);
      printf("%c",'m');
}
