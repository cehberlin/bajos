/*This file has been prepared for Doxygen automatic documentation generation.*/
/* Copyright (C) 2006-2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _pio_h_
#define _pio_h_

#ifdef __GNUC__
  #include <avr32/io.h>
#elif __ICCAVR32__
  #include <avr32/ioap7000.h>
#else
  #error No known compiler used
#endif

/*!
 * A type definitions of pins and module connectivity.
 * First column is the pin number, the second is pio connectivity
 */
typedef char avr32_piomap_t[][2];

typedef struct
{
  unsigned char pin;              
  unsigned char function;         
} gpio_map_t[];

#define PIO_INVALID_ARGUMENT -1
#define PIO_SUCCESS           0

extern volatile avr32_pio_t *pioGetHandle(int port);

extern int gpio_enable_module_pin(int pin, int function);

extern int pio_setup_pin(int pin, int function);

//extern void pio_enable_module(const gpio_map_t piomap, int size);

extern void gpio_enable_module(const gpio_map_t piomap, int size);

#endif
