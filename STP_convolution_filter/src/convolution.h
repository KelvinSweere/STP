#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

// Debug output defines
//#define KERNEL_FOR_EXCEL	// Om een kernel voor excel te genereren (om kernel te kunnen plotten in een diagram)
//#define KERNEL_DEBUG		// Stuurt debug output naar de terminal

#define F_SAMPLE 40000

volatile int  i,tel;
volatile int  y;
volatile int  x[M+1];	// buffer

void TIM3_IRQHandler(void);
void ConvInterruptInit(void);
void ConvCalc(void);
void ConvPrintVal(void);
void ConvGenerateKernel(void);

#endif /* CONVOLUTION_H_ */
