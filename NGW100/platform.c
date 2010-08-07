
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

#define __raw_writel(v,a)	(*(volatile unsigned int   *)(a) = (v))
#define writel(v,a)		__raw_writel(v,a)
#define HMATRIX_BASE		0xFFF00800
#define __raw_readl(a)		(*(volatile unsigned int   *)(a))
#define readl(a)		__raw_readl(a)
#define P2SEG			0xa0000000
#define P2SEGADDR(a) 		((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P2SEG))
#define uncached(addr)		((void *)P2SEGADDR(addr))

#define NB_CLOCK_CYCLE_DELAY_SHORTER    10000   // 1 ms if fCPU==20MHz
#define NB_CLOCK_CYCLE_DELAY_SHORT    1000000   // 100 ms if fCPU==20MHz
#define NB_CLOCK_CYCLE_DELAY_LONG    20000000   // 1 s if fCPU==20MHz

// COUNT/COMPARE match interrupt handler
// GCC-specific syntax to declare an interrupt handler. The interrupt handler
// registration is done in the main function using the INTC software driver module.
__attribute__((__interrupt__))	static void compare_irq_handler(void)	{
  // Count the number of times this IRQ handler is called.
  //u32NbCompareIrqTrigger++;
  // Clear the pending interrupt(writing a value to the COMPARE register clears
  // any pending compare interrupt requests). Schedule the COUNT&COMPARE match
  // interrupt to happen every NB_CLOCK_CYCLE_DELAY_LONG cycles.
  // AP7000 don't reset COUNT on compare match. We need to offset next COMPARE.
   U32 next_compare = Get_sys_compare()+NB_CLOCK_CYCLE_DELAY_SHORTER;
 // Avoid disabling compare			
  Set_sys_compare((next_compare==0)?1:next_compare);
  timerMilliSec++;
}

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
//MT481C2M16B2TG SDRAM 
hmatrix2_writel(SFR4, 1 << 1);
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
   U32 next_compare = Get_sys_compare()+NB_CLOCK_CYCLE_DELAY_SHORTER;
 // Avoid disabling compare			
  Set_sys_compare((next_compare==0)?1:next_compare); // GO
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
  // Initialize it in RS232 mode
  usart_init_rs232(usart, &opt, cpu_hz);
pio_enable_module(usart_piomap,
                      sizeof(usart_piomap) / sizeof(usart_piomap[0]));
}
