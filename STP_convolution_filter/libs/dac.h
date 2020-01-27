/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5

    dac.c:
          DAC driver with or without interrupts for SpARM-board v1

    pin-info:
           PA4 - DAC Channel 1
           PA5 - DAC Channel 2


To use the DAC channel 1 or 2 use the following initialize:
	DAC_init(Channel_1);
	DAC_init(Channel_2);
To use a timer interrupt (TIM3) use:
	DAC_INT_init();
The interrupt handler called is TIM3_IRQHandler
*/

/****************Libraries******************************/
/* Libraries needed for DAC are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_conf.h"
 * #include "stm32f4xx_gpio.h"
 * #include "stm32f4xx_rcc.h"
 * #include "stm32f4xx_dac.h"
 */

/****************Defines********************************/
#define Channel_1	1
#define Channel_2	2

//extern volatile y,i,tel;
//extern volatile int x[];	// buffer

/****************Function Prototypes********************/
extern volatile int x[];	// buffer

void DAC_init(int Channel);
void conv_INT_init(void);
void TIM3_IRQHandler(void);
void conv(void);
