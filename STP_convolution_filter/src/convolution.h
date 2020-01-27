#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

#define F_SAMPLE 40000

volatile int  i,tel;
volatile long y;
volatile int  x[M+1];	// buffer

void TIM3_IRQHandler(void);
void ConvInterruptInit(void);
void ConvCalc(void);
void ConvPrintVal(void);
void ConvGenerateKernel(void);

#endif /* CONVOLUTION_H_ */
