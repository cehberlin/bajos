/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
// fuer lehrzwecke,...
#include <stdio.h>
#include <avr32/io.h>
#include "platform.h"
#include "spi.h"
#include "pmuc3a.h"
#include "gpiouc3a.h"
#include "evk1100.h"
#include "dip204.h"
#include "countuc3a.h"
#include "rtcuc3a.h"
#include "intcuc3a.h"
#include "sdramc.h"
#include "../typedefinitions.h"
#include "../bajvm.h"

// 0 for terminal

void initHW()	{
usart1Init();
usart0Init();
stdIOInit(); 
initTimer();
sdramc_init(FOSC0);

static const gpio_map_t DIP204_SPI_GPIO_MAP =
  {
    {DIP204_SPI_SCK_PIN,  DIP204_SPI_SCK_FUNCTION },  // SPI Clock.
    {DIP204_SPI_MISO_PIN, DIP204_SPI_MISO_FUNCTION},  // MISO.
    {DIP204_SPI_MOSI_PIN, DIP204_SPI_MOSI_FUNCTION},  // MOSI.
    {DIP204_SPI_NPCS_PIN, DIP204_SPI_NPCS_FUNCTION}   // Chip Select NPCS.
  };

 // Switch the CPU main clock to oscillator 0
  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

 // add the spi options driver structure for the LCD DIP204
  spi_options_t spiOptions =
  {
    .reg          = DIP204_SPI_CS,
    .baudrate     = 1000000,
    .bits         = 8,
    .spck_delay   = 0,
    .trans_delay  = 0,
    .stay_act     = 1,
    .spi_mode     = 0,
    .fdiv         = 0,
    .modfdis      = 1
  };

  // Assign I/Os to SPI
  gpio_enable_module(DIP204_SPI_GPIO_MAP,
                     sizeof(DIP204_SPI_GPIO_MAP) / sizeof(DIP204_SPI_GPIO_MAP[0]));

  // Initialize as master
  spi_initMaster(DIP204_SPI, &spiOptions);

  // Set selection mode: variable_ps, pcs_decode, delay
  spi_selectionMode(DIP204_SPI, 0, 0, 0);

  // Enable SPI
  spi_enable(DIP204_SPI);

  // setup chip registers
  spi_setupChipReg(DIP204_SPI, &spiOptions, FOSC0);

  // configure local push buttons
 // dip204_example_configure_push_buttons_IT();

  // configure local joystick
 // dip204_example_configure_joystick_IT();

  /* initialize LCD */
  dip204_init(99);//backlight_PWM);
  /* reset marker */
 // char current_char = 0x10;

  // Display default message.
  dip204_set_cursor_position(7,1);
  dip204_write_string("FWR-BA");
  dip204_set_cursor_position(1,2);
  dip204_write_string("EVK1100-AVR32 UC3A");
  dip204_set_cursor_position(8,3);
  dip204_write_string("BAJOS");
  dip204_set_cursor_position(3,4);
  dip204_write_string("Informatik");
  dip204_hide_cursor();
}

/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: void rtc_irq(void);
## Parameters: none
## Return Value: none
##
## Interrupt handler for Timer
*/
#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
/* RTC Interrupt  */
#pragma handler = RTC_INT_GROUP, 1
__interrupt
#endif
void rtc_irq(){
  timerMilliSec++;
  // clear the interrupt flag
  rtc_clear_interrupt(&AVR32_RTC);
}
/*
## Author: H.-Christian Hecht, CoMedServ GmbH, IT 2006
## Method: void initTimer(void);
## Parameters: none
## Return Value: none
##
## start the timer, set the interrupt handling method
*/
void initTimer() 
{
  // Disable all interrupts. */
  Disable_global_interrupt();
  // The INTC driver has to be used only for GNU GCC for AVR32.
#if __GNUC__
  // Initialize interrupt vectors.
  INTC_init_interrupts();
  // Register the RTC interrupt handler to the interrupt controller.
  INTC_register_interrupt(&rtc_irq, AVR32_RTC_RTC_IRQ, INT0);
#endif
  // Initialize the RTC
//psel = log(Fosc/Frtc)/log(2)-1; Frtc wanted f, Fosc 32KHz
  if (!rtc_init(&AVR32_RTC, RTC_OSC_32KHZ,4))// 1khz
  {
    //usart_write_line(&AVR32_USART0, "Error initializing the RTC\r\n");
    {printf("Error initializing the RTC\r\n");
    exit(-1);	}
  }
  // Set top value to 0 to generate an interruption every Milli-seconds */
  rtc_set_top_value(&AVR32_RTC, 0);
  // Enable the interruptions
  rtc_enable_interrupt(&AVR32_RTC);
  // Enable the RTC
  rtc_enable(&AVR32_RTC);
  // Enable global interrupts
  Enable_global_interrupt();
}
void delay_ms(u4 delayMilliSec)
{
u4 start=timerMilliSec;
while ((timerMilliSec-start)<delayMilliSec);
}


int __attribute__((weak
)) _read (int file, char * ptr, int len){
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

void usart0Init()	{
static const gpio_map_t USART_GPIO_MAP =
  {
    {AVR32_USART0_RXD_0_PIN, AVR32_USART0_RXD_0_FUNCTION},
    {AVR32_USART0_TXD_0_PIN, AVR32_USART0_TXD_0_FUNCTION}
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
  usart_init_rs232(&AVR32_USART0, &USART_OPTIONS, FOSC0);
}


void usart1Init()	{
static const gpio_map_t USART_GPIO_MAP =
  {
    {AVR32_USART1_RXD_0_PIN, AVR32_USART1_RXD_0_FUNCTION},
    {AVR32_USART1_TXD_0_PIN, AVR32_USART1_TXD_0_FUNCTION}
  };
  // USART options.
  static const usart_options_t USART_OPTIONS =
  {
    .baudrate     = 1200,
 //    .baudrate     = 115200,
    .charlength   = 7,
    .paritytype   = USART_NO_PARITY,
    .stopbits     = USART_2_STOPBITS,
    .channelmode  = USART_NORMAL_CHMODE
  };

  // Switch main clock to external oscillator 0 (crystal).
  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
  // Assign GPIO to USART.
  gpio_enable_module(USART_GPIO_MAP,
                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

  // Initialize USART in RS232 mode.
  usart_init_rs232(&AVR32_USART1, &USART_OPTIONS, FOSC0);
}
