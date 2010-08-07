/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief SSC USART example driver.
 *
 * This file defines a useful set of functions for the SSC USART interface on
 * AVR32 devices. The driver handles normal polled usage and direct memory
 * access (PDC) usage.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a SSC module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 *****************************************************************************/

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


#ifndef _AVR32_SSC_USART_H_
#define _AVR32_SSC_USART_H_

#include <avr32/io.h>


/* Parameter defines */
#define SSC_UART_STOPBITS_ONE 0
#define SSC_UART_STOPBITS_TWO 1
#define SSC_UART_STOPBITS_ONE_AND_HALF 2

#define SSC_UART_PARITY_NONE 0
#define SSC_UART_PARITY_EVEN 1
#define SSC_UART_PARITY_ODD 2
#define SSC_UART_PARITY_SPACE 3
#define SSC_UART_PARITY_MARK 4

#define SSC_UART_SUCCESS 0
#define SSC_UART_FAILURE -1
#define SSC_UART_BAD_ARGS -2
#define SSC_UART_TIMEOUT -3


#define SSC_UART_LOOPBACK_MODE_DISABLE	0
#define SSC_UART_LOOPBACK_MODE_ENABLE	1

/* Default value used in blocking read/write functions */
#define SSC_UART_DEFAULT_TIMEOUT 10000



/*! \brief Sets up an SSC as UART
 *
 * Initializes an SSC for use as USART, by setting appropriate registers
 *
 * \param ssc Pointer to base address of SSC.
 * \param baud_rate Desired baud rate
 * \param stop_bits Stop bit setting.
 *                  \arg SSC_UART_STOPBITS_ONE One stop bit.
 *                  \arg SSC_UART_STOPBITS_ONE_AND_HALF 1.5 stop bits.
 *                  \arg SSC_UART_STOPBITS_TWO Two stop bits.
 * \param parity_bits Parity bit setting.
 *                  \arg SSC_UART_PARITY_NONE No parity bit.
 *                  \arg SSC_UART_PARITY_ODD Odd parity bit.
 *                  \arg SSC_UART_PARITY_EVEN Even parity bit.
 *                  \arg SSC_UART_PARITY_SPACE Parity bit always 0.
 *                  \arg SSC_UART_PARITY_MARK Parity bit always 1.
 * \param char_len  Number of bits per symbol. May be 5,6,7,8 or 9.
 * \param loop_back Loopback mode enable/disable (\ref SSC_UART_LOOPBACK_MODE_ENABLE or \ref SSC_UART_LOOPBACK_MODE_DISABLE)
 * \param pbaHz     PBA frequency (in Herz)
 *
 * \retval SSC_SUCCESS On success.
 * \retval SSC_BAD_ARGS On invalid arguments.
 *
 */
extern int ssc_uart_init(volatile avr32_ssc_t  *ssc, int baud_rate, int stop_bits, int parity_bits,
  int char_len,int loop_back,long pbaHz );

/*! \brief Writes a character to SSC UART
 *
 * This function writes a character to SSC UART. It waits a short while if UART
 * is not ready to send, if still not ready it returns a timeout.
 *
 * \param ssc Pointer to base address of SSC.
 * \param ch Character to send
 *
 * \retval SSC_UART_SUCCESS Character was successfully sent
 * \retval SSC_UART_TIMEOUT Timed out while trying to send.
 *
 */
extern int ssc_uart_putchar(volatile avr32_ssc_t  *ssc, unsigned short ch);

/*! \brief Reads a character from SSC UART.
 *
 * This function reads a character from SSC UART. It starts a busy waiting loop
 * and there is no timeout.
 *
 * \param ssc Pointer to base address of SSC.
 *
 * \retval SSC_RX_ERROR Buffer underrun error.
 * \return Returns character read.
 *
 */
extern int ssc_uart_getchar(volatile avr32_ssc_t  *ssc);

/*! \brief Resets the error flags of the SSC UART.
 *
 * This function will reset the underrun-, parity- and framing-error flags
 *
 * \param ssc Pointer to base address of SSC.
 *
 */
extern void ssc_uart_reset_status(volatile avr32_ssc_t  * ssc);


/*! \brief Returns whether or not a parity error has occured.
 *
 * \param ssc Pointer to base address of SSC.
 *
 * \retval 0 No parity error has occured.
 * \retval 1 A parity error has occured.
 *
 */
extern int ssc_uart_parity_error(volatile avr32_ssc_t  * ssc);


/*! \brief Returns whether or not a framing error has occured.
 *
 * \param ssc Pointer to base address of SSC.
 *
 * \retval 0 No framing error has occured.
 * \retval 1 A framing error has occured.
 *
 */
extern int ssc_uart_framing_error(volatile avr32_ssc_t  * ssc);


/*! \brief Returns whether or not a overrun error has occured.
 *
 * \param ssc Pointer to base address of SSC.
 *
 * \retval 0 No overrun error has occured.
 * \retval 1 A overrun error has occured.
 *
 */
extern int ssc_uart_overrun_error(volatile avr32_ssc_t * ssc);



#endif
