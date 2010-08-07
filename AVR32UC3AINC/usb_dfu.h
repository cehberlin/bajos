/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief Management of the USB device firmware upgrade.
 *
 * This file manages the USB device firmware upgrade.
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


#ifndef _USB_DFU_H_
#define _USB_DFU_H_


//_____ I N C L U D E S ____________________________________________________

#include "conf_usb.h"

#if USB_DEVICE_FEATURE == DISABLED
  #error usb_dfu.h is #included although USB_DEVICE_FEATURE is disabled
#endif


#include "compiler.h"


//_____ M A C R O S ________________________________________________________

// DFU status
#define STATUS_OK                     0x00
#define STATUS_errTARGET              0x01
#define STATUS_errFILE                0x02
#define STATUS_errWRITE               0x03
#define STATUS_errERASE               0x04
#define STATUS_errCHECK_ERASED        0x05
#define STATUS_errPROG                0x06
#define STATUS_errVERIFY              0x07
#define STATUS_errADDRESS             0x08
#define STATUS_errNOTDONE             0x09
#define STATUS_errFIRMWARE            0x0A
#define STATUS_errVENDOR              0x0B
#define STATUS_errUSBR                0x0C
#define STATUS_errPOR                 0x0D
#define STATUS_errUNKNOWN             0x0E
#define STATUS_errSTALLEDPKT          0x0F

// DFU state
#define STATE_appIDLE                 0x00
#define STATE_appDETACH               0x01
#define STATE_dfuIDLE                 0x02
#define STATE_dfuDNLOAD_SYNC          0x03
#define STATE_dfuDNBUSY               0x04
#define STATE_dfuDNLOAD_IDLE          0x05
#define STATE_dfuMANIFEST_SYNC        0x06
#define STATE_dfuMANIFEST             0x07
#define STATE_dfuMANIFEST_WAIT_RESET  0x08
#define STATE_dfuUPLOAD_IDLE          0x09
#define STATE_dfuERROR                0x0A


//_____ D E C L A R A T I O N S ____________________________________________

extern U8 usb_dfu_status;
extern U8 usb_dfu_state;

extern void usb_dfu_init(void);
extern Bool usb_dfu_dnload(void);
extern Bool usb_dfu_upload(void);


#endif  // _USB_DFU_H_
