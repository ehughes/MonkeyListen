################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 13
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Sources/drivers/mcg/subdir.mk
-include Sources/drivers/subdir.mk
-include Sources/drivers/adc16/subdir.mk
-include Sources/cpu/subdir.mk
-include Sources/GFX/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \

EXECUTABLES_QUOTED += \

EXECUTABLES_OS_FORMAT += \


# All Target
all: warning-messages explicit-dependencies MonkeyListen.elf secondary-outputs

dependents:
	-cd "$(CMSIS_DSP_3_2)/Debug" && $(MAKE) all

# Tool invocations
MonkeyListen.elf: $(OBJS) $(USER_OBJS) $(CMSIS_DSP_3_2)/Debug/libCMSIS_DSP_3.2.a ../Project_Settings/Linker_Files/MK20DX128_flash.ld
	@echo 'Building target: $@'
	@echo 'Executing target #14 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc"  $(USER_OBJS_QUOTED)  @"MonkeyListen.args" -o"MonkeyListen.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) "./*/*/*.o" "./*/*/*/*.o" "./*/*.o"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*/*/*.d" "./*/*.d"  $(ProjDirPath)/FLASH/MonkeyListen.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:
$(CMSIS_DSP_3_2)/Debug/libCMSIS_DSP_3.2.a:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/MK20DX128_flash.ld 

warning-messages: 

