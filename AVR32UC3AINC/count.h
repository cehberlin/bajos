/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Cycle counter driver.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32UC devices.
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


#ifndef _CYCLE_COUNTER_H_
#define _CYCLE_COUNTER_H_

#include "compiler.h"


#if __GNUC__ || __ICCAVR32__
#  define Get_sys_count()     ( Get_system_register(AVR32_COUNT)        )
#  define Set_sys_count(x)    ( Set_system_register(AVR32_COUNT,   (x)) )
#  define Get_sys_compare()   ( Get_system_register(AVR32_COMPARE)      )
#  define Set_sys_compare(x)  ( Set_system_register(AVR32_COMPARE, (x)) )

#else
/*!
 * \brief Returns the system counter value
 */
extern   U32 get_sys_count( void );

/*!
 * \brief Set the system counter value
 */
extern   void set_sys_count( U32 x );

/*!
 * \brief Returns the system compare value
 */
extern   U32 get_sys_compare( void );

/*!
 * \brief Set the system compare value
 */
extern   void set_sys_compare( U32 x );

#  define Get_sys_count()     ( get_sys_count()    )
#  define Set_sys_count(x)    ( Set_sys_count(x)   )
#  define Get_sys_compare()   ( get_sys_compare()  )
#  define Set_sys_compare(x)  ( set_sys_compare(x) )
#endif


#endif // _CYCLE_COUNTER_H_
