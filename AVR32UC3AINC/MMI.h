/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Control Panel MMI module.
 *
 * - Compiler:           GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices.
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

#ifndef __MMI_H__
#define __MMI_H__


/****************************************************************************
on a DIP204 LCD, the display is as follow :
  Line   Space Name          1<----Display---->20
   1    "DateScreen"         MM/DD/YYYY     HH:MM
   2    "ParamScreen"        XXX.XXX.XXX.XXX    Y (when USB connected)
   3    "UserMenuScreen"     < blablabla        > (when other item available)
   4    "UserMessScreen"     blablablablablablabl
****************************************************************************/

/*! types for User Menu screen */
typedef enum {
eUserMenuIdle = 0,
eUserMenuIdleActing,
eUserMenuWaitHost,
eUserMenuUSBHost,
eUserMenuUSBHostActing,
eUserMenuUSBDevice
}eUserMenu;

#define MMI_MAINTAINER_MODE             "Waiting for Host    "

/*! \brief Init MMI, for Man to Macine Interface management.
 *
 */
Bool bMMI_start( void );

/*! \brief MMI function for Man to Macine Interface management.
 *
 */
void vMMI_Manage( void );

/*! \brief display IP in Param space
 *
 *  \param IPAddress   Input. IP to display
 *
 */
void vMMI_DisplayIP(portCHAR * IPAddress);

/*! \brief display IP in Param space
 *
 *  \param IPAddress   Input. IP to display
 *
 */
void vMMI_DisplayUSBState(Bool Connected);


/*! \brief display Date in Date space
 *
 *  \param Date        Input. date to display
 *
 */
void vMMI_DisplayDateAndTime(portCHAR * Date);


/*! \brief display next screen in User Menu space
 *
 *  \param UnderIT   Input. True if calling from IT
 *
 */
void vMMI_UserMenuDisplayNextItem(Bool UnderIT);


/*! \brief display previous screen in User Menu space
 *
 *  \param UnderIT   Input. True if calling from IT
 *
 */
void vMMI_UserMenuDisplayPreviousItem(Bool UnderIT);


/*! \brief set MMI current Menu
 *
 *  \param UserMenu           Input. User Menu to handle
 *  \param ResetCurrentState  Input. pdFALSE if no need to reset MMI current screen number
 *
 */
void vMMI_SetUserMenuMode(eUserMenu UserMenu, Bool ResetCurrentState);

/*! \brief display message in User Menu space
 *
 *  \param Message   Input. Message to display
 *
 */
void vMMI_UserMenuDisplay(portCHAR * Message);

/*! \brief Validate the current item in User Menu
 *
 *  \param UnderIT   Input. True if calling from IT
 *
 */
void vMMI_UserMenuValidateItem(Bool UnderIT);



/*! \brief display message in User Mess space
 *
 *  \param Message   Input. Message to display
 *
 */
void vMMI_UserMessDisplay(portCHAR * Message);

#endif


