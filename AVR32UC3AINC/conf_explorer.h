/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file ******************************************************************
 *
 * \brief FAT configuration file.
 *
 * This file contains the possible external configuration of the FAT.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
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


#ifndef _CONF_EXPLORER_H_
#define _CONF_EXPLORER_H_

#include "compiler.h"


//! Include files and compile options (don't touch).
#define LIB_MEM               <string.h>
#define LIB_CTRLACCESS        "ctrl_access.h"

//! Supported FATs (\c ENABLED or \c DISABLED).
#define FS_FAT_12             ENABLED
#define FS_FAT_16             ENABLED
#define FS_FAT_32             ENABLED

//! The explorer may support either the ASCII or the UNICODE string format, or both.
#define FS_ASCII              ENABLED
#define FS_UNICODE            DISABLED

//! The navigator may support only the first partition (\c DISABLED), or multiple partitions (\c ENABLED).
#define FS_MULTI_PARTITION    DISABLED

//! Maximal number of characters in file path.
#define MAX_FILE_PATH_LENGTH  30

//! Maximal size of configuration file.
#define MAX_CONFIG_FILE_SIZE  136  // smtp.txt : port(12) + mailto(49) + mailfrom(51) + server(24)

//! Level of features.
//! Select among:
//!  - \c FSFEATURE_READ:           All read functions.
//!  - \c FSFEATURE_WRITE:          nav_file_copy(), nav_file_paste(), nav_file_del(), file_create(), file_open(MODE_WRITE), file_write(), file_putc().
//!  - \c FSFEATURE_WRITE_COMPLET:  FSFEATURE_WRITE functions and nav_drive_format(), nav_dir_make(), nav_file_rename(), nav_file_dateset(), nav_file_attributset().
//!  - \c FSFEATURE_ALL:            All functions.
#define FS_LEVEL_FEATURES     (FSFEATURE_READ | FSFEATURE_WRITE_COMPLET)

//! Number of caches used to store a cluster list of files (interesting in case of many `open file').
//! In player mode, 1 is OK (shall be > 0).
#define FS_NB_CACHE_CLUSLIST  1

//! Maximal number of simultaneous navigators.
#define FS_NB_NAVIGATOR       10

//! Number of reserved navigators (ids from \c 0 to <tt>(FS_NB_RESERVED_NAVIGATOR - 1)</tt>).
#define FS_NB_RESERVED_NAV    3

/*! \name Navigator Affiliations
 *
 * Define the affiliations of the navigators.
 *
 * \note The explorer always has the navigator ID 0.
 *
 * \note The update and status files use a different navigator (2) because they are enabled at the same time.
 */
//! @{

//! The play list uses the navigator ID 1.
#define FS_NAV_ID_PLAYLIST    1

//! The explorer uses the navigator ID 2 to open the `copy file' and the ID 0 to open the `paste file'.
#define FS_NAV_ID_COPYFILE    2

//! Define the default navigator used to open the default update file.
#define FS_NAV_ID_UPDATEFILE  1

//! Define the default navigator used to open the default status file.
#define FS_NAV_ID_STATUSFILE  2

//! @}


//***** INOFRMATION ABOUT FILE-SYSTEM LIMITATIONS *****
// Only 2-FAT FSs supported by this navigator.
// Only 512-byte sectors supported by this navigator.
// Up to 65534 files supported per directory.
//*****************************************************


#endif  // _CONF_EXPLORER_H_
