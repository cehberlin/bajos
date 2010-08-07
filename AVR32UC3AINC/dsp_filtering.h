/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief List of currently implemented filters
 *
 * This file show a list of the filtering functions currently implemented in the DSP library.
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

#ifndef __DSP_FILTERING_H
#define __DSP_FILTERING_H

#include "dsp_vectors.h"

/*! \name Finite Impulse Response Filter
 * \brief This function computes a real FIR filter using the impulse response of the desire
 * filter onto a fixed-length signal. It returns a signal of a length equals to (size - h_size + 1) elements.
 * \note The impulse response of the filter has to be scaled to avoid overflowing values.\n
 * All the vectors have to be 32-bits aligned.
 */
//! \{
/*! \brief 16 bits fixed point version of the FIR .
 * \ingroup group_filtering
 * \param vect1 A pointer on a 16-bits fixed-point vector of (size - h_size + 1) elements corresponding to the output buffer.
 * \param vect2 A pointer on a 16-bits fixed-point vector of size elements corresponding to the input buffer.
 * \param size The length of the input buffer (must be greater or equals to 4).
 * \param h A pointer on a 16-bits fixed-point vector of h_size elements corresponding to the buffer containing the impulse response coefficients.
 * \param h_size The length of the impulse response of the filter (must be greater than 7)
 * \warning Due to its implementation, for the avr32-uc3 optimized version of the FIR,
 * the output buffer (vect1) have to have a length of 4*n elements to avoid overflows.
 * \note You need the "Partial Convolution" module
 */
inline static void dsp16_filt_fir(dsp16_t *vect1, dsp16_t *vect2, int size, dsp16_t *h, int h_size) { dsp16_vect_convpart(vect1, vect2, size, h, h_size); };
/*! \brief 32 bits fixed point version of the FIR .
 * \ingroup group_filtering
 * \param vect1 A pointer on a 32-bits fixed-point vector of (size - h_size + 1) elements corresponding to the output buffer.
 * \param vect2 A pointer on a 32-bits fixed-point vector of size elements corresponding to the input buffer.
 * \param size The length of the input buffer (must be greater or equals to 4).
 * \param h A pointer on a 32-bits fixed-point vector of h_size elements corresponding to the buffer containing the impulse response coefficients.
 * \param h_size The length of the impulse response of the filter (must be greater than 7)
 * \note You need the "Partial Convolution" module
 */
inline static void dsp32_filt_fir(dsp32_t *vect1, dsp32_t *vect2, int size, dsp32_t *h, int h_size) { dsp32_vect_convpart(vect1, vect2, size, h, h_size); };
//! \}

/*! \name Infinite Impulse Response Filter
 * \brief This function computes a real IIR filter using the coefficients of the desire
 * filter onto a fixed-length signal. It returns a signal of a length equals to (size - num_size + 1) elements.
 * \note The data have to be scaled to avoid overflowing values.\n
 * All the vectors have to be 32-bits aligned.
 */
//! \{
/*! \brief 16 bits fixed point version of the IIR .
 * \ingroup group_filtering
 * \param vect1 A pointer on a 16-bits fixed-point vector of (size - num_size + 1) elements corresponding to the output buffer.
 * \param vect2 A pointer on a 16-bits fixed-point vector of size elements corresponding to the input buffer.
 * \param size The length of the input buffer.
 * \param num A pointer on a 16-bits fixed-point vector of num_size elements corresponding to the buffer containing the numerator’s coefficients of the filter.
 * \param num_size The length of the numerator's coefficients of the filter.
 * \param den A pointer on a 16-bits fixed-point vector of den_size elements corresponding to the buffer containing the denominator’s coefficients of the filter.
 * \param den_size The length of the denominator's coefficients of the filter.
 * \param prediv The predivisor used to scale down the denominator’s coefficients of the filter in order to avoid overflow values.
 * So when you use this feature, you have to prescale manually the denominator’s coefficients by 2^prediv else leave this field to 0.
 * \warning Due to its implementation, for the avr32-uc3 optimized version of the IIR,
 * the output buffer (vect1) have to have a length of 6*n elements to avoid overflows.
 */
void dsp16_filt_iir(dsp16_t *vect1, dsp16_t *vect2, int size, dsp16_t *num, int num_size, dsp16_t *den, int den_size, int prediv);
/*! \brief 32 bits fixed point version of the IIR .
 * \ingroup group_filtering
 * \param vect1 A pointer on a 32-bits fixed-point vector of (size - num_size + 1) elements corresponding to the output buffer.
 * \param vect2 A pointer on a 32-bits fixed-point vector of size elements corresponding to the input buffer.
 * \param size The length of the input buffer.
 * \param num A pointer on a 32-bits fixed-point vector of num_size elements corresponding to the buffer containing the numerator’s coefficients of the filter.
 * \param num_size The length of the numerator's coefficients of the filter.
 * \param den A pointer on a 32-bits fixed-point vector of den_size elements corresponding to the buffer containing the denominator’s coefficients of the filter.
 * \param den_size The length of the denominator's coefficients of the filter.
 * \param prediv The predivisor used to scale down the denominator’s coefficients of the filter in order to avoid overflow values.
 * So when you use this feature, you have to prescale manually the denominator’s coefficients by 2^prediv else leave this field to 0.
 */
void dsp32_filt_iir(dsp32_t *vect1, dsp32_t *vect2, int size, dsp32_t *num, int num_size, dsp32_t *den, int den_size, int prediv);
//! \}


/*! \name Least Mean Square Filter
 * \brief This function computes an adaptive LMS filter. It returns a (size)-length signal.
 * \note The data have to be scaled to avoid overflowing values.\n
 * All the vectors have to be 32-bits aligned.
 */
//! \{
/*! \brief 16-bit fixed point version of the LMS filter.
 * \ingroup group_filtering
 * \param x A pointer on a 16-bit fixed-point vector of (size) elements that acts as a circular buffer, filled with the input samples. Its elements
 * have to be initialized to zero and then you just need to reinject this vector each time you call this functions whithout filling any of its values.
 * \param w A pointer on a 16-bit fixed-point vector of size elements corresponding to the coefficients of the filter. Just initialize its elements
 * to zero and after several  iterations, this buffer will be filled with the actual coefficients of the filter.
 * \param size The length of the circular buffer (x) and of the coefficient's buffer (w). It must be a multiple of 4.
 * \param new_x A 16-bit fixed-point value which contains a new input sample signal.
 * \param d A 16-bit fixed-point value which contains the current sample of the reference's signal.
 * \param y A pointer on a 16-bit fixed-point value corresponding to the current sample of the output signal.
 * \param e A pointer on a 16-bit fixed-point value corresponding to the current sample of the output error signal.
 */
void dsp16_filt_lms(dsp16_t *x, dsp16_t *w, int size, dsp16_t new_x, dsp16_t d, dsp16_t *y, dsp16_t *e);
/*! \brief 32-bit fixed point version of the LMS filter.
 * \ingroup group_filtering
 * \param x A pointer on a 32-bit fixed-point vector of (size) elements that acts as a circular buffer, filled with the input samples. Its elements
 * have to be initialized to zero and then you just need to reinject this vector each time you call this functions whithout filling any of its values.
 * \param w A pointer on a 32-bit fixed-point vector of size elements corresponding to the coefficients of the filter. Just initialize its elements
 * to zero and after several  iterations, this buffer will be filled with the actual coefficients of the filter.
 * \param size The length of the circular buffer (x) and of the coefficient's buffer (w).
 * \param new_x A 32-bit fixed-point value which contains a new input sample signal.
 * \param d A 32-bit fixed-point value which contains the current sample of the reference's signal.
 * \param y A pointer on a 32-bit fixed-point value corresponding to the current sample of the output signal.
 * \param e A pointer on a 32-bit fixed-point value corresponding to the current sample of the output error signal.
 */
void dsp32_filt_lms(dsp32_t *x, dsp32_t *w, int size, dsp32_t new_x, dsp32_t d, dsp32_t *y, dsp32_t *e);
//! \}


/*! \name Normalized Least Mean Square Filter
 * \brief This function computes an adaptive NLMS filter. It returns a (size)-length signal.
 * \note The data have to be scaled to avoid overflowing values.\n
 * All the vectors have to be 32-bits aligned.
 */
//! \{
/*! \brief 16-bit fixed point version of the NLMS filter.
 * \ingroup group_filtering
 * \param x A pointer on a 16-bit fixed-point vector of (size) elements that acts as a circular buffer, filled with the input samples. Its elements
 * have to be initialized to zero and then you just need to reinject this vector each time you call this functions whithout filling any of its values.
 * \param w A pointer on a 16-bit fixed-point vector of size elements corresponding to the coefficients of the filter. Just initialize its elements
 * to zero and after several  iterations, this buffer will be filled with the actual coefficients of the filter.
 * \param size The length of the circular buffer (x) and of the coefficient's buffer (w). It must be a multiple of 4.
 * \param new_x A 16-bit fixed-point value which contains a new input sample signal.
 * \param d A 16-bit fixed-point value which contains the current sample of the reference's signal.
 * \param y A pointer on a 16-bit fixed-point value corresponding to the current sample of the output signal.
 * \param e A pointer on a 16-bit fixed-point value corresponding to the current sample of the output error signal.
 */
void dsp16_filt_nlms(dsp16_t *x, dsp16_t *w, int size, dsp16_t new_x, dsp16_t d, dsp16_t *y, dsp16_t *e);
/*! \brief 32-bit fixed point version of the NLMS filter.
 * \ingroup group_filtering
 * \param x A pointer on a 32-bit fixed-point vector of (size) elements that acts as a circular buffer, filled with the input samples. Its elements
 * have to be initialized to zero and then you just need to reinject this vector each time you call this functions whithout filling any of its values.
 * \param w A pointer on a 32-bit fixed-point vector of size elements corresponding to the coefficients of the filter. Just initialize its elements
 * to zero and after several  iterations, this buffer will be filled with the actual coefficients of the filter.
 * \param size The length of the circular buffer (x) and of the coefficient's buffer (w).
 * \param new_x A 32-bit fixed-point value which contains a new input sample signal.
 * \param d A 32-bit fixed-point value which contains the current sample of the reference's signal.
 * \param y A pointer on a 32-bit fixed-point value corresponding to the current sample of the output signal.
 * \param e A pointer on a 32-bit fixed-point value corresponding to the current sample of the output error signal.
 */
void dsp32_filt_nlms(dsp32_t *x, dsp32_t *w, int size, dsp32_t new_x, dsp32_t d, dsp32_t *y, dsp32_t *e);
//! \}

#endif
