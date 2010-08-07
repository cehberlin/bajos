/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief DSP library debbuging functions
 *
 * This file defines a useful set of debugging functions for the DSP library
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

#ifndef __DSP_DEBUG_PRINT_H
#define __DSP_DEBUG_PRINT_H

#include "dsp_types.h"

/*! \brief This type definition is a pointer on a function that permits to print a string of a device.\n
 * It is used to separate the debug module to a printer device module.
 */
typedef void (*dsp_debug_print_str_t)(char *);

/*! \ingroup group_debug
 * This define permits to print the date and the time of the last compilation of the \ref dsp_debug_initialization__ function
 */
#define dsp_debug_initialization(x) dsp_debug_initialization__(__DATE__, __TIME__, x)

/*! \name Initialization function
 * \ingroup group_debug
 * \brief This function permits to initialize the debugging module.
 * \param date A string containing today's date.
 * \param time A string containing the current time.
 * \param fct A pointer on a function that permits to print a string on a device.
 * \note This function has to be called before using all the other functions.\n
 * It is highly recommended to use the define \ref dsp_debug_initialization instead of using directly this function.
 */
void dsp_debug_initialization__(char *date, char *time, dsp_debug_print_str_t fct);


/*! \name Q formatted number print functions 
 * \brief These function permits to print a signed fixed point Q formatted number.
 */
//! \{
/*! \ingroup group_debug
 * \brief Generic function to print all kind of Q formatted numbers.
 * \param nb_bits The number of bits of the number to print.
 * \param q The a parameter of a Qa.b formatted number.
 * \param n The signed fixed point Q formatted number to print.
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
void dsp_debug_print(int nb_bits, int q, int n);
/*! \ingroup group_debug
 * \brief Generic function to print all kind of Q formatted numbers.
 * \param out The buffer that recieved the printed number.
 * \param nb_bits The number of bits of the number to print.
 * \param q The a parameter of a Qa.b formatted number.
 * \param i The signed fixed point Q formatted number to print.
 */
int dsp_debug_sprint(char **out, int nb_bits, int q, int i);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp16_t typed number.
 * \param n The 16 bits signed fixed point Q formatted number to print.
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
#if __GNUC__
inline
#elif __ICCAVR32__
#    pragma inline
#endif
void dsp16_debug_print(dsp16_t n);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp32_t typed number.
 * \param n The 32 bits signed fixed point Q formatted number to print.
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
#if __GNUC__
inline
#elif __ICCAVR32__
#    pragma inline
#endif
void dsp32_debug_print(dsp32_t n);
//! \}

/*! \name Complex number print functions 
 * \brief These function permits to print a complex signed fixed point Q formatted number.
 */
//! \{
/*! \ingroup group_debug
 * \brief Generic function to print all kind of complex Q formatted numbers.
 * \param nb_bits The number of bits of the complex number to print.
 * \param q The a parameter of a Qa.b formatted number.
 * \param real The real part of the signed fixed point Q formatted complex number to print.
 * \param imag The imaginary part of the signed fixed point Q formatted complex number to print.
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
void dsp_debug_print_complex(int nb_bits, int q, int real, int imag);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp16_complex_t typed number.
 * \param n The 16 bits signed fixed point Q formatted complex number to print.
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
#if __GNUC__
inline
#elif __ICCAVR32__
#    pragma inline
#endif
void dsp16_debug_print_complex(dsp16_complex_t *n);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp32_complex_t typed number.
 * \param n The 32 bits signed fixed point Q formatted complex number to print.
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
#if __GNUC__
inline
#elif __ICCAVR32__
#    pragma inline
#endif
void dsp32_debug_print_complex(dsp32_complex_t *n);
//! \}

/*! \name Vector print functions
 * \brief These function permits to print vectors.\n
 * All these functions print a vector the same way.
 * \code
 * 1	0.02556
 * 2	-0.60125
 * ...
 * \endcode
 * For a real type vector.
 * \code
 * 1	0.02556 + -0.08965i
 * 2	-0.65064 + 0.i
 * ...
 * \endcode
 * For a complex type vector.
 */
//! \{
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp16_t typed vector.
 * \param vect the vector to print.
 * \param size The size of the vector (vect).
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
void dsp16_debug_print_vect(dsp16_t *vect, int size);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp32_t typed vector.
 * \param vect the vector to print.
 * \param size The size of the vector (vect).
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
void dsp32_debug_print_vect(dsp32_t *vect, int size);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp16_complex_t typed vector.
 * \param vect the vector to print.
 * \param size The size of the vector (vect).
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
void dsp16_debug_print_complex_vect(dsp16_complex_t *vect, int size);
/*! \ingroup group_debug
 * \brief This functions permits to print a \ref dsp32_complex_t typed vector.
 * \param vect the vector to print.
 * \param size The size of the vector (vect).
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
void dsp32_debug_print_complex_vect(dsp32_complex_t *vect, int size);
//! \}

/*! \ingroup group_debug
 * \brief This functions permits to copy the digits after the radix of a division's result in the specified buffer.
 * \param out The buffer that recieved the digits after the radix.
 * \param num The numerator of the division.
 * \param den The denominator of the division.
 * \param nb_digits The number of digits to print.
 * \return the number of characters written.
 */
int dsp_debug_sprint_after_radix(char **out, unsigned int num, unsigned int den, int nb_digits);

/*! \name Printf functions 
 * \brief These function permits to print any data on a printing device.\n
 * These functions work as the printf function, except that they include the fixed point Q formatted format notion.
 * The following attributes are currently implemented:\n
 * \li \b s : The const char * argument is expected to be a pointer to an array of character type (pointer to a string).
 * Characters from the array are written up to (but not including) a terminating null byte ('\\0'); if a precision is specified,
 * no more than the number specified are written. If a precision is given, no null byte need be present; if the precision is not
 * specified, or is greater than the size of the array, the array must contain a terminating null byte.\n
 * \li \b d ,\b i : The int argument is converted to signed decimal notation. The precision, if any, gives the minimum number of digits that must appear;
 * if the converted value requires fewer digits, it is padded on the left with zeros. The default precision is 1.
 * When 0 is printed with an explicit precision 0, the output is empty.\n
 * \li \b o ,\b u ,\b x ,\b X : The unsigned int argument is converted to unsigned octal (\b o ), unsigned decimal (\b u ), or unsigned hexadecimal (\b x and \b X ) notation.
 * The letters abcdef are used for x conversions; the letters ABCDEF are used for X conversions. The precision, if any, 
 * gives the minimum number of digits that must appear; if the converted value requires fewer digits, it is padded on the left with zeros.
 * The default precision is 1. When 0 is printed with an explicit precision 0, the output is empty.\n
 * \li \b c : The int argument is converted to an unsigned char, and the resulting character is written.\n
 * \li \b f : The fixed point argument is converted to decimal notation in the style [-]ddd.ddd, [THE FOLLOWING IS NOT IMPLEMENTED YET!] where the number of digits after the decimal-point character
 * is equal to the precision specification. If the precision is missing, it is taken as 6; if the precision is explicitly zero, no decimal-point character appears.
 * If a decimal point appears, at least one digit appears before it. 
 */
//! \{
/*! \ingroup group_debug
 * \brief This function is the printf version for 16-bit Q formatted signed numbers
 * \param format The number of bits of the complex number to print.
 * \return The number of characters written
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
int dsp16_debug_printf(const char *format, ...);
/*! \ingroup group_debug
 * \brief This function is the printf version for 32-bit Q formatted signed numbers
 * \param format The number of bits of the complex number to print.
 * \return The number of characters written
 * \note This function print the data on the printing device specified in the initialization function.
 * \pre The \ref dsp_debug_initialization__ function has to be called before using this function.
 */
int dsp32_debug_printf(const char *format, ...);
//! \}

#endif
