################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Drivers/PWM/FTM.c \
../source/Drivers/PWM/PWM.c 

OBJS += \
./source/Drivers/PWM/FTM.o \
./source/Drivers/PWM/PWM.o 

C_DEPS += \
./source/Drivers/PWM/FTM.d \
./source/Drivers/PWM/PWM.d 


# Each subdirectory must supply rules for building sources it contributes
source/Drivers/PWM/%.o: ../source/Drivers/PWM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


