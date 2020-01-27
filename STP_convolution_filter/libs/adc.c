/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		2-10-2016
Revision:	2

    adc.c:
          ADC driver for ARM-board v5

    pin-info:
           PA4 - DAC Channel 1
           PA5 - DAC Channel 2


To use the DAC channel 1 or 2 use the following initialize:
	ADC_init(Channel_1);
	ADC_init(Channel_2);
*/

#include "main.h"
#if AD
void ADC_init(int Channel)
/* ADC Initialize
 * Use this function to initialize the ADC channel 1 or 2.
 * The input of the ADC channel 1 is on pin PC.1.
 * The input of the ADC channel 2 is on pin PC.2.
 * The DAC can be loaded with:
 * Data = Get_ADC_Value(Channel_1);
 * Data = Get_ADC_Value(Channel_2);
 * Data is an 16 bit integer with the 12 bit ADC value
 */
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	ADC_InitTypeDef 		ADC_InitStructure;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

	/* Configure ADC Channel pin as analog input */
	if (Channel == Channel_1)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* ADC Common configuration *************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 regular channel 10 to 15 configuration ******************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	if (Channel == Channel_1)
	{
		ADC_Init(ADC2, &ADC_InitStructure);
		ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);
		/* Enable ADC2*/
		ADC_Cmd(ADC2, ENABLE);
	}
	else
	{
		ADC_Init(ADC3, &ADC_InitStructure);
		ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
		/* Enable ADC3*/
		ADC_Cmd(ADC3, ENABLE);
	}
}

uint16_t Get_ADC_Value(int Channel)
/* ADC Get Value
 * Use this function fetch the value of ADC channel 1 or 2.
 * Return is an 16 bit integer with the 12 bit ADC value
 * Shifted 4 spaces to get the right value
 * (Left aligned LSB filled with zeros)
 */
{
	if (Channel == 1)
	{
		ADC_SoftwareStartConv(ADC2);
		while(ADC_GetSoftwareStartConvStatus(ADC2) != RESET);
		return (ADC_GetConversionValue(ADC2) >> 4);
	}
	else
	{
		ADC_SoftwareStartConv(ADC3);
		while(ADC_GetSoftwareStartConvStatus(ADC3) != RESET);
		return (ADC_GetConversionValue(ADC3) >> 4);
	}
}
#endif
