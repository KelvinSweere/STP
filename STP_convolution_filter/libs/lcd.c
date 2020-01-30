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

#include "main.h"



/****************Globals********************************/
unsigned char curpos = 0; // remember cursorposition

void ClearBits(void)
/* Clear bits
 * Clear all data bits
 */
{
	GPIO_ResetBits(PLCD_DATA, LCD_DATA);
}

void LCD_cursor_home(void)
/* LCD Cursor home
 * Reset the cursor position to position 0
 */
{
   LCD_writecontrol(0x02); // cursor home
   curpos=0;               // reset position
}

void LCD_clear(void)
/* LCD clear screen
 * Clear the whole LCD and resets cursor position
 */
{
   LCD_writecontrol(0x01); // clearscreen
   curpos=0;               // reset position
   DELAY_ms(5);
   busyflag();

}

void LCD_XY(unsigned int x, unsigned int y)
/* LCD X,Y
 * This function puts the cursor on position x,y
 * Y is the line the cursor must be placed
 */
{
    if ( y==0 )
    	LCD_writecontrol((1<<7)+0x00+x);
    else if ( y==1)
    	LCD_writecontrol((1<<7)+0x40+x);
    else if ( y==2)
    	LCD_writecontrol((1<<7)+0x14+x);
    else /* y==3 */
    	LCD_writecontrol((1<<7)+0x54+x);
}

void LCD_init(void)
/* LCD initialize
 * Display is used in 4 bits modus, 2 line, 5x7 dots font.
 * This function sets the pins in the correct mode and initializes the display
 */
{
	GPIO_InitTypeDef gpio;	// GPIO init structure
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // Enable Clock for PortE
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Pin = LCD_DATA;
	GPIO_Init(PLCD_DATA, &gpio);

	gpio.GPIO_Pin = LCD_EN;
	GPIO_Init(PLCD_EN, &gpio);

	gpio.GPIO_Pin = LCD_RW;
	GPIO_Init(PLCD_RW, &gpio);

	gpio.GPIO_Pin = LCD_RS;
	GPIO_Init(PLCD_RS, &gpio);

	GPIO_ResetBits(PLCD_RS, LCD_RS);
	GPIO_ResetBits(PLCD_EN, LCD_EN);
	GPIO_ResetBits(PLCD_RW, LCD_RW);

	DELAY_ms(15);

    LCD_writebyte(0x30); // function-set boot
    DELAY_ms(50);
    LCD_writebyte(0x30);
    LCD_writebyte(0x30);

     // interface op 4 bits
    LCD_writebyte(0x02);
    DELAY_ms(5);

    LCD_writebyte(0x2C);  // function set
    DELAY_ms(15);
    LCD_writebyte(0x0E);  // lcd aan
    DELAY_ms(15);
    LCD_writebyte(0x01);  // clear screen
    DELAY_ms(15);
    LCD_writebyte(0x06);  // entry mode set
    DELAY_ms(15);
}

void LCD_putchar(char c)
/* LCD Put character
 * This function puts the given character on the display on the last cursor position
 */
{
    GPIO_SetBits(PLCD_RS, LCD_RS);
    LCD_writebyte(c);
    if (++curpos==32) // remember cursorpos
    	curpos=0;
}

void LCD_put(char *string)
/* LCD Put
 * LCD Put puts the given string on the display
 */
{
    unsigned char k;
	// If you use the 1 line display, there is no character 8 to 31.
    // You have to jump to 0,32 to continue writing the display
    for (k=0; string[k]; k++)
	{
        if(curpos==32) // at 16, goto next valid lcd-position
            LCD_XY(0,32);

        LCD_putchar(string[k]);
    }
}

void LCD_putint(unsigned int num)
/* LCD Put integer
 * This function puts an positive integer on the screen
 */
{
    unsigned int rest;
    static unsigned char chars[10] = "0123456789"; // possible chars
    char s[17]; // display has 16 chars [0..15] + 1 closing null
	s[16]='\0'; // closing null set
    int i=15;   // startposition at last display char

    // Integer to ascii (simplified)
    if(num==0)
        s[i--]='0'; // '0' at 14
    else
	{
        while(num>0 && i)
		{
            rest = num%10;
            num /= 10;
            s[i--]=chars[rest];
        }
    }
	LCD_put(&s[i+1]);
}

void LCD_writenibble(unsigned char data)
/* LCD Write nibble
 * Writes 4 bits value to the display
 */
{
    ClearBits();

    if (data & 0x01) {
		GPIO_SetBits(PLCD_DATA, LCD_D7);
	}
	if (data & 0x02) {
		GPIO_SetBits(PLCD_DATA, LCD_D6);
	}
	if (data & 0x04) {
		GPIO_SetBits(PLCD_DATA, LCD_D5);
	}
	if (data & 0x08) {
		GPIO_SetBits(PLCD_DATA, LCD_D4);
	}
}

void LCD_writebyte(unsigned char data)
/* LCD Write byte
 * Writes 8 bits value to the display
 */
{
    /* 4 MSB */
    GPIO_SetBits(PLCD_EN, LCD_EN);
    LCD_writenibble((data>>4)&0x0F);
    GPIO_ResetBits(PLCD_EN, LCD_EN);

    busyflag();

    /* 4 LSB */
    GPIO_SetBits(PLCD_EN, LCD_EN);
    LCD_writenibble(data&0x0F);
    GPIO_ResetBits(PLCD_EN, LCD_EN);

    busyflag();
}

void LCD_writecontrol(unsigned char data)
/* LCD Write control
 * Write a commando to the screen
 */
{
    GPIO_ResetBits(PLCD_RS, LCD_RS);
    LCD_writebyte(data);
}

void busyflag(void)
/* Wait for busyflag
 * If something is written to the display, D7 function will change to the busy flag
 * This function makes an input from the data bits and wait for the flag to go low.
 */
{
	uint8_t bitstatus = 0x00;
	uint8_t bitstatus1 = Bit_SET;
	GPIO_InitTypeDef gpio;

	while(bitstatus == bitstatus1)
	{
		gpio.GPIO_Pin = LCD_DATA;
		gpio.GPIO_Mode = GPIO_Mode_IN;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		gpio.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(PLCD_DATA, &gpio);

		GPIO_ResetBits(PLCD_RS, LCD_RS);
		GPIO_SetBits(PLCD_RW, LCD_RW);

		DELAY_ms(1);
		GPIO_SetBits(PLCD_EN, LCD_EN);
		DELAY_ms(1);

		bitstatus = GPIO_ReadInputDataBit(PLCD_DATA, LCD_D7);
		GPIO_ResetBits(PLCD_EN, LCD_EN);
		DELAY_ms(1);
	}

	gpio.GPIO_Pin = LCD_DATA;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PLCD_DATA, &gpio);

	DELAY_ms(1);
}
