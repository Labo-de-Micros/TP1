################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App_display.c \
../source/Display_7.c \
../source/SysTick.c \
../source/button.c \
../source/encoder.c \
../source/gpio.c \
../source/timer.c 

OBJS += \
./source/App_display.o \
./source/Display_7.o \
./source/SysTick.o \
./source/button.o \
./source/encoder.o \
./source/gpio.o \
./source/timer.o 

C_DEPS += \
./source/App_display.d \
./source/Display_7.d \
./source/SysTick.d \
./source/button.d \
./source/encoder.d \
./source/gpio.d \
./source/timer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


