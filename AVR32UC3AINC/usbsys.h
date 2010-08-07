/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief AVR32 UC3 Control Panel USB system interface.
 *
 * This module is in charge of defining the USB objects and starting the USB tasks.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - OS:                 FreeRTOS port on AVR32 UC3.
 * - Supported devices:  AVR32 UC3 products family.
 * - Boards:             EVK1100
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


#ifndef __USBSYS_H__
#define __USBSYS_H__

#include <time.h>

#include "FreeRTOS.h"

#include "supervisor.h"


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________


/*!
 * Start the USB module.
 *
 * \return TRUE upon success, else FALSE.
 */
Bool b_usbsys_start( void );


/*!
 *  \brief In host mode, display basic low-level information about the connected device.
 *         No parameters.
 *         Format: lsusb
 *
 *  \note  This function must be of the type pfShellCmd defined by the shell module.
 *
 *  \param xModId         Input. The module that is calling this function.
 *  \param FsNavId        Ignored.
 *  \param ac             Input. The argument counter. Ignored.
 *  \param av             Input. The argument vector. Ignored
 *  \param ppcStringReply Input/Output. The response string.
 *                        If Input is NULL, no response string will be output.
 *                        Else a malloc for the response string is performed here;
 *                        the caller must free this string.
 *
 *  \return the status of the command execution. If the status is SHELL_EXECSTATUS_KO,
 *  the caller MUST NOT FREE the response string. If the status is SHELL_EXECSTATUS_OK
 *  and the response string is not NULL, the caller MUST free the response string.
 */
eExecStatus e_usbsys_lsusb( eModId xModId, signed short FsNavId,
                       int ac, signed portCHAR *av[],
                       signed portCHAR **ppcStringReply );


/*!
 *  \brief In host mode, copy the /LOG directory to the USB MS key.
 *         No parameters.
 *         Format: cp_logs_to_ukey
 *
 *  \note  This function must be of the type pfShellCmd defined by the shell module.
 *
 *  \param xModId         Input. The module that is calling this function.
 *  \param FsNavId        Input. The file system navigator id to use.
 *  \param ac             Input. The argument counter. Ignored.
 *  \param av             Input. The argument vector. Ignored
 *  \param ppcStringReply Input/Output. The response string.
 *                        If Input is NULL, no response string will be output.
 *                        Else a malloc for the response string may be performed here;
 *                        the caller must thus free this string.
 *
 *  \return the status of the command execution. If the status is SHELL_EXECSTATUS_KO,
 *  the caller MUST NOT FREE the response string. If the status is SHELL_EXECSTATUS_OK
 *  and the response string is not NULL, the caller MUST free the response string.
 */
eExecStatus e_usbsys_cp_logs_to_key( eModId xModId, signed short FsNavId,
                                      int ac, signed portCHAR *av[],
                                      signed portCHAR **ppcStringReply );

/*!
 *  \brief In host mode, move the /LOG directory content to the USB MS key.
 *         No parameters.
 *         Format: mv_logs_to_ukey
 *
 *  \note  This function must be of the type pfShellCmd defined by the shell module.
 *
 *  \param xModId         Input. The module that is calling this function.
 *  \param FsNavId        Input. The file system navigator id to use.
 *  \param ac             Input. The argument counter. Ignored.
 *  \param av             Input. The argument vector. Ignored
 *  \param ppcStringReply Input/Output. The response string.
 *                        If Input is NULL, no response string will be output.
 *                        Else a malloc for the response string may be performed here;
 *                        the caller must thus free this string.
 *
 *  \return the status of the command execution. If the status is SHELL_EXECSTATUS_KO,
 *  the caller MUST NOT FREE the response string. If the status is SHELL_EXECSTATUS_OK
 *  and the response string is not NULL, the caller MUST free the response string.
 */
eExecStatus e_usbsys_mv_logs_to_key( eModId xModId, signed short FsNavId,
                                     int ac, signed portCHAR *av[],
                                     signed portCHAR **ppcStringReply );
                                     
                                     
/*!
 *  \brief In host mode, copy the /CFG directory content of the USB MS key to the
 *  Control Panel /CFG directory.
 *  No parameters.
 *  Format: cp_cfg_to_local
 *
 *  \note  This function must be of the type pfShellCmd defined by the shell module.
 *
 *  \param xModId         Input. The module that is calling this function.
 *  \param FsNavId        Input. The file system navigator id to use.
 *  \param ac             Input. The argument counter. Ignored.
 *  \param av             Input. The argument vector. Ignored
 *  \param ppcStringReply Input/Output. The response string.
 *                        If Input is NULL, no response string will be output.
 *                        Else a malloc for the response string may be performed here;
 *                        the caller must thus free this string.
 *
 *  \return the status of the command execution. If the status is SHELL_EXECSTATUS_KO,
 *  the caller MUST NOT FREE the response string. If the status is SHELL_EXECSTATUS_OK
 *  and the response string is not NULL, the caller MUST free the response string.
 */
eExecStatus e_usbsys_cp_cfg_to_local( eModId xModId, signed short FsNavId,
                                      int ac, signed portCHAR *av[],
                                      signed portCHAR **ppcStringReply );


/*!
 *  \brief In host mode, copy the /WEB directory content of the USB MS key to the
 *  Control Panel /WEB directory.
 *  No parameters.
 *  Format: cp_web_to_local
 *
 *  \note  This function must be of the type pfShellCmd defined by the shell module.
 *
 *  \param xModId         Input. The module that is calling this function.
 *  \param FsNavId        Input. The file system navigator id to use.
 *  \param ac             Input. The argument counter. Ignored.
 *  \param av             Input. The argument vector. Ignored
 *  \param ppcStringReply Input/Output. The response string.
 *                        If Input is NULL, no response string will be output.
 *                        Else a malloc for the response string may be performed here;
 *                        the caller must thus free this string.
 *
 *  \return the status of the command execution. If the status is SHELL_EXECSTATUS_KO,
 *  the caller MUST NOT FREE the response string. If the status is SHELL_EXECSTATUS_OK
 *  and the response string is not NULL, the caller MUST free the response string.
 */
eExecStatus e_usbsys_cp_web_to_local( eModId xModId, signed short FsNavId,
                                      int ac, signed portCHAR *av[],
                                      signed portCHAR **ppcStringReply );

#endif // __USBSYS_H__
