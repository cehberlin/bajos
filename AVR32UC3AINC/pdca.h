/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief PDCA driver for AVR32 UC3.
 *
 * This file defines a useful set of functions for the PDCA interface on AVR32
 * devices.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with PDCA and USART modules.
 * - AppNote:
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


#ifndef _PDCA_H_
#define _PDCA_H_

#include <avr32/io.h>


//! PDCA size of transfer Byte.
#define PDCA_MODE_BYTE            AVR32_PDCA_BYTE

//! PDCA size of half-word transfer.
#define PDCA_MODE_HALF_WORD       AVR32_PDCA_HALF_WORD

//! PDCA size of word transfer.
#define PDCA_MODE_WORD            AVR32_PDCA_WORD

/*! \name PDCA Driver Status Codes
 */
//! @{
#define PDCA_SUCCESS 0
#define PDCA_INVALID_ARGUMENT -1
//! @}

/*! \name PDCA Transfer Status Codes
 */
//! @{
#define PDCA_TRANSFER_ERROR                   AVR32_PDCA_TERR_MASK
#define PDCA_TRANSFER_COMPLETE                AVR32_PDCA_TRC_MASK
#define PDCA_TRANSFER_COUNTER_RELOAD_IS_ZERO  AVR32_PDCA_RCZ_MASK
//! @}


//! PDCA channel options.
typedef struct
{
  //! Memory address.
  unsigned int  addr   ;
  //! Transfer counter.
  unsigned int  size   ;
  //! Next memory address.
  unsigned int  r_addr ;
  //! Next transfer counter.
  unsigned int  r_size ;
  //! Select peripheral ID.
  unsigned int  pid    ;
  //! Select the size of the transfer (byte, half-word or word).
  unsigned int  mode   ;
} pdca_channel_options_t;


/*! \brief Get PDCA channel handler
 *
 * \param pdca_ch_number  PDCA channel
 *
 * \return channel handled or PDCA_INVALID_ARGUMENT
 */
extern volatile avr32_pdca_channel_t *pdca_get_handler(int pdca_ch_number);

/*! \brief Set the channel configuration
 *
 * \param pdca_ch_number PDCA channel
 * \param opt channel option
 */
extern int pdca_init_channel(int pdca_ch_number, const pdca_channel_options_t *opt);

/*! \brief Disable the PDCA for the given channel
 *
 * \param pdca_ch_number PDCA channel
 */
extern void pdca_disable(int pdca_ch_number);

/*! \brief Enable the PDCA for the given channel
 *
 * \param pdca_ch_number PDCA channel
 */
extern void pdca_enable(int pdca_ch_number);

/*! \brief Get the PDCA channel status
 *
 * \param pdca_ch_number PDCA channel
 */
extern int pdca_get_channel_status(int pdca_ch_number);

/*! \brief Set the adddress of memory buffer
 *
 * \param pdca_ch_number PDCA channel
 * \param mem_add the memory address
 * \param transfer_counter Size of the transfer
 */
extern void pdca_set_memory_add(int pdca_ch_number, unsigned long mem_add, int transfer_counter);

/*! \brief Set the adddress of memory buffer
 *
 * \param pdca_ch_number PDCA channel
 * \param pid the peripheral ID
 */
extern void pdca_set_peripheral_select(int pdca_ch_number, int pid);

/*! \brief Set the size of the transfer
 *
 * \param pdca_ch_number PDCA channel
 * \param mode size of the transfer
 */
extern void pdca_set_mode(int pdca_ch_number, int mode);

/*! \brief Enable PDCA transfer interrupt when completed (ie TCR and TCRR are both zero)
 *
 * \param pdca_ch_number PDCA channel
 */
extern void pdca_enable_interrupt_transfer_complete(int pdca_ch_number);

/*! \brief Disable PDCA transfer interrupt when completed (ie TCR and TCRR are both zero)
 *
 * \param pdca_ch_number PDCA channel
 */
extern void pdca_disable_interrupt_transfer_complete(int pdca_ch_number);

/*! \brief Enable PDCA transfer interrupt when TCRR reaches zero
 *
 * \param pdca_ch_number PDCA channel
 */
extern void pdca_enable_interrupt_reload_counter_zero(int pdca_ch_number);

/*! \brief Didable PDCA transfer interrupt when TCRR reaches zero
 *
 * \param pdca_ch_number PDCA channel
 */
extern void pdca_disable_interrupt_reload_counter_zero(int pdca_ch_number);

/*! \brief Get PDCA channel transfer status
 *
 * \param pdca_ch_number PDCA channel
 *
 * \return PDCA transfer status with the following bit-masks:\n
 *           - \c PDCA_TRANSFER_ERROR;\n
 *           - \c PDCA_TRANSFER_COMPLETE;\n
 *           - \c PDCA_TRANSFER_COUNTER_RELOAD_IS_ZERO.
 */
extern unsigned int pdca_get_transfer_status(int pdca_ch_number);

/*! \brief Set PDCA channel reload values
 *
 * \param pdca_ch_number PDCA channel
 * \param address        address where data to load are stored
 * \param size           size of the data block to load
 */
extern void pdca_reload_channel(int pdca_ch_number, unsigned int address, unsigned int size);


#endif  // _PDCA_H_
