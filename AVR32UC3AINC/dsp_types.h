/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Fixed Point DSP library data type definition
 *
 * This file defines the types used in the Fixed Point DSP library
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices.
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

#ifndef __DSP_TYPES_H
#define __DSP_TYPES_H

#include "compiler.h"

#if __GNUC__
//! \brief Alignment attribute on 4 bytes for 32 bits or lower microcontroller optimization for GCC compiler
#  define A_ALIGNED	__attribute__ ((aligned(4)))
//! \brief Packed data attribute for GCC compiler
#  define A_PACKED		__attribute__ ((__packed__))
#elif __ICCAVR32__
#  define A_ALIGNED      _Pragma("data_alignment=4")
#  define A_PACKED
#else
#  error Unknown compiler
#endif

/*! \name Q Format definition for the 16 bits type
 * \ingroup group_qformat
 * \note The 16 bits functions are optimized to work with a Q1.15 format:\n
 * Resolution: 0.00003\n
 * Range: [-1.; 0.99997]
 */
//!  @{
#ifndef DSP16_FORMAT
#  define DSP16_QA	1
#  define DSP16_QB	15
#else
#  define DSP16_QA	(16 - DSP16_FORMAT)
#  define DSP16_QB	DSP16_FORMAT
#endif
//!  @}
#if (DSP16_QA + DSP16_QB) != 16
#	error in the definition of the 16 bits type. The sum of the "Q format" coefficients should be 16.
#endif

/*! \name Q Format definition for the 32 bits type
 * \ingroup group_qformat
 * \note The 32 bits functions are optimized to work with a Q1.31 format:\n
 * Resolution: 0.00000000047\n
 * Range: [-1.; 0.99999999953]
 */
//! @{
#ifndef DSP32_FORMAT
#  define DSP32_QA	1
#  define DSP32_QB	31
#else
#  define DSP32_QA	(32 - DSP32_FORMAT)
#  define DSP32_QB	DSP32_FORMAT
#endif
//! @}
#if (DSP32_QA + DSP32_QB) != 32
#	error in the definition of the 32 bits type. The sum of the "Q format" coefficients should be 32.
#endif

//! \name Basic types
//! \{
#ifndef U64
/*! \brief unsigned 64-bit type
 * \ingroup group_types
*/
#  define U64  unsigned long long
#endif
#ifndef S64
/*! \brief signed 64-bit type
 * \ingroup group_types
*/
#  define S64  long long
#endif
#ifndef U32
/*! \brief unsigned 32-bit type
 * \ingroup group_types
*/
#  define U32  unsigned int
#endif
#ifndef S32
/*! \brief signed 32-bit type
 * \ingroup group_types
*/
#  define S32  int
#endif
#ifndef U16
/*! \brief unsigned 16-bit type
 * \ingroup group_types
*/
#  define U16  unsigned short
#endif
#ifndef S16
/*! \brief signed 16-bit type
 * \ingroup group_types
*/
#  define S16  short
#endif
#ifndef U8
/*! \brief unsigned 8-bit type
 * \ingroup group_types
*/
#  define U8   unsigned char
#endif
#ifndef S8
/*! \brief signed 8-bit type
 * \ingroup group_types
*/
#  define S8   char
#endif
//! \}


/*! \ingroup group_types
 * \brief 16 bits signed fixed point type
 */
#define dsp16_t__	S16
/*! \ingroup group_types
 * \brief 16 bits signed fixed point type optimized for 32 bits or lower microcontrollers
 */
typedef dsp16_t__ dsp16_t;

/*! \ingroup group_types
 * \brief 32 bits signed fixed point type
 */
#define dsp32_t__	S32
/*! \ingroup group_types
 * \brief 32 bits signed fixed point type optimized for 32 bits or lower microcontrollers
 */
typedef dsp32_t__ dsp32_t;

/*! \ingroup group_types
 * \brief 16 bits complex signed fixed point type
 */
#if __ICCAVR32__
#	pragma pack(1)
#endif
typedef struct A_PACKED dsp16_complex_t
{
    //! real part
	dsp16_t__ real;
    //! imaginary part
	dsp16_t__ imag;
}dsp16_complex_t;
#if __ICCAVR32__
#	pragma pack()
#endif

/*! \ingroup group_types
 * \brief 32 bits complex signed fixed point type
 */
#if __ICCAVR32__
#	pragma pack(1)
#endif
typedef struct A_PACKED dsp32_complex_t
{
    //! real part
	dsp32_t__ real;
    //! imaginary part
	dsp32_t__ imag;
}dsp32_complex_t;
#if __ICCAVR32__
#	pragma pack()
#endif

/*! \name Limit values in float type of a signed fixed point number typed Qa.b
 * \ingroup group_macros
 */
//! \{
//! \brief Return the maximum value
#define DSP_FP_MAX(a, b)    (((float) (1 << ((a)-1))) - 1./((float) (1 << (b))))
//! \brief Return the maximum value
#define DSP_FP_MIN(a, b)    (-((float) (1 << ((a)-1))))
//! \}

/*! \name Limit values in fixed point type of a signed fixed point number typed Qa.b
 * \ingroup group_macros
 */
//! \{
//! \brief Return the maximum value
#define DSP_Q_MAX(a, b)        (((unsigned int) -1) >> (sizeof(int)*8 - ((a)+(b)-1)))
//! \brief Return the maximum value
#define DSP_Q_MIN(a, b)        (1 << ((a)+(b)-1))
//! \}

/*! \name Convertion macro from a float to a Q Formated value
 * \note This macro limits the number in its range
 */
//! \{
/*! \ingroup group_macros
 * \brief Convert a float into a signed fixed-point typed Qa.b 
 */
#define DSP_Q(a, b, fnum)        (((fnum) > DSP_FP_MAX(a, b))?\
                            	DSP_Q_MAX(a, b):\
                                (((fnum) < DSP_FP_MIN(a, b))?\
                            	DSP_Q_MIN(a, b):\
                                (((fnum)*(((unsigned) (1 << (b))))))))
/*! \ingroup group_macros
 * \brief Convert a float into a 16 bits signed fixed-point type
 */
#define DSP16_Q(fnum)            ((dsp16_t__) DSP_Q(DSP16_QA, DSP16_QB, fnum))
/*! \ingroup group_macros
 * \brief Convert a float into a 32 bits signed fixed-point type
 */
#define DSP32_Q(fnum)            ((dsp32_t__) DSP_Q(DSP32_QA, DSP32_QB, fnum))
//! \}





/*! \ingroup group_constants
 * \brief PI definition
 */
#define PI	3.141592653589793238462643383279502884197

/*! \ingroup group_constants
 * \brief PI/4 definition for the 16 bits fixed point type
 */
#define DSP16_PI_DIV_4	DSP16_Q(PI/4.)
/*! \ingroup group_constants
 * \brief PI/4 definition for the 32 bits fixed point type
 */
#define DSP32_PI_DIV_4	DSP32_Q(PI/4.)



#endif
