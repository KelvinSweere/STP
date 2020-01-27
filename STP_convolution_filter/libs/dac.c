#include "main.h"
#if DA

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

#endif
