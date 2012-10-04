################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../USER/uC-LIB/Ports/RealView/lib_mem_a.s 

OBJS += \
./USER/uC-LIB/Ports/RealView/lib_mem_a.o 

S_DEPS += \
./USER/uC-LIB/Ports/RealView/lib_mem_a.d 


# Each subdirectory must supply rules for building sources it contributes
USER/uC-LIB/Ports/RealView/%.o: ../USER/uC-LIB/Ports/RealView/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Assembler'
	armasm -g --brief_diagnostics --keep --diag_style=ide --apcs=interwork --depend=$(@:%.o=%.d) -I"D:\Keil\ARM\RV31\INC" -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


