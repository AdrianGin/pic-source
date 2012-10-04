################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/uC-LIB/lib_mem.c \
../USER/uC-LIB/lib_str.c 

OBJS += \
./USER/uC-LIB/lib_mem.o \
./USER/uC-LIB/lib_str.o 

C_DEPS += \
./USER/uC-LIB/lib_mem.d \
./USER/uC-LIB/lib_str.d 


# Each subdirectory must supply rules for building sources it contributes
USER/uC-LIB/%.o: ../USER/uC-LIB/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


