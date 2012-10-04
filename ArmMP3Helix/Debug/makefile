################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include USER/ucCos_App/subdir.mk
-include USER/helix/real/arm/subdir.mk
-include USER/helix/real/subdir.mk
-include USER/helix/subdir.mk
-include USER/WM8731/subdir.mk
-include USER/Unicode/subdir.mk
-include USER/USB/subdir.mk
-include USER/TouchPanel/subdir.mk
-include USER/SPI_Flash/subdir.mk
-include USER/SD_Card/subdir.mk
-include USER/MicrochipGUI_V2.10/src/subdir.mk
-include USER/MicrochipGUI_V2.10/Image/subdir.mk
-include USER/MicrochipGUI_V2.10/GUI_App/subdir.mk
-include USER/MicrochipGUI_V2.10/Font/subdir.mk
-include USER/MicrochipGUI_V2.10/subdir.mk
-include USER/MP3Header/subdir.mk
-include USER/GLCD/subdir.mk
-include USER/FreeRTOS/Source/portable/RVDS/ARM_CM3/subdir.mk
-include USER/FreeRTOS/Source/portable/MemMang/subdir.mk
-include USER/FreeRTOS/Source/subdir.mk
-include USER/FATFS_V0.08A/src/subdir.mk
-include USER/Audio/subdir.mk
-include USER/subdir.mk
-include Libraries/STM32_USB-FS-Device_Driver/src/subdir.mk
-include Libraries/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/subdir.mk
-include Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/subdir.mk
-include Libraries/CMSIS/CM3/CoreSupport/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: ArmMP3Helix.axf

# Tool invocations
ArmMP3Helix.axf: $(OBJS) M:\Robotics\Source\ArmMP3Helix\Project.sct $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM RealView Linker'
	armlink --strict --verbose --callgraph --map --symbols --xref --diag_style=ide --list="M:\Robotics\Source\ArmMP3Helix\Project.map" --scatter=M:\Robotics\Source\ArmMP3Helix\Project.sct --libpath="D:\Keil\ARM\RV31\Lib" --library_type=microlib --debug --info sizes,totals,unused --cpu Cortex-M3 -o "ArmMP3Helix.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(S_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) ArmMP3Helix.axf
	-@echo ' '

pre-build:
	-make -v
	-@echo ' '

post-build:
	-fromelf.exe --i32 --output ArmMP3Helix.hex ArmMP3Helix.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
