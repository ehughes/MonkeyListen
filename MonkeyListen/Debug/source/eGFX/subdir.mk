################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/eGFX/eGFX.c \
../source/eGFX/eGFX_Driver_FRDM-OLED_NHD-2.712864.c 

OBJS += \
./source/eGFX/eGFX.o \
./source/eGFX/eGFX_Driver_FRDM-OLED_NHD-2.712864.o 

C_DEPS += \
./source/eGFX/eGFX.d \
./source/eGFX/eGFX_Driver_FRDM-OLED_NHD-2.712864.d 


# Each subdirectory must supply rules for building sources it contributes
source/eGFX/%.o: ../source/eGFX/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK66FN2M0VMD18" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\startup" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\CMSIS_DSP_4_5\Sources\Include" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\source\eGFX" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\board" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\utilities" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\CMSIS" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\drivers" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


