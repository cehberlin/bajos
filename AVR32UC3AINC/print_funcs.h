/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Strings and integers print module for debug purposes.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USART module can be used.
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2007, Atmel Corporation All rights reserved.
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


#ifndef _PRINT_FUNCS_H_
#define _PRINT_FUNCS_H_

#include <avr32/io.h>
#include "board.h"


/*! \name USART Settings for the Debug Module
 */
//! @{
#if BOARD == EVK1100
#  define DBG_USART               (&AVR32_USART1)
#  define DBG_USART_RX_PIN        AVR32_USART1_RXD_0_PIN
#  define DBG_USART_RX_FUNCTION   AVR32_USART1_RXD_0_FUNCTION
#  define DBG_USART_TX_PIN        AVR32_USART1_TXD_0_PIN
#  define DBG_USART_TX_FUNCTION   AVR32_USART1_TXD_0_FUNCTION
#  define DBG_USART_BAUDRATE      57600
#elif BOARD == EVK1101
#  define DBG_USART               (&AVR32_USART1)
#  define DBG_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
#  define DBG_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
#  define DBG_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
#  define DBG_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
#  define DBG_USART_BAUDRATE      57600
#endif
//! @}


/*! \brief Sets up DBG_USART with 57600 8N1.
 *
 * \param pba_hz PBA clock frequency (Hz).
 */
extern void init_dbg_rs232(long pba_hz);

/*! \brief Prints a string of characters to DBG_USART.
 *
 * \param str The string of characters to print.
 */
extern void print_dbg(const char *str);

/*! \brief Prints a character to DBG_USART.
 *
 * \param c The character to print.
 */
extern void print_dbg_char(int c);

/*! \brief Prints an integer to DBG_USART in a decimal representation.
 *
 * \param n The integer to print.
 */
extern void print_dbg_ulong(unsigned long n);

/*! \brief Prints an integer to DBG_USART in an hexadecimal representation.
 *
 * \param n The integer to print.
 */
extern void print_dbg_hex(unsigned long n);

/*! \brief Prints a string of characters to a given USART.
 *
 * \param usart Base address of the USART instance to print to.
 * \param str The string of characters to print.
 */
extern void print(volatile avr32_usart_t *usart, const char *str);

/*! \brief Prints a character to a given USART.
 *
 * \param usart Base address of the USART instance to print to.
 * \param c The character to print.
 */
extern void print_char(volatile avr32_usart_t *usart, int c);

/*! \brief Prints an integer to a given USART in a decimal representation.
 *
 * \param usart Base address of the USART instance to print to.
 * \param n The integer to print.
 */
extern void print_ulong(volatile avr32_usart_t *usart, unsigned long n);

/*! \brief Prints an integer to a given USART in an hexadecimal representation.
 *
 * \param usart Base address of the USART instance to print to.
 * \param n The integer to print.
 */
extern void print_hex(volatile avr32_usart_t *usart, unsigned long n);


#endif  // _PRINT_FUNCS_H_
