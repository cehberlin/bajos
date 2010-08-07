
// a bad and ugly hack!!
/* ************************************************************************ *\
Copyright (c) 2006, Atmel Corporation All rights reserved. 

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the 
following disclaimer. 

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. The name of ATMEL may not be used to endorse or promote products 
derived from this software without specific prior written permission.  

THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS 
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
PARTICULAR PURPOSE ARE EXPRESSLY AND SPECIFICALLY 
DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY 
WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 

POSSIBILITY OF SUCH DAMAGE. 

\* ************************************************************************ */
#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "pio.h"
#include "gpio.h"
#include "usart.h"
#include "sdram.h"
#include "hsdramc1.h"
#include "hmatrix2.h"
#include "intc.h"
#include "atngw100.h"
#include "pm.h"
#include "cycle_counter.h"
#include "platform.h"
#include "../definitions.h"
#include "../bajvm.h"

#define __raw_writel(v,a)       (*(volatile unsigned int   *)(a) = (v))
#define writel(v,a)               __raw_writel(v,a)
#define HMATRIX_BASE                               0xFFF00800
#define __raw_readl(a)          (*(volatile unsigned int   *)(a))
#define readl(a)              __raw_readl(a)
#define P2SEG              0xa0000000
#define P2SEGADDR(a) ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P2SEG))
#define uncached(addr) ((void *)P2SEGADDR(addr))



#define NB_CLOCK_CYCLE_DELAY_SHORTER    10000   // 1 ms if fCPU==20MHz
#define NB_CLOCK_CYCLE_DELAY_SHORT    1000000   // 100 ms if fCPU==20MHz
#define NB_CLOCK_CYCLE_DELAY_LONG    20000000   // 1 s if fCPU==20MHz

// COUNT/COMPARE match interrupt handler
// GCC-specific syntax to declare an interrupt handler. The interrupt handler
// registration is done in the main function using the INTC software driver module.
__attribute__((__interrupt__))	static void compare_irq_handler(void)
{

  // Count the number of times this IRQ handler is called.
 //u32NbCompareIrqTrigger++;
  // Clear the pending interrupt(writing a value to the COMPARE register clears
  // any pending compare interrupt requests). Schedule the COUNT&COMPARE match
  // interrupt to happen every NB_CLOCK_CYCLE_DELAY_LONG cycles.
  U32 next_compare;
  // AP7000 don't reset COUNT on compare match. We need to offset next COMPARE.
  next_compare = Get_sys_compare();
  next_compare += NB_CLOCK_CYCLE_DELAY_SHORTER;
  if (next_compare == 0) // Avoid disabling compare
    next_compare++;
  Set_sys_compare(next_compare);
timerMilliSec++;

}



/* \brief Software Delay
 *
 */
static void software_delay(void)
{
  volatile unsigned int i;
  for (i=0; i<2000000; i++);
}


 

/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: void rtc_irq(void);
## Parameters: none
## Return Value: none
##
## Interrupt handler for Timer
*/
/*
__attribute__((__interrupt__))	void rtc_irq(){
  timerMilliSec++;
  // clear the interrupt flag
  rtc_clear_interrupt(&AVR32_RTC);
}
*/
/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: void initTimer(void);
## Parameters: none
## Return Value: none
##
## start the timer, set the interrupt handling method
*/
/*
#define INT0          0 //!< Lowest interrupt priority level.
#include "rtc.h"
void initTimer() 
{
  // Disable all interrupts. 
  Disable_global_interrupt();
  // The INTC driver has to be used only for GNU GCC for AVR32.
#if __GNUC__
  // Initialize interrupt vectors.
//bh  INTC_init_interrupts();
  // Register the RTC interrupt handler to the interrupt controller.
 INTC_register_interrupt(&rtc_irq, AVR32_RTC_IRQ, INT0);
#endif
  // Initialize the RTC
//psel = log(Fosc/Frtc)/log(2)-1; Frtc wanted f, Fosc 32KHz
  if (!rtc_init(&AVR32_RTC, RTC_OSC_32KHZ,4))// 1khz
  {
    //usart_write_line(&AVR32_USART0, "Error initializing the RTC\r\n");
    {printf("Error initializing the RTC\r\n");
    exit(-1);	}
  }
  // Set top value to 0 to generate an interruption every Milli-seconds 
  rtc_set_top_value(&AVR32_RTC, 0);
  // Enable the interruptions
  rtc_enable_interrupt(&AVR32_RTC);
  // Enable the RTC
  rtc_enable(&AVR32_RTC);
  // Enable global interrupts
  Enable_global_interrupt();
}
*/
typedef char avr32_piomap_t[][2];

void initHW(){


   // Reset PM. Makes sure we get the expected clocking after a soft reset (e.g.: JTAG reset)
  pm_reset();
//init_ngw100(S190MHZ);
   // Switch the main clock to OSC0
   // pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
   //  pm_switch_to_osc0(pm, FOSC0, OSC0_STARTUP);

usartInit();
stdIOInit();

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
	.txsr		= 5,		};

/*
static const struct sdram_info sdram = {
	.physical_address	= NGW_SDRAM_BASE,
	.rows	= 13,
	.cols	= 9,
	.banks	= 2,
	.cas		= 3,
	.twr		= 2,
	.trc		= 7,
	.trp		= 2,
	.trcd		= 2,
	.tras		= 5,
	.txsr		= 5,
};
*/
//MT481C2M16B2TG SDRAM 

	hmatrix2_writel(SFR4, 1 << 1);
//	gpio_enable_ebi();
sdram_init(&sdram);

   // Disable all interrupts.
   Disable_global_interrupt();

   INTC_init_interrupts();

   // Register the compare interrupt handler to the interrupt controller.
   // compare_irq_handler is the interrupt handler to register.
   // AVR32_CORE_COMPARE_IRQ is the IRQ of the interrupt handler to register.
   // AVR32_INTC_INT0 is the interrupt priority level to assign to the group of this IRQ.
// void INTC_register_interrupt(__int_handler handler, unsigned int irq, unsigned int int_lev);
 INTC_register_interrupt(&compare_irq_handler, AVR32_CORE_COMPARE_IRQ, AVR32_INTC_INT0);

   // Enable all interrupts.
   Enable_global_interrupt();

printf(" starting time\n");
U32 u32CompareVal;
U32 u32CompareValVerif;
U32 u32CountVal;
U32 u32CountNextVal;

u32CountVal = Get_sys_count();

u32CompareVal = u32CountVal + NB_CLOCK_CYCLE_DELAY_SHORT; // WARNING: MUST FIT IN 32bits.
// If u32CompareVal ends up to be 0, make it 1 so that the COMPARE and exception
// generation feature does not get disabled.
if(0 == u32CompareVal)
{
u32CompareVal++;
}

Set_sys_compare(u32CompareVal); // GO

// Check if the previous write in the COMPARE register succeeded.
u32CompareValVerif = Get_sys_compare();
 if  (u32CompareVal==u32CompareValVerif ) printf("ist gleich\n");

//while(1){ software_delay();printf("%08x\n",timerMilliSec);}
}

char conIn()	{
return (char) usart_getchar(&AVR32_USART1);
}

#define FAILURE -1
#define SUCCESS 0
void conOut(char c)	 {
usart_bw_write_char(&AVR32_USART1, (int) c);
}

int __attribute__((weak)) _read (int file, char * ptr, int len){
int i;
//if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);
//if (file != 0)return unimplemented_syscall("_read with filedes != 0");
for ( i = 0; i < len; i++ )	ptr[i] = (char)conIn();
return len;
}

/**
* Low-level write command.
* When newlib buffer is full or fflush is called, this will output
* data to correct location.
* 1 and 2 is stdout and stderr which goes to usart
* 3 is framebuffer
*/
int __attribute__((weak)) _write (int file, char * ptr, int len){
int i;
//if ( !do_not_use_oscall_coproc ) return _write_sim(file, ptr, len);
//if ( (file != 1)&& (file != 2) && (file!=3)) return unimplemented_syscall("_write with file != 1 or 2");
/* if(file==3){
for(i = 0 ; i < len; i++){
avr32fb_putchar(ptr[i]);
}
} else */{
for ( i = 0; i < len; i++ ){
conOut(ptr[i]);
if (ptr[i]=='\n') conOut('\r');
}
}
return len;
} 

void stdIOInit()	{	
//To configure standard I/O streams as unbuffered, you can simply:
setbuf(stdin, NULL);
setbuf(stdout, NULL); 
}

void usartInit()	{
int cpu_hz = 20000000;
  struct usart_options_t opt;
  volatile struct avr32_usart_t *usart = &AVR32_USART1;
  avr32_piomap_t usart_piomap = {				   \
    {AVR32_USART1_RXD_0_PIN, AVR32_USART1_RXD_0_FUNCTION}, \
    {AVR32_USART1_TXD_0_PIN, AVR32_USART1_TXD_0_FUNCTION}   \
  };

  // Set options for the USART
  opt.baudrate = 115200;
  opt.charlength = 8;
  opt.paritytype = USART_NO_PARITY;
  opt.stopbits = USART_1_STOPBIT;
  opt.channelmode = USART_NORMAL_CHMODE;
//pm_reset();
  // Initialize it in RS232 mode
  usart_init_rs232(usart, &opt, cpu_hz);
pio_enable_module(usart_piomap,
                      sizeof(usart_piomap) / sizeof(usart_piomap[0]));
}
