/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "main.h"

// Debug output defines
//#define KERNEL_FOR_EXCEL	// Om een kernel voor excel te genereren (om kernel te kunnen plotten in een diagram)
#define KERNEL_DEBUG		// Stuurt debug output naar de terminal

volatile float fc = 0.1;		//Cutoff frequentie
volatile float h_temp[M+1];		//Windowed sinc kernel array
int   h[M+1];
volatile int   deler = 130000;

int main(void)
{
	/* Configure peripheral clock for system clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 30 );

	SystemInit();			/* Set system clock to 168 MHz 									*/
	MicroInit();			/* Initialize all peripherals needed from the microcontroller	*/

	ConvGenerateKernel();
	ConvPrintVal();

	ConvInterruptInit();	/* Initialize and start convolution timer interrupt				*/
	timing_pin_init();

//	UART_printf(256, "Init's zijn succesvol uitgevoerd \r\n\r\n\r\n\r\n\r\n");
//	DELAY_ms(1000);

	while(1)
	{

	}
}

void timing_pin_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Initialize the pins. We use the GPIO library
	GPIO_InitTypeDef GPIO_InitStructure; // Initstructure for GPIO settings
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // No pull-up or down
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // Output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;

	GPIO_Init(GPIOD, &GPIO_InitStructure); // Init GPIO's from port D
}

void MicroInit(void)
{
	DELAY_init();
	UART_init();
	KEYS_INT_init();
	LCD_init();
	DAC_init(Channel_1);
	ADC_init(Channel_1);
}
