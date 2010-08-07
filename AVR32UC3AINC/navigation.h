/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FAT 12/16/32 Services.
 *
 * This file defines a useful set of functions for file navigation.
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

#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include "fs_com.h"
#include "fat.h"

//! \name nav_filelist_set() options parameter
//! @{
#define  FS_FIND_NEXT      TRUE     //!< move in list to next file
#define  FS_FIND_PREV      FALSE    //!< move in list to previous file
//! @}


//************************ String format select ************************

//! This fonction select the UNICODE mode for all routines with FS_STRING parameter 
//!
//! @verbatim
//! If you have enable the FS_ASCII AND FS_UNICODE define
//! then FS_STRING parameter can be a ASCII or UNICODE table.
//! Use this routines to select the type UNICODE of string table.
//! @endverbatim
//!
void  nav_string_unicode( void );

//! This fonction select the ASCII mode for all routines with FS_STRING parameter 
//!
//! @verbatim
//! If you have enable the FS_ASCII AND FS_UNICODE define
//! then FS_STRING parameter can be a ASCII or UNICODE table.
//! Use this routines to select the type ASCII for string table.
//! @endverbatim
//!
void  nav_string_ascii( void );


//************** Initialise or Stop navigation module ****************** 

//! This fonction reset ALL navigations to init file sytem module
//!
//! @verbatim
//! Call this one at the startup of program
//! or after a other program have change the disk (write disk access)
//! @endverbatim
//!
void  nav_reset( void );

//! This fonction select the current navigation to use
//!
//! @param     u8_idnav    Id navigator to select (0 to FS_NB_NAVIGATOR-1)
//!
//! @return    FALSE if ID navigator don't exist
//! @return    TRUE otherwise
//!
Bool  nav_select( U8 u8_idnav );

//! This fonction return the current navigation used
//!
//! @return    u8_idnav    Id navigator selected
//!
U8    nav_get( void );

//! This fonction copy all current navigator informations to a other navigator
//!
//! @param     u8_idnav    Id navigator where the main navigator will be to copy
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! Use this routine to open and select quickly the same file of a other navigator
//! @endverbatim
//!
Bool  nav_copy_file_current( U8 u8_idnav );


//********************* Drive navigation fonctions *********************

//! This fonction return the number of device possible
//!
//! @return    number of device, 0 = NO DEVICE AVIABLE
//!
//! @verbatim
//! This value may be dynamic because it depends of memory drivers (e.g. in case of USB HOST)
//! @endverbatim
//!
U8    nav_drive_nb( void );

//! This fonction selects the drive in current navigator but don't mount the disk
//!
//! @param     u8_number    device number (0 to nav_drive_nb()-1 )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_drive_set( U8 u8_number );

//! This fonction return the drive number selected in current navigator
//!
//! @return    0 to nav_drive_nb()-1
//! @return    0xFF in case of no drive selected
//!
U8    nav_drive_get( void );

//! This fonction return the drive letter selected in current navigator
//!
//! @return    'A','B',...
//! @return    'X', in case of no drive selected
//!
U8    nav_drive_getname( void );

//! This fonction format the current drive (disk)
//!
//! @param     u8_fat_type    Select the type of format <br>
//!            FS_FORMAT_DEFAULT, The file system module choose the better FAT format for the drive space <br>
//!            FS_FORMAT_FAT, The FAT12 or FAT16 is used to format the drive, if possible (disk space <2GB) <br>
//!            FS_FORMAT_FAT32, The FAT32 is used to format the drive, if possible (disk space >32MB) <br>
//!            FS_FORMAT_NOMBR_FLAG, if you don't want a MRB in disk then add this flag (e.g. FAT format on a CD support)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine can't format a multi-partiton, if the disk containt as a multi-partition
//! then the multi-partition is erase and replace by a single partition on ALL disk space.
//! @endverbatim
//!
Bool  nav_drive_format( U8 u8_fat_type );


//******************* Partition navigation fonctions ******************* 

//! This fonction return the number of partition present on current drive selected
//!
//! @return    u8_number   number of partition
//!
U8    nav_partition_nb( void );

//! This fonction select a partition available on current drive
//!
//! @param     u8_number   partition number (0 to 3)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_partition_set( U8 partition_number );

//! This fonction mount the current partition selected 
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! If the option FS_MULTI_PARTITION is disable
//! then the mount routine select the first partition supported by file system. <br>
//! After mount, the file list corresponding at the files and directorys present in ROOT directory 
//! @endverbatim
//!
Bool  nav_partition_mount( void );

//! This fonction give the partition type
//!
//! @return partition type : FS_TYPE_FAT_12, FS_TYPE_FAT_16, FS_TYPE_FAT_32
//! @return FS_TYPE_FAT_UNM, in case error or unknow format
//!
U8    nav_partition_type( void );

//! This fonction read or write the serial number of current partition selected
//!
//! @param     b_action    to select the action <br>
//!                        FS_SN_READ to read serial number <br>
//!                        FS_SN_WRITE to write serial number <br>
//! @param     a_u8_sn     pointer on a array of 4 bytes <br>
//!                        if FS_SN_READ, then use to store serial number <br>
//!                        if FS_SN_WRITE, then use to pass the new serial number <br>
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_partition_serialnumber( Bool b_action , U8 _MEM_TYPE_SLOW_ *a_u8_sn );

//! This fonction read or write the label of current partition selected
//!
//! @param     b_action    to select the action <br>
//!                        FS_LABEL_READ to read label <br>
//!                        FS_LABEL_WRITE to write label <br>
//! @param     sz_label    pointer on a string ASCII of 11 characters + NULL terminator (=12 bytes) <br>
//!                        if FS_LABEL_READ, then use to store label <br>
//!                        if FS_LABEL_WRITE, then use to pass the new label <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_partition_label( Bool b_action , FS_STRING sz_label );

//! This fonction return the total space of the current partition
//!
//! @return    number of sector
//! @return    0, in case of error
//!
//! @verbatim
//! You shall mount the partition before call this routine
//! @endverbatim
//!
U32   nav_partition_space( void );

//! This fonction return the free space of the current partition
//!
//! @return    number of free sector
//! @return    0 in case of error or full partition
//!
//! @verbatim
//! You shall mount the partition before call this routine
//! @endverbatim
//!
U32   nav_partition_freespace( void );

//! This fonction return the space free in percent of the current partition
//!
//! @return    percent of free space (0% to 100%)
//! @return    0% in case of error or full partition
//!
//! @verbatim
//! To speed up the compute, the resultat have a error delta of 1%
//! @endverbatim
//!
U8    nav_partition_freespace_percent( void );


//****************** File list navigation fonctions ******************** 

//! This fonction reset the pointer of selection, so "no file selected" in file list
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_reset( void );

//! This fonction check if a file is selected
//!
//! @return    TRUE  if a file is selected
//! @return    FALSE if no file is selected
//!
Bool  nav_filelist_validpos( void );

//! This fonction check if no file is open
//!
//! @return    TRUE  if no file is open
//! @return    FALSE if a file is open
//!
Bool  nav_filelist_fileisnotopen( void );

//! This fonction move the pointer of selection in file list
//!
//! @param     u16_nb      numbers file to jump before stopping action <br>
//!                        0, stop at the first file found <br>
//!                        1, stop at the second file found <br>
//!
//! @param     b_direction search direction <br>
//!                        FS_FIND_NEXT, move to next file or directory
//!                        FS_FIND_PREV, move to previous file or directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! Note: if no file is selected then nav_filelist_set( 0 , FS_NEXT ) go to first file of the File list.
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_set( U16 u16_nb , Bool b_direction );

//! This fonction return the position of selected file in file list
//!
//! @return    position of selected file in current directory, 0 is the first position
//! @return    FS_NO_SEL, in case of no file selected
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
U16   nav_filelist_get( void );

//! This fonction go to a position in file list
//!
//! @param     u16_new_pos    new position to select in file list, 0 is the first position
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_goto( U16 u16_newpos );

//! This fonction search a file name in file list 
//!
//! @param     sz_name        name to search (UNICODE or ASCII) <br>
//!                           It must be terminate by NULL or '*' value
//! @param     b_match_case   FALSE to ignore the case
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This fonction start a search at the current position the file name in file list 
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_findname( const FS_STRING sz_name , Bool b_match_case );

//! This fonction check the end of file list
//!
//! @return    FALSE, NO end of file list
//! @return    TRUE, in case of end of list or error
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_eol( void );

//! This fonction check the beginning of file list
//!
//! @return    FALSE, NO begining of file list
//! @return    TRUE, in case of the file selected is the first file, or in case of error
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_bol( void );

//! This fonction check the presence of files or directorys in file list
//!
//! @param     b_type   FS_DIR  to check the directory presence <br>
//!                     FS_FILE to check the file presence <br>
//!
//! @return    TRUE, in case of a file or a directory exist
//! @return    FALSE, in case of no file or no directory exist, or error
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_exist( Bool b_type );

//! This fonction compute the number of files or directorys in file list
//!
//! @param     b_type   FS_DIR  to compute the directory presence <br>
//!                     FS_FILE to compute the file presence <br>
//!
//! @return    number of file or directory present in file list
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
U16   nav_filelist_nb( Bool b_type );

//! This fonction go to at the first file or directory in file list
//!
//! @param     b_type   FS_DIR  to go at the first directory <br>
//!                     FS_FILE to go at the first file <br>
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_first( Bool b_type );

//! This fonction go to at the last file or directory in file list
//!
//! @param     b_type   FS_DIR  to go at the last directory <br>
//!                     FS_FILE to go at the last file <br>
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_last( Bool b_type );


//************************ Index fonctions *****************************

//! This fonction return a small pointer on the current file system position (disk, partition, dir, file/dir selected )
//!
//! @return    It is a small index structure with the informations on file system position (disk, partition, dir, file/dir selected )
//!
//! @verbatim
//! This routine is interresting to save a file pointer in small variable.
//! This pointer permit to reinit a navigator quickly with nav_gotoindex() routine.
//! @endverbatim
//!
Fs_index nav_getindex( void );

//! This fonction reinit a navigator via a file pointer
//!
//! @param     index    It is a structure with the informations on file system position (disk, partition, dir, file/dir selected )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine permit to reinit a navigator quickly via file pointer (disk, partition, dir, file/dir selected )
//! To get a file pointer use the routine nav_getindex()
//! @endverbatim
//!
Bool  nav_gotoindex( const Fs_index _MEM_TYPE_SLOW_ *index );


//************************ Directory fonctions *************************

//! This fonction init the file list on the root directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_root( void );

//! This fonction enter in the current directory selected in file list
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After this routine the file list change and containt the files of new current directory,
//! also no file is selected
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_cd( void );

//! This fonction go to at the parent directory of current directory corresponding at file list
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After, the file list change and containt the files of parent directory,
//! also the file selected is the old directory.
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_gotoparent( void );

//! This fonction return the directory name corresponding at the file list
//!
//! @param     sz_path        buffer to store the name file (ASCII or UNICODE )
//! @param     u8_size_max    buffer size (unit ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_name( FS_STRING sz_path  , U8 u8_size_max  );

//! This fonction create a sub directory in the directory correcponding at file list
//!
//! @param     sz_name     buffer with the directory name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_make( const FS_STRING sz_name  );

//! This fonction return the path of the current file list
//!
//! @param     sz_path              buffer to store path (ASCII or UNICODE )
//! @param     u8_size_max          buffer size (unit ASCII or UNICODE )
//! @param     b_view_file_select   TRUE, include in path the name of file selected
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_getcwd( FS_STRING sz_path  , U8 u8_size_path , Bool b_view_file_select  );

//! This fonction init navigator position via a path
//!
//! @param     sz_path           buffer with the path (ASCII or UNICODE )
//! @param     b_match_case      FALSE to ignore the case
//! @param     b_create          TRUE, if path no exist then create it <br>
//!                              FALSE, no create path <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! The syntact "./../../file_name" is supported.
//! With syntact "./dir_parent/directory_name"  the file list corresponding at "dir_parent" and "directory_name" is selected.
//! With syntact "./dir_parent/directory_name/" the file list corresponding at "directory_name" and no file is selected.
//! @endverbatim
//!
Bool  nav_setcwd( FS_STRING sz_path , Bool b_match_case , Bool b_create );


//*********************** File control fonctions ***********************

//! This fonction return name of file selected or check the name of file selected
//!
//! @param     b_mode         action mode: <br>
//!                           FS_NAME_GET    to get the name of file selected <br>
//!                           FS_NAME_CHECK  to check the name of file selected <br>
//! @param     sz_name        if FS_NAME_GET    then buffer to store the name file (ASCII or UNICODE ) <br>
//!                           if FS_NAME_CHECK  then buffer containt the filter name (ASCII or UNICODE),
//!                                             it must be terminate by NULL or '*' value <br>
//! @param     b_match_case   FALSE, ignore the case (only used in "FS_NAME_CHECK" action mode)
//!
//! @param     u8_size_max    buffer size (unit ASCII or UNICODE ), only used in "FS_NAME_GET" action mode
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_file_name( FS_STRING sz_name , U8 u8_size_max , Bool b_mode , Bool b_match_case  );

//! This fonction return the size of the file selected
//!
//! @return    Size of file selected (unit byte)
//!
U32   nav_file_lgt( void );

//! This fonction return sector size of the file selected
//!
//! @return    Size of file selected (unit sector)
//!
U16   nav_file_lgtsector( void );

//! This fonction check the disk write protection and attribut "read only" of file selected
//!
//! @return    FALSE, it is possible to modify the file selected
//! @return    TRUE, in other case
//!
Bool  nav_file_isreadonly( void );

//! This fonction return the type of file selected
//!
//! @return    TRUE, it is a directory
//! @return    FALSE, in other case
//!
Bool  nav_file_isdir( void );

//! This fonction check the extension of file selected
//!
//! @param     sz_filterext, extension filter in ASCII format (e.g. "txt") (e.g. "txt,d*,wk" )
//!
//! @return    TRUE, the file extension corresponding at extension filter
//! @return    FALSE, in other case
//!
Bool  nav_file_checkext( const FS_STRING sz_filterext );

//! This fonction return the date of file selected
//!
//! @param     type_date      FS_DATE_LAST_WRITE,  to get the date of last write access <br>
//!                           FS_DATE_CREATION,    to get the date of creation
//! @param     sz_date        ASCCI string buffer (>17B) to store the information about date <br>
//!                           "YYYYMMDDHHMMSSMS" = year, month, day, hour, minute, seconde, miliseconde
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_dateget( FS_STRING sz_date , Bool type_date );

//! This fonction return the attribut of file selected
//!
//! @return    attribut of file selected, see file system masks "FS_ATTR_" in fs_com.h file.
//!
U8    nav_file_attributget( void );

//! This fonction change the date of file selected
//!
//! @param     type_date      FS_DATE_LAST_WRITE,  to get the date of last write access <br>
//!                           FS_DATE_CREATION,    to get the date of creation
//! @param     sz_date        ASCCI string buffer containt the informations about date to modify <br>
//!                           "YYYYMMDDHHMMSSMS" = year, month, day, hour, minute, seconde, miliseconde
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_dateset( const FS_STRING sz_date , Bool type_date );

//! This fonction change the attribut of file selected
//!
//! @param   u8_attribut   value to set at file selected, see file system masks "FS_ATTR_" in fs_com.h file.
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_file_attributset( U8 u8_attribut );

//! This fonction delete the file or folder tree selected in navigator
//!
//! @param     b_only_empty      TRUE, del directory selected only if empty <br>
//!                              FALSE, del directorys and files include in directory selected <br>
//!                              If the file selected is not a directory, then this param is ignored.
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_del( Bool b_only_empty );

//! This fonction rename the directory or file selected
//!
//! @param     sz_name     buffer with the name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_rename( const FS_STRING sz_name  );

//! This fonction create a file with size 0 and file attribut 0
//!
//! @param     sz_name     buffer with the name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! Use this routine to create a file and after call file_open() to open this new file
//! @endverbatim
//!
Bool  nav_file_create( const FS_STRING sz_name  );

//! This fonction init the COPY navigator with the position of the current navigator
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! If you use the COPY navigator (see FS_NAV_ID_COPYFILE in conf_explorer.h) after this routine then the copy information is lost.
//! @endverbatim
//!
Bool  nav_file_copy( void );

//! This fonction paste the file selected in COPY navigator in the file list of the current navigator
//!
//! @param     sz_name     buffer with the name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! After this routine, you shall call nav_file_paste_state() to run and way the copy
//! @endverbatim
//!
Bool  nav_file_paste_start( const FS_STRING sz_name  );

//! This fonction execute the copy file
//!
//! @param     b_stop      set TRUE to stop copy action
//!
//! @return    copy status <br>
//!            COPY_BUSY,     copy running
//!            COPY_FAIL,     copy fail
//!            COPY_FINISH,   copy finish
//!
U8    nav_file_paste_state( Bool b_stop );

#endif  // _NAVIGATION_H_
