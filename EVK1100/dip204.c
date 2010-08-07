/* This source file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief LCD DIP204 driver
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

#include "board.h"
#include "dip204.h"
#include "gpio.h"
#include "compiler.h"
#include "spi.h"
#include "pwm.h"
#include "../platform.h"
/*! Write Command start byte */
#define DIP204_WRITE_COMMAND             0xF8

/*! Write Data start byte */
#define DIP204_WRITE_DATA                0xFA

/*! Read Command start byte, for Busy-flag checking */
#define DIP204_READ_COMMAND              0xFC

/*! Read Data from DDRAM */
#define DIP204_READ_DATA                 0xFE

#define DIP204_PERIOD_MAX          50


static void dip204_write_byte(unsigned short byte);
static void dip204_read_byte(unsigned short * byte);
static void dip204_select(void);
static void dip204_unselect(void);
static void dip204_wait_busy(void);


/*! the duty cycle that will be updated to change backlight power */
unsigned short pwm_duty;

/*! the PWM channel config */
avr32_pwm_channel_t pwm_channel;

/*! The channel number */
int channel_id = -1;

/****************************** global functions *****************************/

void dip204_init(backlight_options option)
{
  pwm_opt_t pwm_opt;  // pwm option config

  if (option == backlight_PWM)
  {
/*    channel_id = DIP204_PWM_CHANNEL;
    gpio_enable_module_pin(AVR32_PWM_PWM_6_PIN, AVR32_PWM_PWM_6_FUNCTION);

    // PWM controller configuration
    pwm_opt.diva=1;
    pwm_opt.divb=0;
    pwm_opt.prea=0;
    pwm_opt.preb=0;

    pwm_init(&pwm_opt);
    pwm_duty = DIP204_PERIOD_MAX - 1;
    pwm_channel.CMR.calg = PWM_MODE_LEFT_ALIGNED;   // channel mode
    pwm_channel.CMR.cpol = PWM_POLARITY_LOW;   // channel polarity
    pwm_channel.CMR.cpd = PWM_UPDATE_PERIOD;   // not used the first time
    pwm_channel.CMR.cpre = 8;   // channel prescaler
    pwm_channel.cdty = pwm_duty;  // channel duty cycle, should be < CPRD
    pwm_channel.cprd = DIP204_PERIOD_MAX;  // channel period
    pwm_channel.cupd = 0;  // channel update is not used here.

    pwm_channel_init(channel_id, &pwm_channel);
    // start PWM
    pwm_start_channels(1 << channel_id);
*/
  }
  else
  {
    gpio_clr_gpio_pin(DIP204_BACKLIGHT_PIN);
  }
  // delay for power on
  delay_ms(20);
  // select the LCD chip
  dip204_select();
  // Send Command Start Byte
  dip204_write_byte(DIP204_WRITE_COMMAND);
  // Send "extended Function Set" Command  (RE=1)
  dip204_write_byte(0x34);
  // Wait for command execution
  delay_ms(1);
  // Send "Enter 4-Line Mode" Command
  dip204_write_byte(0x09);
  // Wait for command execution
  delay_ms(1);
  // Send "Function Set" Command (RE=0)
  dip204_write_byte(0x30);
  // Wait for command execution
  delay_ms(1);
  // Send "Display On Command: Display On, Cursor On, Blink On"
  dip204_write_byte(0x0F);
  // Wait for command execution
  delay_ms(1);
  // Send "Display Clear" Command
  dip204_write_byte(0x01);
  // Wait for command execution
  delay_ms(5);
  // Send "Entry Mode Set Command: Increment Mode, Entire Shift off"
  dip204_write_byte(0x06);
  // Wait for command execution
  delay_ms(1);
  dip204_wait_busy();
  // unselect chip
  dip204_unselect();
}

void dip204_set_backlight(backlight_power power)
{
  if ((power == backlight_power_decrease) && (channel_id != -1))
  {
    // update channel duty cycle using double buffering to prevent unexpected waveform.
    pwm_duty = Max((pwm_duty - (DIP204_PERIOD_MAX / 10)) , 1);
    pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;
    // new duty cycle
    pwm_channel.cupd = pwm_duty;
    // set channel configuration.
    pwm_update_channel (channel_id, &pwm_channel);
  }
  else if ((power == backlight_power_increase) && (channel_id != -1))
  {
    // update channel duty cycle using double buffering to prevent unexpected waveform.
    pwm_duty = Min((pwm_duty + (DIP204_PERIOD_MAX / 10)) , (DIP204_PERIOD_MAX-1));
    pwm_channel.CMR.cpd = PWM_UPDATE_DUTY;
    // new duty cycle
    pwm_channel.cupd = pwm_duty;
    // set channel configuration.
    pwm_update_channel (channel_id, &pwm_channel);
  }
}


void dip204_show_cursor(void)
{
  /* select the LCD chip */
  dip204_select();
  /* Send Command Start Byte */
  dip204_write_byte(DIP204_WRITE_COMMAND);
  /* Send "Display On Command: Display On, Cursor On, Blink On" */
  dip204_write_byte(0x0F);
  dip204_wait_busy();
  /* unselect chip */
  dip204_unselect();
}


void dip204_hide_cursor(void)
{
  /* select the LCD chip */
  dip204_select();
  /* Send Command Start Byte */
  dip204_write_byte(DIP204_WRITE_COMMAND);
  /* Send "Display On Command: Display On, Cursor On, Blink On" */
  dip204_write_byte(0x0C);
  dip204_wait_busy();
  /* unselect chip */
  dip204_unselect();
}


void dip204_write_data(unsigned char data)
{
  dip204_select();
  /* Send Write Data Start-Byte */
  dip204_write_byte(DIP204_WRITE_DATA);
  /* send data */
  dip204_write_byte(data);
  dip204_wait_busy();
  dip204_unselect();
}


void dip204_read_data(unsigned short *data)
{
  dip204_select();
  /* Send Read Data Start-Byte */
  dip204_write_byte(DIP204_READ_DATA);
  /* read SPI data */
  *data = 0;
  dip204_read_byte(data);
  /* wait for LCD */
  dip204_wait_busy();
  dip204_unselect();
}


void dip204_set_cursor_position(unsigned char row, unsigned char line)
{
unsigned char address = 0;

  dip204_select();
  if ((row <= 20) && (line <= 4))
  {
    /* Calculate DDRAM address from line and row values */
    address = ( (line-1) * 32 ) + ( row-1 ) + 128;
  }
  /* Send Command Start Byte */
  dip204_write_byte(DIP204_WRITE_COMMAND);
  /* Send Adress lower Nibble */
  dip204_write_byte(address);
  dip204_wait_busy();
  dip204_unselect();
}


void dip204_clear_display(void)
{
  dip204_select();
  /* Send Command Start Byte */
  dip204_write_byte(DIP204_WRITE_COMMAND);
  /* Send Display Clear Command */
  dip204_write_byte(0x01);
  /* Wait for command execution */
  delay_ms(4);
  dip204_wait_busy();
  dip204_unselect();
}

void dip204_write_string(const char *string)
{
unsigned char i=0;

  dip204_select();
  /* for all chars in string */
  while(string[i]!=0)
  {
    /* Send Write Data Start Byte */
    dip204_write_byte(DIP204_WRITE_DATA);
    /* Send byte */
    dip204_write_byte(string[i]);
    /* go to next char */
    i++;
    dip204_wait_busy();
  }
  dip204_unselect();
}

/****************************** local functions ******************************/

/*! \brief function to select the LCD
 *
 */
static void dip204_select(void)
{
  spi_selectChip(DIP204_SPI, DIP204_SPI_CS);
}

/*! \brief function to unselect the LCD
 *
 */
static void dip204_unselect(void)
{
  spi_unselectChip(DIP204_SPI, DIP204_SPI_CS);
}

/*! \brief hardware abstraction layer to send a byte to LCD
 *         depends if LCD is plugged on SPI or on EBI
 *
 *  \param  byte  Input. byte to write to the LCD (D7 .. D0)
 *
 */
static void dip204_write_byte(unsigned short byte)
{
unsigned short i;
unsigned short reverse = 0x00;
  switch (byte)
  {
    /* MSB first for command */
    case DIP204_READ_COMMAND:
    case DIP204_WRITE_COMMAND:
    case DIP204_READ_DATA:
    case DIP204_WRITE_DATA:
    {
      /* send D7 to D0 */
      spi_write(DIP204_SPI, byte);
      break;
    }
    /* LSB first for all other data */
    default :
    {
      /* reverse byte */
      for (i = 0 ; i < 8 ; i++)
      {
        if (Tst_bits(byte, (1 << i)))
        {
          Set_bits(reverse, (1 << (7-i)));
        }
      }
      /* send D0 to D3 */
      spi_write(DIP204_SPI, (reverse & 0xF0));
      /* send D4 to D7 */
      spi_write(DIP204_SPI, ((reverse << 4) & 0xF0));
      break;
    }
  }
}

/*! \brief hardware abstraction layer to read a byte from LCD
 *         depends if LCD is plugged on SPI or on EBI
 *
 *  \param  byte  Input. byte read from the LCD (D7 .. D0)
 *
 */
static void dip204_read_byte(unsigned short * byte)
{
unsigned short reverse = 0x00;
unsigned short i;
  /* dummy write */
  spi_write(DIP204_SPI, 0x00);
  /* read RSR register */
  spi_read(DIP204_SPI, &reverse);
  *byte = 0;
  /* Revert received byte (issued LSB first by the LCD) */
  for (i = 0 ; i < 8 ; i++)
  {
    if (Tst_bits(reverse, (1 << i)))
    {
      Set_bits(*byte, (1 << (7-i)));
    }
  }
}

/*! \brief function to wait for LCD becomes not busy
 *
 */
static void dip204_wait_busy(void)
{
unsigned short status = 0x00;
  /* send read commd to LCD */
  dip204_write_byte(DIP204_READ_COMMAND);
  /* read next byte */
  do {
  dip204_read_byte(&status);
  /* keep D7 to know status */
  }while (status & 0x80);
}
