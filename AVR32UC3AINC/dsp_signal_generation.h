/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief List of currently implemented signal generation functions
 *
 * This file show a list of the signal generation functions currently implemented in the DSP library.
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

#ifndef __DSP_SIGNAL_GENERATION_H
#define __DSP_SIGNAL_GENERATION_H

/*! \name Sinusoidal
 * \brief This function generate a sinusoidal signal with a specified frequency,
 * sampling frequency and phase.
 * \note The phase is scaled to fit in the range [-1; 1], where -1 corresponds to -pi and 1 to pi.
 */
//! \{
/*! \brief 16-bit fixed point version of the sinusoidal signal generation.
 * \ingroup group_signal_generation
 * \param vect1 A pointer on a 16-bit vector which is the output buffer of this function.
 * \param size The number of elements of the output vector.
 * \param f The frequency of the output signal.
 * \param fs The sampling frequency of the output signal.
 * \param phase The phase of the output signal.
 */
void dsp16_gen_sin(dsp16_t *vect1, int size, int f, int fs, dsp16_t phase);
/*! \brief 32-bit fixed point version of the sinusoidal signal generation.
 * \ingroup group_signal_generation
 * \param vect1 A pointer on a 32-bit vector which is the output buffer of this function.
 * \param size The number of elements of the output vector.
 * \param f The frequency of the output signal.
 * \param fs The sampling frequency of the output signal.
 * \param phase The phase of the output signal.
 */
void dsp32_gen_sin(dsp32_t *vect1, int size, int f, int fs, dsp32_t phase);
//! \}

/*! \name Cosinusoidal
 * \brief This function generate a cosinusoidal signal with a specified frequency,
 * sampling frequency and phase.
 * \note The phase is scaled to fit in the range [-1; 1], where -1 corresponds to -pi and 1 to pi.
 */
//! \{
/*! \brief 16-bit fixed point version of the cosinusoidal signal generation.
 * \ingroup group_signal_generation
 * \param vect1 A pointer on a 16-bit vector which is the output buffer of this function.
 * \param size The number of elements of the output vector.
 * \param f The frequency of the output signal.
 * \param fs The sampling frequency of the output signal.
 * \param phase The phase of the output signal.
 */
void dsp16_gen_cos(dsp16_t *vect1, int size, int f, int fs, dsp16_t phase);
/*! \brief 32-bit fixed point version of the cosinusoidal signal generation.
 * \ingroup group_signal_generation
 * \param vect1 A pointer on a 32-bit vector which is the output buffer of this function.
 * \param size The number of elements of the output vector.
 * \param f The frequency of the output signal.
 * \param fs The sampling frequency of the output signal.
 * \param phase The phase of the output signal.
 */
void dsp32_gen_cos(dsp32_t *vect1, int size, int f, int fs, dsp32_t phase);
//! \}

/*! \name Noise
 * \brief This function generate a noise  with a specified amplitude.
 */
//! \{
/*! \brief 16-bit fixed point version of the noise generation.
 * \ingroup group_signal_generation
 * \param vect1 A pointer on a 16-bit vector which is the output buffer of this function.
 * \param size The number of elements of the output vector.
 * \param amp The amplitude of the signal.
 */
void dsp16_gen_noise(dsp16_t *vect1, int size, dsp16_t amp);
/*! \brief 32-bit fixed point version of the noise generation.
 * \ingroup group_signal_generation
 * \param vect1 A pointer on a 32-bit vector which is the output buffer of this function.
 * \param size The number of elements of the output vector.
 * \param amp The amplitude of the signal.
 */
void dsp32_gen_noise(dsp32_t *vect1, int size, dsp32_t amp);
//! \}

#endif
