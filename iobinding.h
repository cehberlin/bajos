/*! \name USART Settings
 */
//! @{
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
#ifdef AVR32UC3A
#include "EVK1100/usartuc3a.h"
#endif
#ifdef AVR32AP7000
#include "AVR32AP7000/usartap7000.h"
#endif
char	conIn();
void	conOut(char c);
void	hexConOut(char c);
void	byteConOut(char c);
void	wordConOut(int c);
char ascii2Hex(char c);	
 
int __attribute__((weak)) _read (int file, char * ptr, int len);
int __attribute__((weak)) _write (int file, char * ptr, int len); 
void stdIOInit();
void usartInit();
