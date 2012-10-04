################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/main.c \
../USER/stm32f10x_it.c \
../USER/system_stm32f10x.c 

OBJS += \
./USER/main.o \
./USER/stm32f10x_it.o \
./USER/system_stm32f10x.o 

C_DEPS += \
./USER/main.d \
./USER/stm32f10x_it.d \
./USER/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
USER/%.o: ../USER/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


