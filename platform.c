// übelster stil
#if (AVR32UC3A||AVR32AP7000)
#include "iobinding.h"
#endif 
#ifdef AVR32AP7000
#ifdef NGW100
#include "AVR32AP7000/usartap7000.h"
#include "NGW100/hsdramc1.h"
#include <asm/io.h>
#include <asm/sdram.h>
#include <asm/arch/gpio.h>
#include <asm/arch/hmatrix2.h>
unsigned long sdram_init(const struct sdram_info *info);
#endif
#include <avr32/io.h>
#endif
 
#ifdef EVK1100
#include "EVK1100/spi.h"
#include <avr32/io.h>
#include "EVK1100/pmuc3a.h"
#include "EVK1100/gpiouc3a.h"
#include "EVK1100/evk1100.h"
#include "EVK1100/dip204.h"
#include "EVK1100/countuc3a.h"
#include "EVK1100/rtcuc3a.h"
#include "EVK1100/intcuc3a.h"
#include "EVK1100/sdramc.h"
#endif
#include "platform.h"
#ifdef AVR8
#include <avr/io.h>
#include <avr/interrupt.h>
#include "AVR8/lcd.h"
#define		BYTES(word)			((word)*2)
#define		STRING(a,b)				#a" "#b
#define		INLINEASM(a,b)			STRING(a,b)
#define		LARGEBOOTSTART	0xf000
#define		BOARDRAMEND	0x8000
#define		MONSTART	LARGEBOOTSTART
#define 	MONRAM		CHARONRAMEND-0x100
// Monitorfunktionen (bamo128)
#define		mainLoop		BYTES(LARGEBOOTSTART+2)	// Ruecksprung in Monitor aus Programm mit goto
#define		saveCPU		BYTES(LARGEBOOTSTART+62)	//Time2Comp	// BOOTSTART+62		
FILE uartAVR8 = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
SIGNAL(SIG_OUTPUT_COMPARE2) __attribute__ ((naked));
SIGNAL(SIG_OUTPUT_COMPARE2) {asm volatile  (INLINEASM(jmp,saveCPU));} // monitor für step-betrieb
#endif


#ifdef STK1000
// dmabaseaddr 0x1000 0000
#include <string.h>
#include "STK1000/usart.h"
#include "STK1000/pio.h"
#include "STK1000/spi.h"
#include "STK1000/lcdc.h"
#include "STK1000/utils.h"
#include "STK1000/at32stk1000.h"
void myfill(lcdc_conf_t *lcdc_conf);
void setPixelRGB(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue)	;
extern void ltv350qv_power_on(volatile avr32_spi_t * spi, unsigned char chip_select);
extern int display_virtual_bm(lcdc_conf_t *lcdc_conf, void * bm_file);
extern void usdelay(unsigned long usec);
extern void usart_printHex(volatile avr32_usart_t * usart, const unsigned long n);
void init_spiMaster(volatile avr32_spi_t * spi);
int increment_frame_base(lcdc_conf_t *lcdc_conf, int pixel, int line);
void lcd_pio_config(void);
void init_spiMaster(volatile avr32_spi_t * spi);
void print(volatile struct avr32_usart_t * usart, char *str);
#endif




void initHW(){
#ifdef AVR8
	stdout = stdin=stderr = &uartAVR8;	
LCD_Init();
timer_Init();	// use timer 2 for real time clock
#endif

#if  (AVR32UC3A|| NGW100)
usartInit();
stdIOInit();
#ifdef AVR32UC3A 
initTimer();
#endif
#endif

#ifdef EVK1100
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
  char current_char = 0x10;

  // Display default message.
  dip204_set_cursor_position(8,1);
  dip204_write_string("ATMEL");
  dip204_set_cursor_position(7,2);
  dip204_write_string("EVK1100");
  dip204_set_cursor_position(6,3);
  dip204_write_string("AVR32 UC3");
  dip204_set_cursor_position(3,4);
  dip204_write_string("AT32UC3A Series");
  dip204_hide_cursor();
#endif

#ifdef STK1000
#define SWITCHES_MASK 0x000000FF
#define SWITCH0 0x00000001
#define SWITCH1 0x00000002
#define SWITCH2 0x00000004
#define SWITCH3 0x00000008

/*! \brief Start address of the bitmap file */ 
#define BITMAP_FILE_ADDRESS 0x00400000

/*! \brief LCD controller configuration */
lcdc_conf_t ltv350qv_conf ={
	dmabaddr1: 0x10000000,
	dmabaddr2: 0,
	burst_length: 4,
	xres: 320,
	yres: 240,
	set2dmode: MODE_2D_ON,
	virtual_xres: 640,
	virtual_yres: 480,
	frame_rate: 75,
	lcdcclock: 40000000,
	guard_time: 2,
	memor: BIG_ENDIAN,
	ifwidth: 0,
	scanmod: SINGLE_SCAN,
	distype: TFT,
	invvd: NORMAL,
	invframe: INVERTED,
	invline: INVERTED,
	invclk: INVERTED,
	invdval: INVERTED,
	clkmod: ALWAYS_ACTIVE,
	pixelsize: BPP_32,
	ctrstval: 0x0f,
	ctrst_ena: ENABLED,
	ctrst_pol: NORMAL,
	ctrst_ps: PRE_HALF,
	mval: 0,
	mmode: EACH_FRAME,
	hpw: 16,
	hbp: 15,
	hfp: 33,
	vpw: 1,	
	vbp: 10,
	vfp: 10,
	vhdly: 0,
};
	volatile avr32_spi_t * spi = &AVR32_SPI0;
	unsigned int input;
	volatile avr32_pio_t *piob = &AVR32_PIOB;
	volatile avr32_pm_t *ppm = &AVR32_PM;
	
	board_init();
	piob->per = SWITCHES_MASK;
	piob->codr = SWITCHES_MASK;
	lcd_pio_config();
	
	memset((void *)ltv350qv_conf.dmabaddr1, 0, ltv350qv_conf.virtual_xres * ltv350qv_conf.virtual_yres * ltv350qv_conf.pixelsize / 8);
	
	//print(usart, "Setting up SPI for LTV350QV panel\n");
	init_spiMaster(spi);
	
	//print(usart,"Initializing LTV350QV panel\n");
	ltv350qv_power_on(spi, 1);
	
	//print(usart, "Enabling LCD controller\n");
	/* Power manager setup 
	* Enable CLOCK for LCDC in HSBMASK
	*/
	ppm->hsb_mask |= 0x00000080;	
	/* Enable generic clock PLL0 for LCD controller pixel clock*/
	ppm->gcctrl[7] |= 0x00000006;
	
//	print(usart, "Initializing LCD controller\n");
	lcdc_init(&ltv350qv_conf);
	
	//print(usart, "Filling frame buffer with data\n");
	/* print the image into the virtual framebuffer */
	//myfill(&ltv350qv_conf);
unsigned short x,y;
//unsigned char red,green,blue;
for (x=20; x<300;x++)	 {
setPixelRGB(x, 30, 255, 0, 0);
setPixelRGB(x, 200, 0, 255, 0);
	}
for (y=30; y<200;y++)	 {
setPixelRGB(20, y, 0, 0, 255);
setPixelRGB(300, y,255, 255, 255);
}

stdIOInit();
#endif

#ifdef NGW100
static const struct sdram_info sdram = {
	.phys_addr	= NGW_SDRAM_BASE,
	.row_bits	= 13,
	.col_bits	= 9,
	.bank_bits	= 2,
	.cas		= 3,
	.twr		= 2,
	.trc		= 7,
	.trp		= 2,
	.trcd		= 2,
	.tras		= 5,
	.txsr		= 5,
};
	hmatrix2_writel(SFR4, 1 << 1);

	gpio_enable_ebi();
	gpio_enable_usart1();
#define CONFIG_MACB
#define CONFIG_MMC
#if defined(CONFIG_MACB)
	gpio_enable_macb0();
	gpio_enable_macb1();
#endif
#if defined(CONFIG_MMC)
	gpio_enable_mmci();
#endif
sdram_init(&sdram);
#endif
}

#ifdef AVR32UC3A
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
#endif
void VT102Attribute (u1 fgcolor, u1 bgcolor)	{
    printf("%c",0x1b);
      printf("%c",'[');
//            printf("%c",'4');
     printf("%c",fgcolor);
      //printf("%c",';');
      //printf("%c",40 + bgcolor);
      printf("%c",'m');
}


#ifdef AVR8
void timer_Init()	{
OCR0=0x58;		// Timer zählt bis OCR0, dann Int und Reset Timer, Experimentell bestimmt
		TCCR0 = (1<<CS02) | (1<<CS00) | (1<<WGM01);	// Vorteiler 1024 zu Systemtakt 20 MHz??
		TIMSK |= (1<<OCIE0);
		sei();	// global interrupt zulassen
}
char currentTimeMillis()	{
opStackPush((slot)(u4)timerMilliSec);
	return 1;
}
SIGNAL(SIG_OUTPUT_COMPARE0)		{
	timerMilliSec++;	
}

	int uart_putchar(char c, FILE *stream){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	if (c=='\n') uart_putchar('\r',stream);
	return 0;								}

int uart_getchar(FILE *stream)	{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return (int)UDR0;	
}
#endif

#ifdef STK1000
/*! \brief USART used for console output */
volatile avr32_usart_t *usart = &AVR32_USART1;

 /*! \brief Initialise SPI in master mode for the LCD
 *
 *  \param spi Pointer to the correct avr32_spi_t struct
 */
void init_spiMaster(volatile avr32_spi_t * spi)
{

	struct spi_options_t spiOptions;
	avr32_piomap_t spi_piomap = {					\
		{AVR32_SPI0_SCK_0_PIN, AVR32_SPI0_SCK_0_FUNCTION},	\
		{AVR32_SPI0_MISO_0_PIN, AVR32_SPI0_MISO_0_FUNCTION},	\
		{AVR32_SPI0_MOSI_0_PIN, AVR32_SPI0_MOSI_0_FUNCTION},	\
		{AVR32_SPI0_NPCS_0_PIN, AVR32_SPI0_NPCS_0_FUNCTION},	\
		{AVR32_SPI0_NPCS_1_PIN, AVR32_SPI0_NPCS_1_FUNCTION},	\
		{AVR32_SPI0_NPCS_2_PIN, AVR32_SPI0_NPCS_2_FUNCTION},	\
		{AVR32_SPI0_NPCS_3_PIN, AVR32_SPI0_NPCS_3_FUNCTION},	\
	};
	pio_enable_module(spi_piomap, 7);
	
	spiOptions.reg = 1;
	spiOptions.baudrate = 1500000;
	spiOptions.bits = 8;
	spiOptions.spck_delay = 0;
	spiOptions.trans_delay = 0;
	spiOptions.stay_act = 1;
	spiOptions.spi_mode = 3;
	
	spiOptions.fdiv = 0;
	spiOptions.modfdis = 0;

	/* Initialize as master */
	spi_initMaster(spi, &spiOptions);

	/* Set master mode; variable_ps, pcs_decode, delay */
	spi_selectionMode(spi, 0, 0, 0);

	/* Select slave chip 1 (SPI_NPCS1) */
	spi_selectChip(spi, 1);

	spi_setupChipReg(spi, &spiOptions, pm_read_module_frequency(PM_PBA_SPI0));

	spi_enable(spi);
}

/*! \brief Print a string of characters to an usart
 *
 * \param *usart The usart to write to
 * \param *str The string of characters
 *
 * \return;
 */
void print(volatile struct avr32_usart_t * usart, char *str)
{
	while (*str != '\0')
		usart_putchar(usart, *str++);
}

/*! \brief Sets up the pins for the LCD on the STK1000
 * 
 */
void lcd_pio_config(void){
	
		avr32_piomap_t piomap = {
			{ AVR32_LCDC_CC_0_0_PIN, AVR32_LCDC_CC_0_0_FUNCTION },
			{ AVR32_LCDC_DVAL_0_0_PIN, AVR32_LCDC_DVAL_0_0_FUNCTION },
			{ AVR32_LCDC_HSYNC_0_PIN, AVR32_LCDC_HSYNC_0_FUNCTION },
			{ AVR32_LCDC_MODE_0_0_PIN, AVR32_LCDC_MODE_0_0_FUNCTION },
			{ AVR32_LCDC_PCLK_0_PIN, AVR32_LCDC_PCLK_0_FUNCTION },
			{ AVR32_LCDC_PWR_0_PIN, AVR32_LCDC_PWR_0_FUNCTION },
			{ AVR32_LCDC_VSYNC_0_PIN, AVR32_LCDC_VSYNC_0_FUNCTION },
			{ AVR32_LCDC_DATA_0_0_PIN, AVR32_LCDC_DATA_0_0_FUNCTION },
			{ AVR32_LCDC_DATA_1_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_2_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_3_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_4_0_PIN, AVR32_LCDC_DATA_1_0_FUNCTION },
			{ AVR32_LCDC_DATA_5_PIN, AVR32_LCDC_DATA_5_FUNCTION },
			{ AVR32_LCDC_DATA_6_PIN, AVR32_LCDC_DATA_6_FUNCTION },
			{ AVR32_LCDC_DATA_7_PIN, AVR32_LCDC_DATA_7_FUNCTION },
			{ AVR32_LCDC_DATA_8_0_PIN, AVR32_LCDC_DATA_8_0_FUNCTION },
			{ AVR32_LCDC_DATA_9_0_PIN, AVR32_LCDC_DATA_9_0_FUNCTION },
			{ AVR32_LCDC_DATA_10_0_PIN, AVR32_LCDC_DATA_10_0_FUNCTION },
			{ AVR32_LCDC_DATA_11_0_PIN, AVR32_LCDC_DATA_11_0_FUNCTION },
			{ AVR32_LCDC_DATA_12_0_PIN, AVR32_LCDC_DATA_12_0_FUNCTION },
			{ AVR32_LCDC_DATA_13_PIN, AVR32_LCDC_DATA_13_FUNCTION },
			{ AVR32_LCDC_DATA_14_PIN, AVR32_LCDC_DATA_14_FUNCTION },
			{ AVR32_LCDC_DATA_15_PIN, AVR32_LCDC_DATA_15_FUNCTION },
			{ AVR32_LCDC_DATA_16_0_PIN, AVR32_LCDC_DATA_16_0_FUNCTION },
			{ AVR32_LCDC_DATA_17_0_PIN, AVR32_LCDC_DATA_17_0_FUNCTION },
			{ AVR32_LCDC_DATA_18_0_PIN, AVR32_LCDC_DATA_18_0_FUNCTION },
			{ AVR32_LCDC_DATA_19_0_PIN, AVR32_LCDC_DATA_19_0_FUNCTION },
			{ AVR32_LCDC_DATA_20_0_PIN, AVR32_LCDC_DATA_20_0_FUNCTION },
			{ AVR32_LCDC_DATA_21_0_PIN, AVR32_LCDC_DATA_21_0_FUNCTION },
			{ AVR32_LCDC_DATA_22_PIN, AVR32_LCDC_DATA_22_FUNCTION },
			{ AVR32_LCDC_DATA_23_PIN, AVR32_LCDC_DATA_23_FUNCTION }
			
	};
	pio_enable_module(piomap, 31);
	
}
/*! \brief Move the viewing area in the virtual frame buffer
 * 
 *  \param lcdc_conf Pointer to the LCD controller configuration
 *  \param pixel Number of pixels to move the viewing area
 *  \param line Number of lines to move the viewing area
 * 
 *  \return Returns the status of the movement
 *    \retval 0 Movement succeeded
 *    \retval -1 Movement was not successfull (viewing are is out of the 
 *               virtual frame buffer
 */
int increment_frame_base(lcdc_conf_t *lcdc_conf, int pixel, int line){
	
	volatile avr32_lcdc_t *plcdc = &AVR32_LCDC;
	unsigned long base1;
	
	/* increment frame pointer by lines */
	base1 = plcdc->dmabaddr1 + lcdc_conf->virtual_xres * lcdc_conf->pixelsize / 8 * line;
	/* increment frame pointer by pixel */
	base1 += 4 * pixel;

	/* do not allow to move the viewing area out of the virtual frame buffer */
	if((base1 >= lcdc_conf->dmabaddr1 ) && 
			base1 <= (lcdc_conf->dmabaddr1 + lcdc_conf->virtual_xres * lcdc_conf->pixelsize / 8 * (lcdc_conf->virtual_yres - lcdc_conf->yres) ))
		plcdc->dmabaddr1 = base1;
	else 
		return -1;
	
	/* update DMA configuration DMAUPDT */
	plcdc->dmacon |=  (1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
	return 0;
}
#endif
