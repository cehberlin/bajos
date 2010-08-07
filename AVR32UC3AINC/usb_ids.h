/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Contains some IDs (VID, PID, Class numbers) used in the various
 *        demos.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a USB module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ***************************************************************************/

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


#ifndef _USB_IDS_H_
#define _USB_IDS_H_


//_____ I N C L U D E S ____________________________________________________

//_____ M A C R O S ________________________________________________________

//_____ D E C L A R A T I O N S ____________________________________________

#define ATMEL_VID           0x03EB

#define ENUM_EXAMPLE_PID                      0x2300
#define MS_EXAMPLE_PID                        0x2301
#define MS_SDRAM_LOADER_PID                   0x2302
#define EVK1100_CTRL_PANEL_DEMO_PID           0x2303
#define HID_EXAMPLE_PID                       0x2304
#define EVK1101_CTRL_PANEL_DEMO_HID_PID       0x2305
#define EVK1101_CTRL_PANEL_DEMO_HID_MS_PID    0x2306
#define CDC_EXAMPLE_PID                       0x2307
#define ISP_UC3A_PID                          0x2FF8
#define ISP_UC3B_PID                          0x2FF6

#define NO_CLASS            0x00
#define CDC_ACM_COM_CLASS   0x02
#define CDC_ACM_DATA_CLASS  0x0A
#define HID_CLASS           0x03
#define MS_CLASS            0x08
#define APPLICATION_CLASS   0xFE
#define VENDOR_CLASS        0xFF

#define NO_SUBCLASS         0x00
#define DFU_SUBCLASS        0x01
#define BOOT_SUBCLASS       0x01
#define SCSI_SUBCLASS       0x06

#define NO_PROTOCOL         0x00
#define RUNTIME_PROTOCOL    0x01
#define KEYBOARD_PROTOCOL   0x01
#define DFU_MODE_PROTOCOL   0x02
#define MOUSE_PROTOCOL      0x02
#define BULK_PROTOCOL       0x50


#endif  // _USB_IDS_H_
