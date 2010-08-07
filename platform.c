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
#include "NGW100/hsdramc1.h"
#include <asm/io.h>
#include <asm/sdram.h>
#include <asm/arch/gpio.h>
#include <asm/arch/hmatrix2.h>
unsigned long sdram_init(const struct sdram_info *info);

#endif
#include <avr32/io.h>
#endif
 
#ifdef EVK1100
#include "EVK1100/gpiouc3a.h"
#include "EVK1100/evk1100.h"
#include "EVK1100/dip204.h"
#include "EVK1100/countuc3a.h"
#include "EVK1100/rtcuc3a.h"
#include "EVK1100/intcuc3a.h"
#include "EVK1100/sdramc.h"
#endif
#include "platform.h"
#ifdef AVR8
#include <avr/io.h>
#include <avr/interrupt.h>
#include "AVR8/lcd.h"
#define		BYTES(word)			((word)*2)
#define		STRING(a,b)				#a" "#b
#define		INLINEASM(a,b)			STRING(a,b)
#define		LARGEBOOTSTART	0xf000
#define		BOARDRAMEND	0x8000
#define		MONSTART	LARGEBOOTSTART
#define 	MONRAM		CHARONRAMEND-0x100
// Monitorfunktionen (bamo128)
#define		mainLoop		BYTES(LARGEBOOTSTART+2)	// Ruecksprung in Monitor aus Programm mit goto
#define		saveCPU		BYTES(LARGEBOOTSTART+62)	//Time2Comp	// BOOTSTART+62		
FILE uartAVR8 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
SIGNAL(SIG_OUTPUT_COMPARE2) __attribute__ ((naked));
SIGNAL(SIG_OUTPUT_COMPARE2) {asm volatile  (INLINEASM(jmp,saveCPU));} // monitor für step-betrieb
#endif

void initHW(){
#ifdef AVR8
	stdout = stdin=stderr = &uartAVR8;	
LCD_Init();
timer_Init();	// use timer 2 for real time clock
#endif


#if  (AVR32UC3A|| AVR32AP7000)
usartInit();
stdIOInit();
#ifdef AVR32UC3A 
initTimer();
#endif
#endif

#ifdef EVK1100
sdramc_init(FOSC0);
#endif

#ifdef STK1000
  static sdram_info *info;
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
//  sdram_size = 1 << (info->rows + info->cols + info->banks + 2);

  sdramc_init(info);
  mt481c2m32b2tg_init(info);
#endif

#ifdef NGW100

static const struct sdram_info sdram = {
	.phys_addr	= NGW_SDRAM_BASE,
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
#endif
}
		

#ifdef AVR32UC3A
/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: void rtc_irq(void);
## Parameters: none
## Return Value: none
##
## Interrupt handler for Timer
*/
#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
/* RTC Interrupt  */
#pragma handler = RTC_INT_GROUP, 1
__interrupt
#endif
void rtc_irq(){
  timerSec++;
  // clear the interrupt flag
  rtc_clear_interrupt(&AVR32_RTC);
}
/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: void initTimer(void);
## Parameters: none
## Return Value: none
##
## start the timer, set the interrupt handling method
*/
void initTimer() 
{
  // Disable all interrupts. */
  Disable_global_interrupt();
  // The INTC driver has to be used only for GNU GCC for AVR32.
#if __GNUC__
  // Initialize interrupt vectors.
  INTC_init_interrupts();
  // Register the RTC interrupt handler to the interrupt controller.
  INTC_register_interrupt(&rtc_irq, AVR32_RTC_RTC_IRQ, INT0);
#endif
  // Initialize the RTC
  if (!rtc_init(&AVR32_RTC, RTC_OSC_32KHZ,4))// 1khz
  {
    //usart_write_line(&AVR32_USART0, "Error initializing the RTC\r\n");
    printf("Error initializing the RTC\r\n");
    while(1);
  }
  // Set top value to 0 to generate an interruption every seconds */
  rtc_set_top_value(&AVR32_RTC, 0);
  // Enable the interruptions
  rtc_enable_interrupt(&AVR32_RTC);
  // Enable the RTC
  rtc_enable(&AVR32_RTC);
  // Enable global interrupts
  Enable_global_interrupt();

  return 0;
}
#endif
void VT102Attribute (u1 fgcolor, u1 bgcolor)	{
    printf("%c",0x1b);
      printf("%c",'[');
//            printf("%c",'4');
     printf("%c",fgcolor);
      //printf("%c",';');
      //printf("%c",40 + bgcolor);
      printf("%c",'m');
}


#ifdef AVR8
void timer_Init()	{
OCR0=0x58;		// Timer zählt bis OCR0, dann Int und Reset Timer, Experimentell bestimmt
		TCCR0 = (1<<CS02) | (1<<CS00) | (1<<WGM01);	// Vorteiler 1024 zu Systemtakt 20 MHz??
		TIMSK |= (1<<OCIE0);
		sei();	// global interrupt zulassen
}
char currentTimeMillis()	{
opStackPush((slot)(u4)timerMilliSec);
	return 1;
}
SIGNAL(SIG_OUTPUT_COMPARE0)		{
	timerMilliSec++;	
}

	int uart_putchar(char c, FILE *stream){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	if (c=='\n') uart_putchar('\r',stream);
	return 0;								}

int uart_getchar(FILE *stream)	{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return (int)UDR0;	
}
#endif
