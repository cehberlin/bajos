/*
* HWR-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* C-functions for native methods*/
/* native void method -> C-function ret value 0*/
/* native non void method -> c-cunction ret value 1 (ret value on java -opStack)*/
/* remember:*/
/* invokespecial Operand Stack*/
/* ..., objectref, [arg0, [arg1 ...]] -> ...*/
/* invokestatic: Operand Stack*/
/* ..., [arg0, [arg1 ...]] -> ..*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../definitions.h"
#include "../bajvm.h"
#include "../stack.h"
#include "../classfile.h"
#include "../scheduler.h"
#include "../heap.h"
#include "native.h"
#ifndef WITHMON
#include "evk1104.h"
#include "compiler.h"
#include "gpio.h"
#include "rtc.h"
#include "intc.h"
#include "usart.h"
#endif
#include "platform.h" 



char nativeCharOut()		{
char val=opStackGetValue(local+1).UInt;
	printf("%c",val);
	return 0;							}

char	nativeCharIn()	{
char ch=conIn();
opStackPush((slot)(u4)ch);
return 1;}

#ifndef WITHMON
char conIn()	{
return usart_getchar(  EXAMPLE_USART );
}

void conOut(char c)	{
while (!usart_tx_ready(  EXAMPLE_USART ));
usart_write_char(EXAMPLE_USART , (int)c);
}

char  conStat()	{
if (usart_test_hit(EXAMPLE_USART )){ opStackPush((slot)(u4)66); return 1;	} /* input available*/
if (!usart_tx_ready(EXAMPLE_USART )){opStackPush((slot)(u4)77); return 1;	} /* no input, transmit ready*/
opStackPush((slot)(u4)0);	/* no char available, transmit no ready*/
return 1;	}
#else
char  conStat()	{
opStackPush((slot)(u4)ConStat());
return 1;
}
#endif



/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: int currentTimeMillis(void);
## Parameters: none
## Return Value: int, get the value of the timer
*/
#ifndef WITHMON

#define EXAMPLE_TARGET_PBACLK_FREQ_HZ FOSC0  // PBA clock target frequency, in Hz

void usart1Init()	{
  static const gpio_map_t USART_GPIO_MAP =
  {
    {EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
    {EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
  };

  // USART options.
  static const usart_options_t USART_OPTIONS =
  {
    .baudrate     = 57600,
    .charlength   = 8,
    .paritytype   = USART_NO_PARITY,
    .stopbits     = USART_1_STOPBIT,
    .channelmode  = USART_NORMAL_CHMODE
  };
    // Assign GPIO to USART.
  gpio_enable_module(USART_GPIO_MAP,
                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

  // Initialize USART in RS232 mode.
  usart_init_rs232(EXAMPLE_USART, &USART_OPTIONS, EXAMPLE_TARGET_PBACLK_FREQ_HZ);
}

char currentTimeMillis() {
	opStackPush((slot) (u4) timerMilliSec);
	return 1;
}
#else
char currentTimeMillis() {
	opStackPush((slot) (u4) GetMilliSec());
	return 1;
}
#endif

#ifndef WITHMON
char nativeExit()	{
	exit(opStackGetValue(local+1).UInt);
return 0;
}
#else
char nativeExit()	{
	Monitor(opStackGetValue(local+1).UInt);

return 0;
}
#endif


