################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/GFX/GFX.c" \
"../Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.c" \
"../Sources/GFX/GFX_FONT_10x14.c" \
"../Sources/GFX/GFX_FONT_3x5.c" \
"../Sources/GFX/GFX_FONT_5x7.c" \

C_SRCS += \
../Sources/GFX/GFX.c \
../Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.c \
../Sources/GFX/GFX_FONT_10x14.c \
../Sources/GFX/GFX_FONT_3x5.c \
../Sources/GFX/GFX_FONT_5x7.c \

OBJS += \
./Sources/GFX/GFX.o \
./Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.o \
./Sources/GFX/GFX_FONT_10x14.o \
./Sources/GFX/GFX_FONT_3x5.o \
./Sources/GFX/GFX_FONT_5x7.o \

C_DEPS += \
./Sources/GFX/GFX.d \
./Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.d \
./Sources/GFX/GFX_FONT_10x14.d \
./Sources/GFX/GFX_FONT_3x5.d \
./Sources/GFX/GFX_FONT_5x7.d \

OBJS_QUOTED += \
"./Sources/GFX/GFX.o" \
"./Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.o" \
"./Sources/GFX/GFX_FONT_10x14.o" \
"./Sources/GFX/GFX_FONT_3x5.o" \
"./Sources/GFX/GFX_FONT_5x7.o" \

C_DEPS_QUOTED += \
"./Sources/GFX/GFX.d" \
"./Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.d" \
"./Sources/GFX/GFX_FONT_10x14.d" \
"./Sources/GFX/GFX_FONT_3x5.d" \
"./Sources/GFX/GFX_FONT_5x7.d" \

OBJS_OS_FORMAT += \
./Sources/GFX/GFX.o \
./Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.o \
./Sources/GFX/GFX_FONT_10x14.o \
./Sources/GFX/GFX_FONT_3x5.o \
./Sources/GFX/GFX_FONT_5x7.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/GFX/GFX.o: ../Sources/GFX/GFX.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GFX/GFX.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GFX/GFX.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.o: ../Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GFX/GFX_Driver_FRDM-OLED_NHD-2.712864.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GFX/GFX_FONT_10x14.o: ../Sources/GFX/GFX_FONT_10x14.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GFX/GFX_FONT_10x14.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GFX/GFX_FONT_10x14.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GFX/GFX_FONT_3x5.o: ../Sources/GFX/GFX_FONT_3x5.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GFX/GFX_FONT_3x5.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GFX/GFX_FONT_3x5.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GFX/GFX_FONT_5x7.o: ../Sources/GFX/GFX_FONT_5x7.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GFX/GFX_FONT_5x7.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GFX/GFX_FONT_5x7.o"
	@echo 'Finished building: $<'
	@echo ' '


