/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


    adc.c:
          ADC driver for SpARM-board v1

    pin-info:
           PA4 - DAC Channel 1
           PA5 - DAC Channel 2


To use the DAC channel 1 or 2 use the following initialize:
	ADC_init(Channel_1);
	ADC_init(Channel_2);
*/

/****************Libraries******************************/
/* Libraries needed for ADC are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_conf.h"
 * #include "stm32f4xx_gpio.h"
 * #include "stm32f4xx_rcc.h"
 * #include "stm32f4xx_adc.h"
 */

/****************Defines********************************/
#define Channel_1	1
#define Channel_2	2

/****************Function Prototypes********************/
void 		ADC_init		(int Channel);
uint16_t 	Get_ADC_Value	(int Channel);
