/*
Author: 	W Pielage & E Helmond
Date:		13-9-2013
Revision:	2

    pwm.c:
          PWM-driver for SpARM-board v1

    pin-info:
           PD12 - TIM4 Channel 1
           PD13 - TIM4 Channel 2
           PD14 - TIM4 Channel 3
           PD15 - TIM4 Channel 4

To use PWM channels,
Use the following line in your main:
	PWM_init();

*/

#include "main.h"

void PWM_init()
/* PWM Initialize
 * This function initializes the various pins for PWM
 * The PWM pins are the same as the LEDs on the STM32F4 Discovery
 */
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* GPIOE clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14; // Include other pins for more channels
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // Enable TIM4 for GPIOD 12
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); // Enable TIM4 for GPIOD 13
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4); // Enable TIM4 for GPIOD 14
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4); // Enable TIM4 for GPIOD 15

	TIM_TimeBaseStructure.TIM_Period = 20000; // Period time == 20ms
	TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock /2) / 1000000) - 1; // Prescaler has been configured so the TIM4 runs at 1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // No ClockDivision
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;


	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1	GPIOD 12 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3	GPIOD 14 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);

	TIM4->CCR1 = 1500;
//	TIM4->CCR2 = 1500;
	TIM4->CCR3 = 1600;
//	TIM4->CCR4 = 1600;
}
