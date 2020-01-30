/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


    lcd.c:
          LCD-driver SpARM-board v1

    pin-info:
           PE3  - DB7
           PE4  - DB6
           PE5  - DB5
           PE6  - DB4
           PD0  - EN: enable
           PC13 - RS: register-select
           PC11 - RW: read/write

    opmerkingen:
                -Alle instructies voor het display staan achterstevoren! (maakt
                 de code wat eenvoudiger)
                -Zorg ervoor dat de delay-code ook wordt meegecompileerd, anders
                 gaat het hier mis.
                -Deze code is gebaseerd op
                 "LCD routines for OLIMEX LPC-MT-2106,16x2 Character LCD 1602K,
                  4-bit mode operation",
                 Copyright (c) 2004 senz at arm.dreamislife.com
*/

/****************Libraries******************************/
/* Libraries needed for LCD are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_gpio.h"
 */

/****************Defines********************************/
#define  STARTBIT       0
#define  LCD_D7         GPIO_Pin_6
#define  LCD_D6         GPIO_Pin_5
#define  LCD_D5         GPIO_Pin_4
#define  LCD_D4         GPIO_Pin_3
#define	 PLCD_DATA		GPIOE
#define  LCD_EN         GPIO_Pin_0
#define	 PLCD_EN		GPIOD
#define  LCD_RS         GPIO_Pin_13
#define	 PLCD_RS		GPIOC
#define  LCD_RW         GPIO_Pin_11
#define	 PLCD_RW		GPIOC

#define  LCD_DATA       (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define  LCD_IOALL      (LCD_D4|LCD_D5|LCD_D6|LCD_D7|LCD_EN|LCD_RS)

#define LCD_display_on()     LCD_writecontrol(0x0E)
#define LCD_display_off()    LCD_writecontrol(0x08)

#define LCD_cursor_blink()   LCD_writecontrol(0x0F)
#define LCD_cursor_on()      LCD_writecontrol(0x0E)
#define LCD_cursor_off()     LCD_writecontrol(0x0C)

#define LCD_cursor_left()    LCD_writecontrol(0x10)
#define LCD_cursor_right()   LCD_writecontrol(0x14)
#define LCD_display_sleft()  LCD_writecontrol(0x18)
#define LCD_display_sright() LCD_writecontrol(0x1C)

/****************Function Prototypes********************/
void	LCD_writenibble		(unsigned char data);
void	LCD_writebyte		(unsigned char data);
void	LCD_init			(void);
void	LCD_clear			(void);
void	LCD_cursor_home		(void);
void	LCD_writecontrol	(unsigned char data);
void	LCD_put				(char *c);
void	LCD_putchar			(char c);
void	LCD_putint			(unsigned int num);
void	LCD_XY				(unsigned int x, unsigned int y);
void	busyflag			(void);
