/*************************************************************************\
*           Development board - DS182X 1-Wire Digital Thermometer         *
*-------------------------------------------------------------------------*
* Description : The DS1822 digital thermometer provides 9- to 12-bit      *
*               centigrade temperature measurements and has an alarm      *
*               function with NV user-programmable upper and lower trigger*
*               points. The DS1822 communicates over a 1-Wire bus that by *
*               definition requires only one data line (and ground) for   *
*               communication with a central microprocessor.              *
*                                                                         *
*               It has an operating temperature range of -55�C to +125�C  *
*               and is accurate to +-2.0�C over the range of -10�C to     *
*               +85�C. In addition, the DS1822 can derive power directly  *
*               from the data line ("parasite power"), eliminating the    *
*               need for an external power supply.                        *
*-------------------------------------------------------------------------*
* Author(s)   : CEBA & www.DFSOFT.cz & www.HW.cz                          *
* Developed   : xx.03.2003                Last Update : 07.11.2003        *
* Version     : 1.1.0                                                     *
*-------------------------------------------------------------------------*
* Compiler    : AVR-GCC                   Version : 3.3                   *
* Source file : ds182x.c                                                  *
* Licence     : www.HW.cz - GNU GPL                                       *
*-------------------------------------------------------------------------*
* Target system : Charon II. - ATmega128, Quartz 14.7456 MHz              *
*                              UART - 19200,N,8,1                         *
*                 DS1822 1-Wire Digital Thermometer                       *
*                                                                         *
* Pin assignment:                                                         *
*   Thermometer DS1822                        Charon II. - dev. board     *
*     1  GND                                      GND                     *
*     2  DQ Data In/Out                           PD0                     *
*     3  Vcc                                      Vcc                     *
*-------------------------------------------------------------------------*
* History :                                                               *
* 07.11.03 - added support for DS18B20 devices - CEBA                     *
* 06.11.03 - added support for DS1820, fixed bug with sign bit - CEBA     *
* 27.04.03 - source code ported from x51 platform by - CEBA               *
*                                                                         *
* FIXME :                                                                 *
* - test DS1822 timing with scope                                         *
\*************************************************************************/
#ifndef _DEV_BOARD_DS182X_C
#define _DEV_BOARD_DS182X_C


#include "ds182x.h"

/* forward definitions of local functions */
static void   TM_Write_byte(u_char);
static u_char TM_Read_byte(void);
static void   TM_Write_bit(u_char);
static u_char TM_Read_bit(void);
static u_char TM_Reset(void);

#if DS_DEBUG
 static void hex_dump (u_char *buf, u_int length);
#endif

#define TM_Enter_Critical()    NutEnterCritical() /* disable all interrupts */
#define TM_Exit_Critical()     NutExitCritical()  /* reenable interrupts */

u_char tm_romdta[8*MAX_CHIP];  /* storage to hold memory pattern */
u_char tm_lst0;                /* last detected zero bit */

/*
 *  TM_Convert_temperature()
 *
 *	This function converts value from raw into an integer data format.
 *	Conversion is chosen based on an 8-bit family code.
 *
 *	Assume:	0x10 - DS1820,
 *			0x10 - DS18S20 - software compatible with the DS1820.
 *	        0x22 - DS1822
 *	        0x28 - DS18B20 - software compatible with the DS1822.
 */
void TM_Convert_temperature(u_char idx, u_int *temp, u_int *frac)
{
	switch(tm_romdta[8*idx])
	{ case 0x10: if(*temp & 0x01) *frac = 5000; /* 4 decimal char. precision */
	             else *frac = 0;

				 *temp >>= 1;
                 break;

	  case 0x22:
	  case 0x28: *frac = ((*temp & (1 << 3)) ? 10000/(1 << 1) : 0) +
                         ((*temp & (1 << 2)) ? 10000/(1 << 2) : 0) +
                         ((*temp & (1 << 1)) ? 10000/(1 << 3) : 0) +
                         ((*temp & (1 << 0)) ? 10000/(1 << 4) : 0);

                 *temp >>= 4;
				 break;
      default:
#if DS_DEBUG
                 printf_P(PSTR("\n\rUnknown family code - %02X.\n\r"), tm_romdta[8*idx]);
#endif
				 *temp = 85;
				 *frac = 0;
				 break;
	}

    if(*temp & 0x80)		/* check the sign bit */
    { *temp &= 0x007F;		/* clear sign bits */
      *temp |= 0x8000;		/* set sign bit */
    }
}

/*
 *  TM_Read_temperature()
 *
 *    This function expects a previous call to TM_Search_rom() which
 *    fills an array of ROM patterns of found devices.
 *
 *  Arguments:
 *    idx is index to this array.
 *
 *  Returns:
 *    0xFFFF (-1) Device not present
 *    0x8000      Error during read (improper CRC)
 *    > 0         Temperature value
 */
u_int TM_Read_temperature(u_char idx)
{
    u_char cnt;
    u_char crc;
    u_int temper;
    u_char temp_buf[9];
    #define crc_read  ((u_char)temp_buf[8]) /* last byte is CRC */

    #if (DS_DEBUG && MULTI_DEVICE) || MULTI_DEVICE
     u_char *ptr_tmp = &tm_romdta[8*idx];
    #endif
    /*
    if (*ptr_tmp != 0x22)
        return -2;          // other device type
    */

    #if DS_DEBUG && MULTI_DEVICE
     hex_dump((char *)ptr_tmp, 8);
    #endif

    TM_Enter_Critical();   /* disable interrupts */

    /* Read previously sampled value ("packet" which last byte is CRC of previous) */
    if (TM_Reset())
    { TM_Exit_Critical();   /* reenable interrupts */
      return -1;            /* device not present (e.g. unplugged) */
    }

    #if !MULTI_DEVICE
     TM_Write_byte(0xCC); /* skip ROM */
    #else
     TM_Write_byte(0x55); /* match ROM */

     cnt = 8;
     do
        TM_Write_byte(*ptr_tmp++);
     while (--cnt);
    #endif

    TM_Write_byte(0xBE);  /* read Scratch Pad */

    for(cnt=0; cnt <= 8; cnt++) temp_buf[cnt] = TM_Read_byte();

    TM_Exit_Critical();   /* reenable interrupts */

    /* check if read byte stream has correct CRC */

    crc = 0;
    for(cnt=0; cnt<sizeof(temp_buf); cnt++) crc = TM_Crc(crc, temp_buf[cnt]);

    #if DS_DEBUG
     hex_dump((char *)&temp_buf, sizeof(temp_buf));
    #endif

    temper = ((temp_buf[1] & 0x0F) << 8) | temp_buf[0];

    if (crc)
    {
        temper |= 0x8000;    /* return error (conversion can be pending) */

        #if DS_DEBUG
         printf_P(PSTR("\n\rTM_read - err:%X(%X!=%X) "), temper, crc, crc_read);
        #endif
    }

    return temper;
}

/*
 *  TM_Sample_temperature()
 *
 *    This function initiates temperature conversion.
 *
 *  Arguments:
 *     idx == 0xFF   skip ROM and initiate conversion on all devices
 */
void TM_Sample_temperature(u_char idx)
{
    u_char *ptr_tmp = &tm_romdta[8*idx];
    u_char cnt;

    /* Issue command to sample temperature to be prepared for next read */

    TM_Enter_Critical();     /* disable interrupts */

    TM_Reset();

    if (idx == 0xFF)
        TM_Write_byte(0xCC); /* skip ROM */
    else
    {
        TM_Write_byte(0x55); /* match ROM */

        cnt = 8;
        do
            TM_Write_byte(*ptr_tmp++);
        while (--cnt);
    }

    TM_Write_byte(0x44);    /* start conversion */

    TM_Exit_Critical();     /* reenable interrupts */
}

#if !MULTI_DEVICE
/*
 *  TM_Read_rom()
 *
 *    If there is a single device on a 1-wire bus
 *    its ROM code can be read by this function.
 *
 *    If more devices are present logical AND of their ROM will be read.
 *
 *    Currently not used. Will be removed from link automatically.
 */
u_char TM_Read_rom(u_char *ptr)
{
    u_char stat = 0,
           cnt = 8,
           crc = 0,
           *fam_ptr = ptr;

    #if DS_DEBUG
     printf_P(PSTR("\n\rReading ROM Code\n\r"));
    #endif

    TM_Enter_Critical();              /* disable interrupts */

    if (!TM_Reset())
    {
        TM_Write_byte(0x33);

        do
          *ptr++ = TM_Read_byte();    /* read ROM code */
        while(--cnt);

        TM_Exit_Critical();           /* reenable interrupts */

	cnt = 8;
  	ptr = fam_ptr;

        do
          crc = TM_Crc(crc, *ptr++);  /* calculate CRC */
        while(--cnt);

        if ( !crc && *fam_ptr != 0)
	 stat = 1;                    /* reading successfully finished */
    }
    else  TM_Exit_Critical();         /* reenable interrupts */

    return stat;
}
#endif

/*
 *  TM_Search_rom()
 *
 *    This function scans all devices up to MAX_CHIP on 1-wire "network".
 *    It fills passed array with ROM patterns of found devices.
 *    This array is supposed to be used with TM_Read_temperature().
 */
u_char TM_Search_rom(u_char *ptr)
{
    u_char  tm_cnt,   /* number of found devices (thermometers) */
            st;
    u_char  cnt = MAX_SEARCH;

    memset(ptr, (u_char)0, 8 * MAX_CHIP);

    tm_lst0 = 0;
    tm_cnt = 0;

    do
    {	   
        st = TM_Search_next(ptr);

        #if DS_DEBUG
         printf_P(PSTR("st:%u "), st);
        #endif

        if (st)
        {
            tm_cnt++;
            if ((st == 1) && (tm_cnt < MAX_CHIP))
            {		
                memcpy(ptr + 8, ptr, 8);
                ptr += 8;
            }
        }
        else
        {
            /*  if 1-wire bus is empty no device responds, this prevents endless loop */
            if (!--cnt)
                break;
        }

    } while ((st != 2) && (tm_cnt < MAX_CHIP));

    return (tm_cnt | ((st != 2) ? 0x80 : 0));
}

/*
 *  TM_Search_next()
 *
 *    Search for a single device.
 */
u_char TM_Search_next(u_char *ptr)
{
    u_char  i = 0, x = 0, lst_dif = tm_lst0;
    u_char  mask = 0x01;
    u_char  tm_rombit;

	
    TM_Enter_Critical();    /* disable interrupts */

    
    if (!TM_Reset()) {		
        TM_Write_byte(0xF0);	
    }
    else
     {  	
	TM_Exit_Critical(); /* reenable interrupts */	
        return 0;
     }

    ptr--;                          /* adjust pointer to satisfy algorithm within loop below */

    while ((i++ < 64) && (x < 3))   /* 8 bytes */
    {
        if (!((i-1) % 8))
        {
            mask = 0x01;            /* mask within a byte */
            ptr++;                  /* skip to next byte if 8 bits passed */
        }
        else
            mask <<= 1;             /* next bit */


        x = ((u_char)TM_Read_bit()) << 1;

        x |= TM_Read_bit();

        if (!x)
        {
            if (i > lst_dif)
                tm_rombit = 0;
            else
            if (i == lst_dif)       /* to device where conflict was last time use 1 now (it was 0 for previous device) */
                tm_rombit = 1;
            else
                tm_rombit = *ptr & mask;
                                    /* seed with bit value of previously found device */

            if (!tm_rombit)         /* last bit not set position */
                tm_lst0 = i;
        }
        else
        #if 0   /* did not worked (against spec ?) */
            tm_rombit = (x & 0x02);  /* or (x & 1) ^ 1 (inverted LSb) */
        #else
            tm_rombit = (x & 1) ^ 1; /* inverted LSb */
        #endif

        if (tm_rombit)
            *ptr |= mask;
        else
            *ptr &= ~mask;
	
        TM_Write_bit(tm_rombit);
    }    
    TM_Exit_Critical();     /* reenable interrupts */    

    #if DS_DEBUG
     hex_dump((char *)&tm_romdta, sizeof(tm_romdta));
    #endif

    return (x == 3) ? 0 : 1 + (lst_dif == tm_lst0);
}

/*
 *  TM_Crc - calculate CRC (Cyclic Redundancy Check) for
 *           Dallas Semiconductor Touch Memory.
 */
u_char TM_Crc(u_char crc, u_char inp)
{
    u_char inp_z = inp,
           i = 8,
           carry;

    do
    {
        inp ^= crc;
        crc >>= 1;

        if (inp & 1)
        {
            crc ^= (0x18 >> 1);
            crc |= 0x80;
        }

        carry = inp_z & 1;
        inp_z >>= 1;

        if (carry)
            inp_z |= 0x80;
        inp = inp_z;

    } while (--i);

    return crc;
}

/*
 *  TM_WRITE_BYTE - writes a byte to the one-wire bus.
 */
static void TM_Write_byte(u_char val)
{
    u_char cnt = 8;

    do
    {
      TM_Write_bit ((val & 1));
      val >>= 1;
    } while (--cnt);
}

/*
 *  TM_READ_BYTE - reads a byte from the one-wire bus.
 */
static u_char TM_Read_byte(void)
{
    u_char cnt = 8;
    u_char value = 0;

    do
    {
        value >>= 1;
        value |= (TM_Read_bit()) ? 0x80 : 0x00;         /* read one bit */
    } while (--cnt);

    return value;
}

/*
 *  TM_WRITE_BIT - writes a byte to the one-wire bus, passed in u_charval.
 */
static void TM_Write_bit(u_char bitval)
{
    sbi(DS_DDR, DQ);        /* set DQ pin direction - output */
    cbi(DS_PORT, DQ);       /* pull DQ low to start timeslot */

COMPRESS_DISABLE;
     _NOP();                /* wait 68 ns */
     _NOP();                /* wait 68 ns */
     _NOP();                /* wait 68 ns */
COMPRESS_REENABLE;
	 
    if( bitval ) sbi(DS_PORT, DQ); /* set DQ */
    else cbi(DS_PORT, DQ);

    TM_Delay(3);            /* hold value for remainder of timeslot */
    sbi(DS_PORT, DQ);       /* DQ = 1 */
    TM_Delay(1);            /* finish timeslot */
}

/*
 *  TM_READ_BIT - reads a byte from the one-wire bus.
 */
static u_char TM_Read_bit(void)
{
    u_char val;

    sbi(DS_DDR, DQ);        /* set DQ pin direction - output */
    cbi(DS_PORT, DQ);       /* pull DQ low to start timeslot */

COMPRESS_DISABLE;
     _NOP();                /* wait 68 ns */
     _NOP();                /* wait 68 ns */
     _NOP();                /* wait 68 ns */
COMPRESS_REENABLE;

    sbi(DS_PORT, DQ);       /* then return high DQ = 1 */

    TM_Delay(1);            /* wait 15 us */

    cbi(DS_DDR, DQ);        /* set DQ pin direction - input */
    sbi(DS_PORT, DQ);       /* enable AVR internal pull-up resistor */

    bit_is_set(DS_PIN, DQ) ? (val = 1) : (val = 0); /* read DQ status */

    TM_Delay(4);            /* wait for end of timeslot */

    return val;             /* return value of DQ line */
}

/*
 *  TM_RESET - the initialization sequence (reset pulse and presence pulse(s)).
 *
 *  Note: All transactions on the 1-Wire bus begin with an initialization sequence.
 */
static u_char TM_Reset(void)
{
    u_char presence;

    sbi(DS_DDR, DQ);        /* set DQ pin direction - output */

    cbi(DS_PORT, DQ);       /* pull DQ line low */

    TM_Delay(32);           /* leave it low for 480us */

                            /* allow DQ line to return high */
    cbi(DS_DDR, DQ);        /* set DQ pin direction - input */
   // sbi(DS_PORT, DQ);       /* enable AVR internal pull-up resistor */

   // sbi(DS_PORT, DQ);
    TM_Delay(4);            /* wait for presence */

                            /* get presence signal */

   if(bit_is_set(DS_PIN, DQ))
   {
   presence = 1;
   }
   else
   {
   presence = 0;
   }						
//    bit_is_set(DS_PIN, DQ) ? (presence = 1) : (presence = 0);

    TM_Delay(20);           /* wait for end of timeslot */

    return presence;        /* presence signal returned
                             * 0 = presence, 1 = no part
                             */
}

/*
 *  TM_Delay()
 *
 *  approximately 15us delay - this is vital for DQ !!
 *                             change number of nops to adjust the delay
 *
 */
void TM_Delay(u_char cnt)
{
  /* 14,7456 MHz ... 1x nop ... 68 ns */

  #define Delay1us()  _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP();  \
                      _NOP()

COMPRESS_DISABLE;
  do
  { Delay1us();       /* 1us */
    Delay1us();       /* 2us */
    Delay1us();       /* 3us */
    Delay1us();       /* 4us */
    Delay1us();       /* 5us */
    Delay1us();       /* 6us */
    Delay1us();       /* 7us */
    Delay1us();       /* 8us */
    Delay1us();       /* 9us */
    Delay1us();       /* 10us */
    Delay1us();       /* 11us */
    Delay1us();       /* 12us */
    Delay1us();       /* 13us */
    Delay1us();       /* 14us */
    Delay1us();       /* 15us */
  }while (--cnt);
COMPRESS_REENABLE;
}


/*
 *  TM_Init()
 *
 *    This is here to initialize the 1-Wire bus devices.
 *
 *   Returns:
 *    > 0  ...  Number of appeared thermometeres on the 1-Wire bus.
 *      0  ...  Thermometer not present.
 */
u_char TM_Init(void)
{
  u_char tm_cnt = 0;

  #if !MULTI_DEVICE && DS_DEBUG   
   TM_Read_rom(&tm_romdta[0]);
   hex_dump((char *)tm_romdta, 8);
  #endif

  /* TM_Search_rom() returns 0x80 if thermometer not connected */
 
  tm_cnt = 0x7F & TM_Search_rom(&tm_romdta[0]);

  if( tm_cnt )
  { /* here you can check if TM_Crc() works */

    #if 0
    { u_char  i, crc = 0;

      for (i = 0; i < 7; i++) crc = TM_Crc(crc, tm_romdta[i]);

      printf_P(PSTR("\ncrc:%X,%X\n"), crc, tm_romdta[7]);
    }
    #endif

    /* start conversion on all thermometers */   
   TM_Sample_temperature(0xFF);
  }

  return tm_cnt;
}

/*
 *  TM_Scan()
 *
 *    If e.g. 1 thermometer is unplugged this can be called to refresh address table.
 *
 */
#if 0                     /* function is not currently used */
void TM_Scan(void)
{
  /* TM_Search_rom() returns 0x80 if thermometer not connected */
  return 0x7F & TM_Search_rom(&tm_romdta[0]);
}
#endif

/*
 * Display some debugging information - ascii/hex dump.
 */
#if DS_DEBUG
void hex_dump (u_char *buf, u_int length)
{ u_char abuf[19] = "[                ]\x0";	/* ascii buffer */
  u_int acnt = 1;								/* ascii counter */

  while(length--)
  {
	printf_P(PSTR("%02X "), *buf);

	if(*buf > ' ') abuf[acnt] = *buf;
	else abuf[acnt] = ' ';

	buf++;

	if(acnt++ == (sizeof(abuf)-3))
	{ printf_P(PSTR("   %s\n"), abuf);
   	  acnt = 1;
	}
  }

  if(acnt>1)
  { do
	{ printf_P(PSTR("   "));
	  abuf[acnt] = ' ';
	}while ( acnt++ < (sizeof(abuf)-3) );

    printf_P(PSTR("   %s\n"), abuf);
  }
}
#endif

#endif
/* ---------------------------- End Of File ------------------------------ */

