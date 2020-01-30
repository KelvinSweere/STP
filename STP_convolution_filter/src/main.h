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

#define	TIMER 1

void timing_pin_init(void);
void MicroInit(void);

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "convolution.h"
#include "delay.h"
#include "adc.h"
#include "dac.h"
#include "uart.h"
#include "keys_int.h"
#include "lcd.h"

#endif /* MAIN_H_ */
