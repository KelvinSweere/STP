/*
 * main.h
 *
 *  Created on: 27 jan. 2020
 *      Author: Neki_
*   Version:
*   	1.00:
*   		#defines van de code samengevoegd in main.h en convolution.h
*   	1.01:
*   		Defines die gaan over convolutie toegevoegd aan convolution.h.
 */

#ifndef MAIN_H_
#define MAIN_H_

#define		AD			1
#define		DA			1
#define		BUZZER		1
#define		LEDS		1
#define 	UART		1
#define		PWM			1
#define		LCD			1
#define		DELAY		1
#define		KEYS_INT	1


// Zodat het in keys_int.c gebruikt kan worden
extern int h[];
extern volatile float fc;
extern volatile float h_temp[];
extern volatile int deler;

// Zodat het nieuwe kernel berekend kan worden in keys_int.c
void generate_kernel(void);
void timing_pin_init(void);
void MicroInit(void);

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_pwr.h"
#include "misc.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "convolution.h"

#if AD
#define	TIMER 1
#include "stm32f4xx_adc.h"
#include "adc.h"
#endif

#if DA
#define	TIMER 1
#include "stm32f4xx_dac.h"
#include "dac.h"
#endif

#if UART
#include "stm32f4xx_usart.h"
#include "uart.h"
#endif

#if KEYS_INT
#define	EXTINT	1
#include "keys_int.h"
#endif


#if LEDS
#include "leds.h"
#endif

#if LCD
#include "lcd.h"
#endif

#if DELAY
#include "delay.h"
#endif

#if PWM
#define	TIMER 1
#include "pwm.h"
#endif

#if EXTINT
#include "stm32f4xx_exti.h"
#endif

#if TIMER
#include "stm32f4xx_tim.h"
#endif



#define		LED1		GPIOD, GPIO_Pin_12
#define		LED2		GPIOD, GPIO_Pin_13
#define		LED3		GPIOD, GPIO_Pin_14
#define		LED4		GPIOD, GPIO_Pin_15

#define		TOGGLE		2
#define		ON			1
#define 	OFF			0

#endif /* MAIN_H_ */
