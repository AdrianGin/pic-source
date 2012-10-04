################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_cpu_c.c \
../USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_dbg.c 

S_SRCS += \
../USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_cpu_a.s 

OBJS += \
./USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_cpu_a.o \
./USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_cpu_c.o \
./USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_dbg.o 

C_DEPS += \
./USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_cpu_c.d \
./USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_dbg.d 

S_DEPS += \
./USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/os_cpu_a.d 


# Each subdirectory must supply rules for building sources it contributes
USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/%.o: ../USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Assembler'
	armasm -g --brief_diagnostics --keep --diag_style=ide --apcs=interwork --depend=$(@:%.o=%.d) -I"D:\Keil\ARM\RV31\INC" -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '

USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/%.o: ../USER/uCOS-II/Ports/ARM-Cortex-M3/RealView/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


