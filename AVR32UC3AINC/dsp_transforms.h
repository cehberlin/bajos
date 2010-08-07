/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief List of currently implemented transform functions
 *
 * This file show a list of the transform functions currently implemented in the DSP library.
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

#ifndef __DSP_TRANSFORMS_H
#define __DSP_TRANSFORMS_H

/*! \name Complex Fast Fourier Transform
 * \brief This function computes a complex FFT from an input signal.
 * It uses the Radix-4 “Decimate In Time” algorithm and does not perform a calculation “in place” 
 * which means that the input buffer has to be different from the output buffer.
 * \note Those functions are affected by the optimization flags: DSP_OPTI_SPEED, DSP_OPTI_ACCURACY, DSP_OPTI_SIZE and DSP_OPTI_ACC_AND_SIZE.
 */
//! \{
/*! \brief 16 bits fixed point version of the complex FFT algorithm.
 * \ingroup group_transforms
 * \param vect1 A pointer on a 16 bits complex vector which is the output buffer of this function.
 * \param vect2 A pointer on a 16 bits real vector which is the input buffer of this function.
 * \param nlog It is the base-2-logarithm of the size of the input/output vector.\n
 * Due to its implementation, this function computes only 4^n-point complex FFT. Therefore, the \b nlog argument has to be even.
 * \note The size of the output buffer has to be the same as the size of the input buffer.\n
 * To avoid overflowing values, the resulting vector amplitude is scaled by 2^\b nlog.\n
 * This function uses a static twiddle factors table.
 */
void dsp16_trans_complexfft(dsp16_complex_t *vect1, dsp16_t *vect2, int nlog);
/*! \brief 32 bits fixed point version of the complex FFT algorithm.
 * \ingroup group_transforms
 * \param vect1 A pointer on a 32 bits complex vector which is the output buffer of this function.
 * \param vect2 A pointer on a 32 bits real vector which is the input buffer of this function.
 * \param nlog It is the base-2-logarithm of the size of the input/output vector.\n
 * Due to its implementation, this function computes only 4^n-point complex FFT. Therefore, the \b nlog argument has to be even.
 * \note The size of the output buffer has to be the same as the size of the input buffer.\n
 * To avoid overflowing values, the resulting vector amplitude is scaled by 2^\b nlog.\n
 * This function uses a static twiddle factors table.
 */
void dsp32_trans_complexfft(dsp32_complex_t *vect1, dsp32_t *vect2, int nlog);
//! \}

#endif
