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
#include "compiler.h"
#include <asm-avr32/arch-at32ap700x/gpio.h>
#include <pm.h>
#include <../AVR32AP7000/usartap7000.h>
#include "hsdramc1.h"
#include <asm/io.h>
#include <asm/sdram.h>
#include <asm/arch/gpio.h>
#include <asm/arch/hmatrix2.h>
#include "../definitions.h"

unsigned long sdram_init(const struct sdram_info *info);


char conIn()	{
return (char) usart_getchar(&AVR32_USART1);
}

#define FAILURE -1
#define SUCCESS 0
void conOut(char c)	 {
usart_bw_write_char(&AVR32_USART1, (int) c);
}

int __attribute__((weak
)) _read (int file, char * ptr, int len){
int i;
//if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);
//if (file != 0)return unimplemented_syscall("_read with filedes != 0");
for ( i = 0; i < len; i++ ){
ptr[i] = (char)conIn();
}
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


typedef unsigned char avr32_piomap_t[][2];

/* enable output on pins */
int pio_enable_module(avr32_piomap_t piomap, unsigned int size);

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

  // Setup pio for USART
  pio_enable_module(usart_piomap, 2);
}


int pio_enable_module(avr32_piomap_t piomap, unsigned int size)
{
  int i;
  volatile struct avr32_pio_t *pio;

  /* get the base address for the port */
  switch (**piomap/32) {

  case 0:
    pio = &AVR32_PIOA;
    break;
  case 1:
    pio = &AVR32_PIOB;
    break;
  case 2:
    pio = &AVR32_PIOC;
    break;
  case 3:
    pio = &AVR32_PIOD;
    break;
  case 4:
    pio = &AVR32_PIOE;
    break;
  default :
    return FAILURE;

  }

  for(i=0; i<size; i++){

    pio->pdr |= ( 1<<( (**piomap) % 32) );
    pio->pudr |= ( 1<<( (**piomap) % 32) );

    switch( *(*piomap+1) ){    
    case 0:
      pio->asr |= ( 1<<( (**piomap) % 32) );
      break;
    case 1:
      pio->bsr |= ( 1<<( (**piomap) % 32) );
      break;
    default:
      return FAILURE;
    }

    ++piomap;

  }
  
  return SUCCESS;
}



void initHW(){
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
}


