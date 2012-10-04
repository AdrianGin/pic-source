################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/uCOS-VIEW/Ports/ARM-Cortex-M3/Realview/os_viewc.c 

OBJS += \
./USER/uCOS-VIEW/Ports/ARM-Cortex-M3/Realview/os_viewc.o 

C_DEPS += \
./USER/uCOS-VIEW/Ports/ARM-Cortex-M3/Realview/os_viewc.d 


# Each subdirectory must supply rules for building sources it contributes
USER/uCOS-VIEW/Ports/ARM-Cortex-M3/Realview/%.o: ../USER/uCOS-VIEW/Ports/ARM-Cortex-M3/Realview/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


