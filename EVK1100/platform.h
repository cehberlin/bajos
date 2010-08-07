/*! \name USART Settings
 */
//! @{
#define BOARD EVK1100
#if BOARD == EVK1100
#  define EXAMPLE_USART               (&AVR32_USART1)
#  define EXAMPLE_USART_RX_PIN        AVR32_USART1_RXD_0_PIN
#  define EXAMPLE_USART_RX_FUNCTION   AVR32_USART1_RXD_0_FUNCTION
#  define EXAMPLE_USART_TX_PIN        AVR32_USART1_TXD_0_PIN
#  define EXAMPLE_USART_TX_FUNCTION   AVR32_USART1_TXD_0_FUNCTION
#elif BOARD == EVK1101
#  define EXAMPLE_USART               (&AVR32_USART1)
#  define EXAMPLE_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
#  define EXAMPLE_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
#  define EXAMPLE_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
#  define EXAMPLE_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
#endif
//! @}


/*! \brief This is an example demonstrating the USART RS232 TX and RX
 *         functionalities using the USART driver.
 */
#include <stdio.h>
#include "usart.h"
#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "compiler.h"
#include "board.h"	
#include "gpio.h"
#include "pm.h"
char	conIn();
void	conOut(char c);
void	hexConOut(char c);
void	byteConOut(char c);
void	wordConOut(int c);
char ascii2Hex(char c);	
void initTimer();
 
int __attribute__((weak)) _read (int file, char * ptr, int len);
int __attribute__((weak)) _write (int file, char * ptr, int len); 
void stdIOInit();
void usart0Init();
void usart1Init();
void initHW();
