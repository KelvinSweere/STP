################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/adc.c \
../libs/dac.c \
../libs/delay.c \
../libs/keys.c \
../libs/keys_int.c \
../libs/lcd.c \
../libs/leds.c \
../libs/pwm.c \
../libs/uart.c 

OBJS += \
./libs/adc.o \
./libs/dac.o \
./libs/delay.o \
./libs/keys.o \
./libs/keys_int.o \
./libs/lcd.o \
./libs/leds.o \
./libs/pwm.o \
./libs/uart.o 

C_DEPS += \
./libs/adc.d \
./libs/dac.d \
./libs/delay.d \
./libs/keys.d \
./libs/keys_int.d \
./libs/lcd.d \
./libs/leds.d \
./libs/pwm.d \
./libs/uart.d 


# Each subdirectory must supply rules for building sources it contributes
libs/%.o: ../libs/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F407VGTx -DSTM32F407G_DISC1 -DDEBUG -DSTM32F40XX -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -I"C:/Users/Neki_/Documents/School Git/STP/STP_convolution_filter/StdPeriph_Driver/inc" -I"C:/Users/Neki_/Documents/School Git/STP/STP_convolution_filter/inc" -I"C:/Users/Neki_/Documents/School Git/STP/STP_convolution_filter/CMSIS/device" -I"C:/Users/Neki_/Documents/School Git/STP/STP_convolution_filter/CMSIS/core" -I"C:/Users/Neki_/Documents/School Git/STP/STP_convolution_filter/src" -I"C:/Users/Neki_/Documents/School Git/STP/STP_convolution_filter/libs" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


