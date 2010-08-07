/* This header file is part of the ATMEL AT32UC3A-SoftwareFramework-1.1.1 Release */

/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FreeRTOS configuration file.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices can be used.
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


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "board.h"
#include "conf_ctrlpanel.h"


/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION      1
#define configUSE_IDLE_HOOK       0
#define configUSE_TICK_HOOK       0
#define configCPU_CLOCK_HZ        ( CP_CPU_SPEED ) /* Hz clk gen */
#define configPBA_CLOCK_HZ        ( CP_PBA_SPEED )
#define configTICK_RATE_HZ        ( ( portTickType ) 1000 )
#define configMAX_PRIORITIES      ( ( unsigned portBASE_TYPE ) 8 )
#define configMINIMAL_STACK_SIZE  ( ( unsigned portSHORT ) 256 )
#define configTOTAL_HEAP_SIZE     ( ( size_t ) ( 0 ) )
#define configMAX_TASK_NAME_LEN   ( 20 )
#define configUSE_TRACE_FACILITY  1
#define configUSE_16_BIT_TICKS    0
#define configIDLE_SHOULD_YIELD   1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES     0
#define configMAX_CO_ROUTINE_PRIORITIES ( 0 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetCurrentTaskHandle   1


/* configTICK_USE_TC is a boolean indicating whether to use a Timer Counter
   for the tick generation. Timer Counter will generate an accurate Tick;
   otherwise the CPU will generate a tick but with time drift.
   configTICK_TC_CHANNEL is the TC channel. */
#define configTICK_USE_TC             1
#define configTICK_TC_CHANNEL         2

/* configHEAP_INIT is a boolean indicating whether to initialize the heap with
   0xA5 in order to be able to determine the maximal heap consumption. */
#define configHEAP_INIT               1

/* Debug trace configuration.
   configDBG is a boolean indicating whether to activate the debug trace. */
#define configDBG                     0
#define configDBG_USART               (&AVR32_USART1)
#define configDBG_USART_RX_PIN        AVR32_USART1_RXD_0_PIN
#define configDBG_USART_RX_FUNCTION   AVR32_USART1_RXD_0_FUNCTION
#define configDBG_USART_TX_PIN        AVR32_USART1_TXD_0_PIN
#define configDBG_USART_TX_FUNCTION   AVR32_USART1_TXD_0_FUNCTION
#define configDBG_USART_BAUDRATE      57600

/*! Turn this define to 1 to display on the trace port info about the current
    list of tasks once per mn.\n
    Else set to 0. */
#define configSTACK_CONSUMPTION_CHECK 1

#if ( configSTACK_CONSUMPTION_CHECK == 1 ) && ( configUSE_TRACE_FACILITY == 0 )
#error "configUSE_TRACE_FACILITY must be set to 1 when configSTACK_CONSUMPTION_CHECK is set to 1."
#endif


/* USB task definitions. */
#define configTSK_USB_NAME            ((const signed portCHAR *)"USB")
#define configTSK_USB_STACK_SIZE      2*256
// #define configTSK_USB_PRIORITY        (tskIDLE_PRIORITY + 4)
#define configTSK_USB_PRIORITY        (configMAX_PRIORITIES - 1)

/* USB device task definitions. */
#define configTSK_USB_DEV_NAME        ((const signed portCHAR *)"UD")
#define configTSK_USB_DEV_STACK_SIZE  256
#define configTSK_USB_DEV_PRIORITY    (tskIDLE_PRIORITY + 3)
#define configTSK_USB_DEV_PERIOD      20

/* USB host task definitions. */
#define configTSK_USB_HST_NAME        ((const signed portCHAR *)"UH")
#define configTSK_USB_HST_STACK_SIZE  256
#define configTSK_USB_HST_PRIORITY    (tskIDLE_PRIORITY + 3)
#define configTSK_USB_HST_PERIOD      20

/* USB device mass-storage task definitions. */
#define configTSK_USB_DMS_NAME        ((const signed portCHAR *)"UDMS")
#define configTSK_USB_DMS_STACK_SIZE  256
#define configTSK_USB_DMS_PRIORITY    (tskIDLE_PRIORITY + 2)
#define configTSK_USB_DMS_PERIOD      20

/* USB host mass-storage task definitions. */
#define configTSK_USB_HMS_NAME        ((const signed portCHAR *)"UHMS")
#define configTSK_USB_HMS_STACK_SIZE  256
#define configTSK_USB_HMS_PRIORITY    (tskIDLE_PRIORITY + 2)
#define configTSK_USB_HMS_PERIOD      20


#endif /* FREERTOS_CONFIG_H */
