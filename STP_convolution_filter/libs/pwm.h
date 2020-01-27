/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


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

/****************Libraries******************************/
/* Libraries needed for PWM are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_gpio.h"
 * #include "stm32f4xx_syscfg.h"
 * #include "stm32f4xx_rcc.h"
 * #include "stm32f4xx_tim.h"
 */

/****************Function Prototypes********************/
void	PWM_init	(void);
