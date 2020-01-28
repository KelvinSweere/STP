/*
 * Version:
 * 		1.01:
 * 			Shiften van waarden in de x-buffer gefixt.
 */


#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

// Debug output defines
//#define KERNEL_FOR_EXCEL	// Om een kernel voor excel te genereren (om kernel te kunnen plotten in een diagram)
//#define KERNEL_DEBUG		// Stuurt debug output naar de terminal

#define F_SAMPLE 40000
// DEFINES DIE WE NODIG HEBBEN
// 64 Tabs is getest op 28-1-2020, de tijd die convolutie erover doet is 24,9 us.
#define M 			 63			//grootte van buffer
#define H_MULTIPLIER 1024		//Getal waarmee kernel in floats wordt gescaled naar een kernel met ints

volatile int  i,tel;
volatile int  y;
volatile int  x[M+1];	// buffer

extern int h[];
extern volatile float fc;
extern volatile float h_temp[];
extern volatile int deler;

void TIM3_IRQHandler(void);
void ConvInterruptInit(void);
void ConvCalc(void);
void ConvPrintVal(void);
void ConvGenerateKernel(void);

#endif /* CONVOLUTION_H_ */
