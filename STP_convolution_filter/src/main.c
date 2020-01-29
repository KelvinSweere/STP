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

extern uint32_t G_CLK;

volatile float fc = 0.075;		//Cutoff frequentie
volatile float h_temp[M+1];		//Windowed sinc kernel array
int   h[M+1];

int main(void)
{
	/* Configure peripheral clock for system clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 30 );

	static int set = 1;		/* reset identifier																*/
	SystemInit();			/* Set system clock to 168 MHz 													*/
	set++;					/* if this value is > 1 you have made a serious pointer error, reset happened	*/

	MicroInit();			/* Initialize all peripherals needed from the microcontroller	*/

	timing_pin_init();
	ConvGenerateKernel();
	ConvPrintVal();

	UART_printf(256, "\r\n\r\nMicrocontroller init succecful! \r\n");
	UART_printf(256, "Running on: %d MHz! \r\n\r\n", G_CLK/1000000);


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
