/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Fixed Point DSP library
 *
 * This file declares the functions supported by the DSP library
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

#ifndef __DSP_H
#define __DSP_H

/************* Group definition for doxygen documentation ******************/

/*! \defgroup group_functions Basic
 *  This group regroups all the basic functions you can use in your project.
 */
/*! \defgroup group_debug Debug
 *  \ingroup group_functions
 *  All the debugging functions currently supported by the DSP library.
 */
/*! \defgroup group_filtering Filtering
 *  \ingroup group_functions
 *  All the filtering functions currently supported by the DSP library.
 */
/*! \defgroup group_operators Operators
 *  \ingroup group_functions
 *  All the fixed point operators functions currently implemented in the DSP library.
 */
/*! \defgroup group_signal_generation Signal generation
 *  \ingroup group_functions
 *  All the signal generation functions currently supported by the DSP library.
 */
/*! \defgroup group_transforms Transforms
 *  \ingroup group_functions
 *  All the transforms functions currently supported by the DSP library.
 */
/*! \defgroup group_vectors Vectors
 *  \ingroup group_functions
 *  All the vector management functions currently supported by the DSP library.
 */

/*! \defgroup group_config Configuration
 *  This group regroups all the defines you can change to optimize the DSP Library for your needs.
 */
/*! \defgroup group_cstdata Constant tables
 *  \ingroup group_config
 * Those defines permit to configure data tables storage methods
 */
/*! \defgroup group_optimization Optimization options
 *  \ingroup group_config
 * Those parameters are used for optimizing the algorithms according to your needs.\n
 * By default the algorithms are defined to run the fastest as the can.
 */
/*! \defgroup group_qformat Q Format
 *  \ingroup group_config
 *  To configurate the signed fixed point Q format.
 */

/*! \defgroup group_constants Constants
 *  This group regroups all the constants you can use in your project.
 */

/*! \defgroup group_macros Macros
 *  This group regroups all the defined macros you can use in your project.
 */

/*! \defgroup group_types Types
 *  This group regroups all the types you can use in your project.
 */
 
/***********************************************************************/

/*! \name Algorithm's optimization options
 * \warning WARNING! Do not modify those defines. There are here to be used with the \ref DSP_OPTIMIZATION definition.
 */
//! \{
/*! \ingroup group_optimization
 * \brief To accelerate the algorithms
*/
#define DSP_OPTI_SPEED  0
/*! \ingroup group_optimization
 * \brief To be the more accurate as the algorithm can be
 */
#define DSP_OPTI_ACCURACY	 1
/*! \ingroup group_optimization
 * \brief To minimize the size of the data needed to compute the algorithms
 */
#define DSP_OPTI_SIZE  2
/*! \ingroup group_optimization
 * \brief To be the more accurate and smallest as the algorithm can be
 */
#define DSP_OPTI_ACC_AND_SIZE	(DSP_OPTI_ACCURACY | DSP_OPTI_SIZE)
//! \}

/*! \name Algorithm's optimization
 * \ingroup group_optimization
 * \brief This is the define you have to modify in order to optimize the algorithms according to your needs.
 * i.e.: if you want to make the algorithms as accurate as they can be and the smallest possible, use this definition:
 * \code #define DSP_OPTIMIZATION	DSP_OPTI_ACC_AND_SIZE \endcode
 * \note You can directly modify this value by defining on the preprocessor options of you compiler the value of this define.
 */
//! \{
#ifndef DSP_OPTIMIZATION
#  define DSP_OPTIMIZATION	(DSP_OPTI_SPEED)
#endif
//! \}

#ifndef DSP_DEBUG
#  define DSP_DEBUG  0
#endif

#if DSP_DEBUG
#  include "dsp_debug.h"
#endif

#ifndef DSP_USE_GENERICS
#  define DSP_USE_GENERICS  0
#endif

#include "dsp_types.h"

// ******* Function definitions *******
#include "dsp_vectors.h"
#include "dsp_filtering.h"
#include "dsp_operators.h"
#include "dsp_signal_generation.h"
#include "dsp_transforms.h"
// ********************************


//! \brief Set this define to 1 to use functions optimized for the AVR32 UC3
#define DSP_AVR32_UC3 (__GNUC__                     && (__AVR32_UC3A0128__ ||  \
                                                        __AVR32_UC3A0256__ ||  \
                                                        __AVR32_UC3A0512__ ||  \
                                                        __AVR32_UC3A1128__ ||  \
                                                        __AVR32_UC3A1256__ ||  \
                                                        __AVR32_UC3A1512__ ||  \
                                                        __AVR32_UC3B064__  ||  \
                                                        __AVR32_UC3B0128__ ||  \
                                                        __AVR32_UC3B0256__ ||  \
                                                        __AVR32_UC3B164__  ||  \
                                                        __AVR32_UC3B1128__ ||  \
                                                        __AVR32_UC3B1256__) || \
                       (__ICCAVR32__ || __AAVR32__) && (__AT32UC3A0128__   ||  \
                                                        __AT32UC3A0256__   ||  \
                                                        __AT32UC3A0512__   ||  \
                                                        __AT32UC3A1128__   ||  \
                                                        __AT32UC3A1256__   ||  \
                                                        __AT32UC3A1512__   ||  \
                                                        __AT32UC3B064__    ||  \
                                                        __AT32UC3B0128__   ||  \
                                                        __AT32UC3B0256__   ||  \
                                                        __AT32UC3B164__    ||  \
                                                        __AT32UC3B1128__   ||  \
                                                        __AT32UC3B1256__  ))



#endif
