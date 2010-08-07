/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief LCD DIP204 driver for EVK1100 board.
 *
 * This file defines a useful set of functions for the DIP204 interface on
 * AVR32 devices.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
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


#ifndef _AVR32_DIP204_H_
#define _AVR32_DIP204_H_


#  include <avr32/io.h>

/*! \name SPI Connections of the DIP204 LCD
 */
//! @{
#define DIP204_SPI                  (&AVR32_SPI1)
#define DIP204_SPI_CS               2
#define DIP204_SPI_SCK_PIN          AVR32_SPI1_SCK_0_PIN
#define DIP204_SPI_SCK_FUNCTION     AVR32_SPI1_SCK_0_FUNCTION
#define DIP204_SPI_MISO_PIN         AVR32_SPI1_MISO_0_PIN
#define DIP204_SPI_MISO_FUNCTION    AVR32_SPI1_MISO_0_FUNCTION
#define DIP204_SPI_MOSI_PIN         AVR32_SPI1_MOSI_0_PIN
#define DIP204_SPI_MOSI_FUNCTION    AVR32_SPI1_MOSI_0_FUNCTION
#define DIP204_SPI_NPCS_PIN         AVR32_SPI1_NPCS_2_PIN
#define DIP204_SPI_NPCS_FUNCTION    AVR32_SPI1_NPCS_2_FUNCTION
//! @}

/*! type for Backlight options : use PWM or IO to drive the backlight
 *
 */
typedef enum {
  backlight_IO = 0,
  backlight_PWM
}backlight_options;

/*! type for Backlight power : increase or decrease the backlight
 *
 */
typedef enum {
  backlight_power_increase = 0,
  backlight_power_decrease
}backlight_power;


/*! Initialize the LCD
 *         (need void delay_ms(unsigned short time_ms) function to perform active wait)
 *
 * \param option backlight_IO if no PWM needed, backlight_PWM if PWM needed...
 */
void dip204_init(backlight_options option);

/*! Change the backlight power
 *
 * \param power increase or decrease the backlight ...
 */
void dip204_set_backlight(backlight_power power);


/*! Show blinking cursor
 *
 */
void dip204_show_cursor(void);


/*! Hide cursor
 *
 */
void dip204_hide_cursor(void);


/*! Write a byte at current position
 *
 * \param  data   Input. data to display
 *
 */
void dip204_write_data(unsigned char data);


/*! Read data at current position
 *
 * \param  data   Output. data read at current position
 *
 */
void dip204_read_data(unsigned short *data);


/*! Set cursor to given position
 *
 * \param  row   Input. row where to set cursor (from 1 to 20)
 * \param  line  Input. line where to set cursor (from 1 to 4)
 *
 */
void dip204_set_cursor_position(unsigned char row, unsigned char line);


/*! Clear the LCD screen
 *         (need void delay_ms(unsigned short time_ms) function to perform active wait)
 *
 */
void dip204_clear_display(void);


/*! Write a string
 *
 * \param  string   Input. null terminated string to display
 *
 */
void dip204_write_string(char *string);

#endif // _AVR32_DIP204_H_
