#include <stdio.h>
#include "usart.h"
#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "compiler.h"
#include "evk1100.h"	
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


void local_start_pll0(volatile avr32_pm_t* pm);
static void software_delay(void);
__attribute__((__interrupt__))	static void compare_irq_handler(void);
void dip204_example_configure_push_buttons_IT(void);
void dip204_example_configure_joystick_IT(void);
void dip204_example_configure_push_buttons_IT(void);


void exit(int status);
