################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/startup_stm32f10x_hd.s 

OBJS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/startup_stm32f10x_hd.o 

S_DEPS += \
./Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/startup_stm32f10x_hd.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/%.o: ../Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Assembler'
	armasm -g --brief_diagnostics --keep --diag_style=ide --apcs=interwork --depend=$(@:%.o=%.d) -I"D:\Keil\ARM\RV31\INC" -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


