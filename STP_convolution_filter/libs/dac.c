#include "main.h"
#if DA

uint32_t Data = 0;
uint32_t Data1 = 0;
uint32_t Data2 = 250;

uint32_t Data;

void DAC_init(int Channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	if(Channel == Channel_1)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger 		 = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer   = DAC_OutputBuffer_Enable;

	if( Channel == Channel_1)
	{
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
		DAC_Cmd(DAC_Channel_1, ENABLE);
	}
	else
	{
		DAC_Init(DAC_Channel_2, &DAC_InitStructure);
		DAC_Cmd(DAC_Channel_2, ENABLE);
	}
}

void conv_INT_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Time base configuration */

	TIM_TimeBaseStructure.TIM_Period = 42000000/fs;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

volatile int i,tel;
volatile long y;
volatile int x[M+1];	// buffer

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		conv();
	}
}

void conv()
{
	//GPIO_SetBits(GPIOD, GPIO_Pin_12); // Zet pin hoog als indicatie wanneer convolutie begint
	y = 0;

	for(i=0; i<M; i++)
	{
		x[M-i-1]  = x[M-i];			//Verschuif alle waarden in de buffer
		y        += (long)(h[M-i] * x[i]);	//Voer M-1 convoluties uit
	}

	x[M] = Get_ADC_Value(1);	//Neem een sample
	y += (long)(h[0] * x[M]);			//Bereken het allerlaatste punt

	y = y / deler;								//Breng y terug naar een range tussen 0 en 4095 voor de DAC
	DAC_SetChannel1Data(DAC_Align_12b_R, y);	//Stuur de output naar de DAC
	//UART_printf(256, "%d \r\n", y);

	//GPIO_ResetBits(GPIOD, GPIO_Pin_12); // Zet pin hoog als indicatie wanneer convolutie klaar is
}

#endif
