################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/WM8731/WM8731.c 

OBJS += \
./USER/WM8731/WM8731.o 

C_DEPS += \
./USER/WM8731/WM8731.d 


# Each subdirectory must supply rules for building sources it contributes
USER/WM8731/%.o: ../USER/WM8731/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


