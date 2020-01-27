/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5

    leds.c:
           LED-driver for SpARM-board v1

    pin-info:
           PB1 - RCK: Storage register clock
           PB8  - SCK: Shift register clock
           PA15 - SER: Data input

To use the LEDs use the following line to set the pins:
	LED_init();
After that, you can put an 8 bit value to the LEDs
*/

#include "main.h"
#if LEDS

void LED_init(void)
/* LED Initialize
 * This function initializes the pins
 */
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_OUT;

	gpio.GPIO_Pin = LED_SER;
	GPIO_Init(P_LED_SER, &gpio);
	gpio.GPIO_Pin = LED_SCK;
	GPIO_Init(P_LED_SCK, &gpio);
	gpio.GPIO_Pin = LED_RCK;
	GPIO_Init(P_LED_RCK, &gpio);

	GPIO_ResetBits(P_LED_SER, LED_SER);
    GPIO_ResetBits(P_LED_SCK, LED_SCK);
    GPIO_SetBits(P_LED_RCK, LED_RCK);
}

void LED_put(unsigned short led_byte)
/* LED put
 * This function puts an 8 bit value to the LEDs, where left is the LSB
 */
{
     unsigned short i;

     for(i=128; i>=1; i>>=1)
     {
          if(led_byte & i)
        	  GPIO_SetBits(P_LED_SER, LED_SER);
          else
        	  GPIO_ResetBits(P_LED_SER, LED_SER);
          GPIO_SetBits(P_LED_SCK, LED_SCK);
          GPIO_ResetBits(P_LED_SCK, LED_SCK);
     }
     GPIO_SetBits(P_LED_RCK, LED_RCK);
     GPIO_ResetBits(P_LED_RCK, LED_RCK);
}
#endif
