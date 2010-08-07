#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "compiler.h"
#ifdef AVR32UC3A
#include "board.h"	
#endif
#include "gpio.h"
#include "pm.h"
#include "iobinding.h"




#ifdef AVR32UC3A
 char conIn()	{
return usart_getchar(EXAMPLE_USART);
}

void conOut(char c)	{
while (!usart_tx_ready(EXAMPLE_USART));
usart_write_char(EXAMPLE_USART, (int)c);
}
#endif

#ifdef AVR32AP7000
char conIn()	{
return (char) usart_getchar(&AVR32_USART1);
}

#define FAILURE -1
#define SUCCESS 0
void conOut(char c)	 {
usart_bw_write_char(&AVR32_USART1, (int) c);
}
#endif

void	hexConOut(char c)	{
conOut(0x30+(( c < 10)?c:(c+'A'-'0'-10)));
}

void	byteConOut(char c)	{
hexConOut((c&0xF0) >>4);
hexConOut(c&0x0F);
}

void	wordConOut(int c)	{
byteConOut((char)((c&0xFF000000) >>24));
byteConOut((char)((c&0x00FF0000) >>16));
byteConOut((char)((c&0x0000FF00) >>8));
byteConOut((char)(c&0x000000FF ));
}
char ascii2Hex(char c)	{
return ((c<='9')?(c-'0'):((c<'G')?(c-'A'+10):(c-'a'+10)));
}
 

int __attribute__((weak)) _read (int file, char * ptr, int len){
int i;
//if ( !do_not_use_oscall_coproc ) return _read_sim(file, ptr, len);
//if (file != 0)return unimplemented_syscall("_read with filedes != 0");
for ( i = 0; i < len; i++ ){
ptr[i] = (char)conIn();
}
return len;
}

/**
* Low-level write command.
* When newlib buffer is full or fflush is called, this will output
* data to correct location.
* 1 and 2 is stdout and stderr which goes to usart
* 3 is framebuffer
*/
int __attribute__((weak)) _write (int file, char * ptr, int len){
int i;
//if ( !do_not_use_oscall_coproc ) return _write_sim(file, ptr, len);
//if ( (file != 1)&& (file != 2) && (file!=3)) return unimplemented_syscall("_write with file != 1 or 2");
/* if(file==3){
for(i = 0 ; i < len; i++){
avr32fb_putchar(ptr[i]);
}
} else */{
for ( i = 0; i < len; i++ ){
conOut(ptr[i]);
if (ptr[i]=='\n') conOut('\r');
}
}
return len;
} 


void stdIOInit()	{	
//To configure standard I/O streams as unbuffered, you can simply:
setbuf(stdin, NULL);
setbuf(stdout, NULL); 
}

#ifdef AVR32UC3A
void usartInit()	{
static const gpio_map_t USART_GPIO_MAP =
  {
    {EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
    {EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
  };
  // USART options.
  static const usart_options_t USART_OPTIONS =
  {
    .baudrate     = 57600,
 //    .baudrate     = 115200,
    .charlength   = 8,
    .paritytype   = USART_NO_PARITY,
    .stopbits     = USART_1_STOPBIT,
    .channelmode  = USART_NORMAL_CHMODE
  };

  // Switch main clock to external oscillator 0 (crystal).
  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
  // Assign GPIO to USART.
  gpio_enable_module(USART_GPIO_MAP,
                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

  // Initialize USART in RS232 mode.
  usart_init_rs232(EXAMPLE_USART, &USART_OPTIONS, FOSC0);
 
 //while (1)	{c=conIn(); conOut(' '); byteConOut(c);} 
}
#endif

#ifdef NGW100

typedef unsigned char avr32_piomap_t[][2];

/* enable output on pins */
int pio_enable_module(avr32_piomap_t piomap, unsigned int size);

void usartInit()	{
int cpu_hz = 20000000;
  struct usart_options_t opt;

  volatile struct avr32_usart_t *usart = &AVR32_USART1;

  avr32_piomap_t usart_piomap = {				   \
    {AVR32_USART1_RXD_0_PIN, AVR32_USART1_RXD_0_FUNCTION}, \
    {AVR32_USART1_TXD_0_PIN, AVR32_USART1_TXD_0_FUNCTION}   \
  };

  // Set options for the USART
  opt.baudrate = 115200;
  opt.charlength = 8;
  opt.paritytype = USART_NO_PARITY;
  opt.stopbits = USART_1_STOPBIT;
  opt.channelmode = USART_NORMAL_CHMODE;

  // Initialize it in RS232 mode
  usart_init_rs232(usart, &opt, cpu_hz);

  // Setup pio for USART
  pio_enable_module(usart_piomap, 2);
}


int pio_enable_module(avr32_piomap_t piomap, unsigned int size)
{
  int i;
  volatile struct avr32_pio_t *pio;

  /* get the base address for the port */
  switch (**piomap/32) {

  case 0:
    pio = &AVR32_PIOA;
    break;
  case 1:
    pio = &AVR32_PIOB;
    break;
  case 2:
    pio = &AVR32_PIOC;
    break;
  case 3:
    pio = &AVR32_PIOD;
    break;
  case 4:
    pio = &AVR32_PIOE;
    break;
  default :
    return FAILURE;

  }

  for(i=0; i<size; i++){

    pio->pdr |= ( 1<<( (**piomap) % 32) );
    pio->pudr |= ( 1<<( (**piomap) % 32) );

    switch( *(*piomap+1) ){    
    case 0:
      pio->asr |= ( 1<<( (**piomap) % 32) );
      break;
    case 1:
      pio->bsr |= ( 1<<( (**piomap) % 32) );
      break;
    default:
      return FAILURE;
    }

    ++piomap;

  }
  
  return SUCCESS;
}
#endif
