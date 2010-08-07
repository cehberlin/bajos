/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief List of currently implemented operators
 *
 * This file show a list of the operators currently implemented in the DSP library.
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

#ifndef __DSP_OPERATORS_H
#define __DSP_OPERATORS_H

/*! \name Sine
 * \brief These functions compute the sine of the argument 'angle'. 
 * \note Angle is scaled to fit in the range [-1; 1], where -1 corresponds to -pi and 1 to pi.
 */
//! \{
/*! \brief 16-bit fixed point version of the sine function.
 * \ingroup group_operators
 * \param angle The angle to compute
 * \return The sine of 'angle' is returned.
 * \note If you are using a number greater than 1 (or lower than -1) for the angle,
 * the function will take the modulo 1 of this angle.
 */
dsp16_t dsp16_op_sin(dsp16_t angle);
/*! \brief 32-bit fixed point version of the sine function.
 * \ingroup group_operators
 * \param angle The angle to compute
 * \return The sine of 'angle' is returned.
 * \note If you are using a number greater than 1 (or lower than -1) for the angle,
 * the function will take the modulo 1 of this angle.
 */
dsp32_t dsp32_op_sin(dsp32_t angle);
//! \}

/*! \name Cosine
 * \brief These functions compute the cosine of the argument 'angle'. 
 * Angles are scale to fit in the range [-1; 1], where -1 corresponds to -pi and 1 to pi.
 */
//! \{
/*! \brief 16-bit fixed point version of the cosine function.
 * \ingroup group_operators
 * \param angle The angle to compute
 * \return The cosine of 'angle' is returned.
 * \note If you are using a number greater than 1 (or lower than -1) for the angle,
 * the function will take the modulo 1 of this angle.
 */
inline static dsp16_t dsp16_op_cos(dsp16_t angle) { return dsp16_op_sin(angle + DSP16_Q(0.5)); }
/*! \brief 32-bit fixed point version of the cosine function.
 * \ingroup group_operators
 * \param angle The angle to compute
 * \return The cosine of 'angle' is returned.
 * \note If you are using a number greater than 1 (or lower than -1) for the angle,
 * the function will take the modulo 1 of this angle.
 */
inline static dsp32_t dsp32_op_cos(dsp32_t angle) { return dsp32_op_sin(angle + DSP32_Q(0.5)); }
//! \}

/*! \name Random
 * \brief These functions generate a pseudo-radomized number
 */
//! \{
/*! \brief Initialize the pseudo-random number generator.
 * \ingroup group_operators
 * \param new_seed An integer value to be used as seed by the pseudo-random number generator algorithm.
 */
void dsp_op_srand(int new_seed);
/*! \brief 16-bit fixed point version of the random operator.
 * \ingroup group_operators
 * \return The cosine of 'angle' is returned.
 * \note The number generated can be any 16-bit value in the range [-1; 1[.
 */
dsp16_t dsp16_op_rand();
/*! \brief 32-bit fixed point version of the random operator.
 * \ingroup group_operators
 * \return The cosine of 'angle' is returned.
 * \note The number generated can be any 32-bit value in the range [-1; 1[.
 */
dsp32_t dsp32_op_rand();
//! \}

#endif
