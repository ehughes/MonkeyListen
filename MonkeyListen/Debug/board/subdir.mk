################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK66FN2M0VMD18" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\startup" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\CMSIS_DSP_4_5\Sources\Include" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\source\eGFX" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\board" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\utilities" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\CMSIS" -I"C:\Users\emh203\Documents\GitHub\MonkeyListen\MonkeyListen\drivers" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


