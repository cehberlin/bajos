#include <string.h>
#include "usart.h"
#include "pio.h"
#include "spi.h"
#include "lcdc.h"
#include "utils.h"
#include "at32stk1000.h"
//extern lcdc_conf_t ltv350qv_conf;
//unsigned int * framePtr=(unsigned int *) (ltv350qv_conf.dmabaddr1 | 0xA0000000);
unsigned int * framePtr=(unsigned int *) (AVR32_EBI1_ADDRESS|0xA0000000);


void setPixelRGB(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue)	{
*(framePtr+y*640+x)=((blue << 16) & 0xff0000) | ((green << 8) & 0x00ff00) | (red & 0x000000fF);
}
