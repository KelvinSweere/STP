/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


    keys_int.c:
          Keyboard_interrupt-driver for ARM-board v5

    pin-info:
           PD1  - R2
           PD2  - R3
           PD3  - R4
           PD6  - K1
           PD7  - K2
           PD8  - K3
           PD9  - K4
           PD11 - R1
           PB0  - Interrupt

To use keyboard on interrupt base:
Use the following line in your main:
	KEYS_INT_init();

G_KEY is a global variable which holds the last pressed button
*/

/****************Libraries******************************/
/* Libraries needed for Keys_int are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_gpio.h"
 * #include "stm32f4xx_syscfg.h"
 * #include "misc.h"
 * #include "stm32f4xx_rcc.h"
 * #include "stm32f4xx_exti.h"
 */

/****************Defines********************************/
#define	KEY_R1			GPIO_Pin_11
#define	KEY_R2			GPIO_Pin_1
#define	KEY_R3			GPIO_Pin_2
#define	KEY_R4			GPIO_Pin_3
#define	KEY_ROW			(KEY_R1 | KEY_R2 | KEY_R3 | KEY_R4)
#define	PKEY_ROW		GPIOD
#define	KEY_K1			GPIO_Pin_6
#define	KEY_K2			GPIO_Pin_7
#define	KEY_K3			GPIO_Pin_8
#define	KEY_K4			GPIO_Pin_9
#define	KEY_COL			(KEY_K1 | KEY_K2 | KEY_K3 | KEY_K4)
#define	PKEY_COL		GPIOD
#define	KEY_INT			GPIO_Pin_0
#define	PKEY_INT		GPIOB


/****************Globals********************************/
extern int 		G_KEY;

/****************Function Prototypes********************/
void 			bepaal_deler		(void);
unsigned int	KEYS_kolom			(void);
unsigned int	KEYS_read			(void);
void			KEYS_INT_init		(void);
void			EXTI0_IRQHandler	(void);
