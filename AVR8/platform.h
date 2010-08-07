/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
/* ************************************************************************ */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../definitions.h"
#include "../typedefinitions.h"
#include "lcd.h"

u2 loadInSram(char* addr);


/* CPU frequency */
#define F_CPU 14745000UL
/* UART baud rate */
#define UART_BAUD  115200


void initHW();
void VT102Attribute (u1 fgcolor, u1 bgcolor);
void timer_Init();
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
