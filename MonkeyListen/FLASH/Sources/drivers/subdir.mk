################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/drivers/frdm-k20-def.c" \

C_SRCS += \
../Sources/drivers/frdm-k20-def.c \

OBJS += \
./Sources/drivers/frdm-k20-def.o \

C_DEPS += \
./Sources/drivers/frdm-k20-def.d \

OBJS_QUOTED += \
"./Sources/drivers/frdm-k20-def.o" \

C_DEPS_QUOTED += \
"./Sources/drivers/frdm-k20-def.d" \

OBJS_OS_FORMAT += \
./Sources/drivers/frdm-k20-def.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/drivers/frdm-k20-def.o: ../Sources/drivers/frdm-k20-def.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/drivers/frdm-k20-def.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/drivers/frdm-k20-def.o"
	@echo 'Finished building: $<'
	@echo ' '


