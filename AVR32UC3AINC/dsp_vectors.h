/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief List of currently implemented vector management functions
 *
 * This file show a list of the vector management functions currently implemented in the DSP library.
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

#ifndef __DSP_VECTORS_H
#define __DSP_VECTORS_H

#include "string.h"

/*! \name Zero padding
 * \brief This function zero pads the tail of the vector.
 */
//! \{
/*! \brief 16 bits fixed point version of the zero padding function.
 * \ingroup group_vectors
 * \param vect1 A pointer on the 16 bits real vector that has to be padded with zeros.
 * \param size The size of this vector.
 * \param num_zero The number of zeros to pad at the end of the vector.
 */
inline static void dsp16_vect_zeropad(dsp16_t *vect1, int size, int num_zero) { memset(vect1 + (size - num_zero)*sizeof(dsp16_t), 0, num_zero*sizeof(dsp16_t)); };
/*! \brief 32 bits fixed point version of the zero padding function.
 * \ingroup group_vectors
 * \param vect1 A pointer on the 32 bits real vector that has to be padded with zeros.
 * \param size The size of this vector.
 * \param num_zero The number of zeros to pad at the end of the vector.
 */
inline static void dsp32_vect_zeropad(dsp32_t *vect1, int size, int num_zero) { memset(vect1 + (size - num_zero)*sizeof(dsp32_t), 0, num_zero*sizeof(dsp32_t)); };
//! \}

/*! \name Copy
 * \brief This function copy a vector into another vector.
 */
//! \{
/*! \brief 16 bits fixed point version of the copy function.
 * \ingroup group_vectors
 * \param vect1 A pointer on the 16 bits real vector that will recieve data.
 * \param vect2 A pointer on the 16 bits real vector to be copied.
 * \param size The size of those vectors.
 */
inline static void dsp16_vect_copy(dsp16_t *vect1, dsp16_t *vect2, int size) { memcpy(vect1, vect2, size*sizeof(dsp16_t)); };
/*! \brief 32 bits fixed point version of the copy function.
 * \ingroup group_vectors
 * \param vect1 A pointer on the 32 bits real vector that will recieve data.
 * \param vect2 A pointer on the 32 bits real vector to be copied.
 * \param size The size of those vectors.
 */
inline static void dsp32_vect_copy(dsp32_t *vect1, dsp32_t *vect2, int size) { memcpy(vect1, vect2, size*sizeof(dsp32_t)); };
//! \}

/*! \name Partial Convolution
 * \brief This function performs a linear partial convolution between two discrete sequences.
 * It returns a signal of a length equals to (vect2_size - vect3_size + 1) elements.
 * \note The two discrete sequences have to be scaled to avoid overflowing values.\n
 * All the vectors have to be 32-bits aligned.
 */
//! \{
/*! \brief 16 bits fixed point version of the Partial Convolution.
 * \ingroup group_vectors
 * \param vect1 A pointer on a 16-bits fixed-point vector corresponding to the output buffer.
 * \param vect2 A pointer on a 16-bits fixed-point vector of vect2_size elements corresponding to the first input buffer.
 * \param vect2_size The length of the first input buffer (must be greater or equals to 4).
 * \param vect3 A pointer on a 16-bits fixed-point vector of vect3_size elements corresponding to the second input buffer.
 * \param vect3_size The length of the second input buffer (must be greater or equals to 8)
 * \warning Due to its implementation, for the avr32-uc3 optimized version of the FIR,
 * the output buffer (vect1) have to have a length of 4*n elements to avoid overflows.
 */
void dsp16_vect_convpart(dsp16_t *vect1, dsp16_t *vect2, int vect2_size, dsp16_t *vect3, int vect3_size);
/*! \brief 32 bits fixed point version of the Partial Convolution.
 * \ingroup group_vectors
 * \param vect1 A pointer on a 32-bits fixed-point vector corresponding to the output buffer.
 * \param vect2 A pointer on a 32-bits fixed-point vector of vect2_size elements corresponding to the first input buffer.
 * \param vect2_size The length of the first input buffer (must be greater or equals to 4).
 * \param vect3 A pointer on a 32-bits fixed-point vector of vect3_size elements corresponding to the second input buffer.
 * \param vect3_size The length of the second input buffer (must be greater or equals to 8)
 */
void dsp32_vect_convpart(dsp32_t *vect1, dsp32_t *vect2, int vect2_size, dsp32_t *vect3, int vect3_size);
//! \}

/*! \name Convolution
 * \brief This function performs a linear convolution between two discrete sequences.
 * It returns a signal of a length equals to (vect2_size + vect3_size - 1) elements.
 * \note The two discrete sequences have to be scaled to avoid overflowing values.\n
 * All the vectors have to be 32-bits aligned.
 * \warning The output buffer of the function has to have at least a length of N + 2*M –
 * 2 elements because of internal computations, where N is the length of the
 * largest input buffer and M, the length of the smallest input buffer.
 */
//! \{
/*! \brief 16 bits fixed point version of the Convolution.
 * \ingroup group_vectors
 * \param vect1 A pointer on a 16-bits fixed-point vector corresponding to the output buffer.
 * \param vect2 A pointer on a 16-bits fixed-point vector of vect2_size elements corresponding to the first input buffer.
 * \param vect2_size The length of the first input buffer (must be greater or equals to 8).
 * \param vect3 A pointer on a 16-bits fixed-point vector of vect3_size elements corresponding to the second input buffer.
 * \param vect3_size The length of the second input buffer (must be greater or equals to 8)
 * \warning Due to its implementation, for the avr32-uc3 optimized version of the FIR,
 * the output buffer (vect1) have to have a length of 4*n elements to avoid overflows.
 * \note You need the "Partial Convolution" module\n
 * the "Copy" module\n
 * the "Zero Padding" module
 */
void dsp16_vect_conv(dsp16_t *vect1, dsp16_t *vect2, int vect2_size, dsp16_t *vect3, int vect3_size);
/*! \brief 32 bits fixed point version of the Convolution.
 * \ingroup group_vectors
 * \param vect1 A pointer on a 32-bits fixed-point vector corresponding to the output buffer.
 * \param vect2 A pointer on a 32-bits fixed-point vector of vect2_size elements corresponding to the first input buffer.
 * \param vect2_size The length of the first input buffer (must be greater or equals to 8).
 * \param vect3 A pointer on a 32-bits fixed-point vector of vect3_size elements corresponding to the second input buffer.
 * \param vect3_size The length of the second input buffer (must be greater or equals to 8)
 * \note You need the "Partial Convolution" module\n
 * the "Copy" module\n
 * the "Zero Padding" module
 */
void dsp32_vect_conv(dsp32_t *vect1, dsp32_t *vect2, int vect2_size, dsp32_t *vect3, int vect3_size);
//! \}

#endif
