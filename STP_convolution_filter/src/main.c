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
#include "../libs/delay.h"

// Debug output defines
//#define KERNEL_FOR_EXCEL	// Om een kernel voor excel te genereren (om kernel te kunnen plotten in een diagram)
#define KERNEL_DEBUG		// Stuurt debug output naar de terminal

volatile float fc = 0.2;		//Cutoff frequentie
volatile float h_temp[M+1];		//Windowed sinc kernel array
int   h[M+1];
volatile int   deler = 130000;

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 30 );

	static int set=1;		// reset identifier
	SystemInit();			// Set SystemCLK
	set++;					// if this value is > 1 you have made a serious pointer error, reset happened

	DELAY_init();			// Delay intit
	UART_init();			// Voor printf

	KEYS_INT_init();		// Knoppen init
	LCD_init();
	DAC_init(Channel_1);	// DAC init voor channel 1
	ADC_init(Channel_1);	// ADC init voor channel 1

	generate_kernel();

	LCD_clear();
	LCD_put("fc = ");

	//Print fc van 0 tot 0.5
	LCD_clear();
	LCD_put("fc = ");

	if(fc < 0.1)
		LCD_put("0.0");
	else
		LCD_put("0.");

	LCD_putint(((int)(fc*10000)));

	//Print fc in Hz
	LCD_XY(0,1);
	LCD_put("fc = ");
	LCD_putint(((int)(fc*fs)));
	LCD_put(" Hz");


	conv_INT_init();		// Interrupt voor de convolutie
	timing_pin_init();

	UART_printf(256, "Init's zijn succesvol uitgevoerd \r\n\r\n\r\n\r\n\r\n");
	DELAY_ms(1000);

	while(1);
}

void generate_kernel(void)
{
	int i = 0;										//teller

	float pi = 3.14159265359;	//pi
	float K, K_temp = 0;							//K en een tijdige K voor berekeningen
	float blackman[M+1];						//blackman window array
	float sinc[M+1];							//Sinc window array

	for(i=0; i<(M+1); i++)
	{
		blackman[i] = 0;
		sinc[i] = 0;
		h[i] = 0;
	}

	for(i=0; i<(M+1); i++)
	{
		blackman[i] = 0.42 - (0.5 * cos( (2*pi*i) / M) ) + ( 0.08 * cos( (4*pi*i) / M ));	//Bereken het volgende punt in de blackman window

		//Check of de loop bij M/2 zit.
		if(i == M/2)
		{
			sinc[i] = 0;	//Kan niet delen door 0, dus maak 0
			h_temp[i] = 0;
		}
		else
		{
			sinc[i]   = sin( (2*pi*fc) * (i-(M/2)) ) / (i-(M/2));	//Bereken punt in sinc array
			h_temp[i] = sinc[i] * blackman[i];						//Bereken kernel in kernel array
		}
	}

	//Bereken K
	for(i=0; i<(M+1); i++)
		K_temp = K_temp + h_temp[i];
	K = 1 / K_temp;

	//Vermenigvuldig de kernel met K
	for(i=0; i<(M+1); i++)
	{
		if(i == M/2)
			h_temp[i] = 2*pi*fc*K * H_MULTIPLIER;		//Verander het punt op M/2, zodat het punt niet 0 is
		else
			h_temp[i] = K * h_temp[i] * H_MULTIPLIER;		//Vermenigvuldig met k

		h[i] = (int)h_temp[i];	// Zet float om naar int

		// Debuggen van het kernel
		#ifdef KERNEL_DEBUG
		UART_printf(256, "h[%d] = %d \r\n", i, h[i]);
		#endif

		#ifdef KERNEL_FOR_EXCEL
			UART_printf(256, "%d \r\n", h[i]);
		#endif


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
