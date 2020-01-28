#include "main.h"

/**
 * @brief initaliseer de timer interupt voor convolutie.
 */
void ConvInterruptInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 					 = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* Time base configuration (42 MHz TIM clock frequency) */
	TIM_TimeBaseStructure.TIM_Prescaler 	= 1;
	TIM_TimeBaseStructure.TIM_Period		= 42000000 / F_SAMPLE;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);			/* TIM IT enable 		*/
	TIM_Cmd(TIM3, ENABLE);								/* TIM3 enable counter 	*/
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		ConvCalc();
	}
}

/**
 * @brief bereken de convolutie en stuur deze uit naar een DAC.
 */
void ConvCalc(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_12); // Zet pin hoog als indicatie wanneer convolutie begint
	y = 0;

	// ! TODO: checken of ook een uint16_t datatype gebruikt kan worden.

	/* Shifting of all values in the buffer and convolute (M-1) multiplications */
	for(i=0; i<M; i++)
	{
		x[M-i-1] = x[M-i];		/* Shift values				*/
		y += h[M-i] * x[i];		/* Multiply and accumulate	*/
	}

	x[M] = Get_ADC_Value(1);	/* Take a new sample						*/
	y += h[0] * x[M];			/* Multiply and accumulate with new value	*/

	y /= deler;								/* Use a divider to bring y back to a range between 0 en 4095 for the DAC	*/
	DAC_SetChannel1Data(DAC_Align_12b_R, y);	/* Send the y-value to the DAC												*/
	//UART_printf(256, "%d \r\n", y);

	GPIO_ResetBits(GPIOD, GPIO_Pin_12); // Zet pin hoog als indicatie wanneer convolutie klaar is
}

/**
 * @brief print de cutoff frequentie van de kantelfrequentie op het LCD scherm.
 */
void ConvPrintVal(void)
{
	LCD_clear();

	/* Print cuttof frequency fc */
	LCD_put("fc = ");

	if(fc < 0.1)	LCD_put("0.0");
	else			LCD_put("0.");

	LCD_putint((int)(fc * 10000));

	/* Print fc in Hz */
	LCD_XY(0,1);
	LCD_put("fc = ");
	LCD_putint((int)(fc * F_SAMPLE));
	LCD_put(" Hz");
}

/**
 * @brief Genereer een kernel waarop de convolutie op wordt uitgevoerd.
 */
void ConvGenerateKernel(void)
{
	int   i = 0;
	float K, K_temp = 0;		/* K is a gain value that decides the attenuation	*/
	float blackman[M+1];		/* blackman window array							*/
	float sinc[M+1];			/* Sinc window array								*/

	/* Clear all arrays */
	for(i=0; i<(M+1); i++)
	{
		blackman[i] = 0;
		sinc[i] = 0;
		h[i] = 0;
	}

	/* Calculation of the kernel */
	for(i=0; i<(M+1); i++)
	{
		/* Calculate the next point in the blackman window */
		blackman[i] = 0.42 - (0.5 * cos((2*M_PI*i) / M)) + (0.08 * cos((4*M_PI*i) / M));

		/* Check if the calculation loop has reached the middle of the kernel.
		 * When the middle of the loop is reached the divider in the sinc array becomes 0:
		 * i - (M/2) = 0. A division by zero is not possible. That's why sinc[i] and h_temp[i] are made 0 */
		if(i == (M/2))
		{
			sinc[i]   = 0;
			h_temp[i] = 0;
		}
		else
		{
			sinc[i]   = sin((2*M_PI*fc) * (i-(M/2))) / (i-(M/2));	/* Calculate point in the sinc array	*/
			h_temp[i] = sinc[i] * blackman[i];						/* Calculate point in the kernel array	*/
		}

		//UART_printf(256, "blackman[%d] = %f \t sinc[%d] = %f \t h_temp[%d] = %f \t i-(M/2) = %d \r\n", i, blackman[i], i, sinc[i], i, h_temp[i], (i-(M/2)));

		/* Excel outputs */
		//UART_printf(256, "%f \r\n", blackman[i]);
		//UART_printf(256, "%f \r\n", sinc[i]);
		//UART_printf(256, "%f \r\n", h_temp[i]);
	}

	/* Calculate the final gain (this will decide the level of attenuation) */
	for(i=0; i<(M+1); i++)
		K_temp = K_temp + h_temp[i];

	K = 1 / K_temp;
	//UART_printf(256, "K gain: %f \r\n", K);

	/* Multiply the gain with the temporary kernel h_temp to get the final kernel values
	 * The H_MULTIPLIER is used to turn the float values into integers						*/
	for(i=0; i<(M+1); i++)
	{
		if(i == M/2)
			h_temp[i] = (K * 2 * M_PI * fc) * H_MULTIPLIER;		/* Change the middle point of the kernel so it's not 0 */
		else
			h_temp[i] = (K * h_temp[i]) * H_MULTIPLIER;			/* Multiply by the gain */

		h[i] = (int)(h_temp[i]);						/* Change float to int and apply division */


		/* Kernel calculation debugging */
		#ifdef KERNEL_DEBUG
		UART_printf(256, "h[%d] = %d \r\n", i, h[i]);
		#endif

		#ifdef KERNEL_FOR_EXCEL
			UART_printf(256, "%f \r\n", h_temp[i]);
		#endif


	}

}
