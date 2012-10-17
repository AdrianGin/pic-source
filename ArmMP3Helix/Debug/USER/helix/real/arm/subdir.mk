################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../USER/helix/real/arm/asmmisc.s \
../USER/helix/real/arm/asmpoly_thumb2.s 

OBJS += \
./USER/helix/real/arm/asmmisc.o \
./USER/helix/real/arm/asmpoly_thumb2.o 

S_DEPS += \
./USER/helix/real/arm/asmmisc.d \
./USER/helix/real/arm/asmpoly_thumb2.d 


# Each subdirectory must supply rules for building sources it contributes
USER/helix/real/arm/%.o: ../USER/helix/real/arm/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Assembler'
	armasm -g --brief_diagnostics --keep --diag_style=ide --cpu=Cortex-M3 --apcs=interwork --depend=$(@:%.o=%.d) -I"D:\Keil\ARM\RV31\INC" -I"D:\Keil\ARM\CMSIS\Include" -I"D:\Keil\ARM\Inc\ST\STM32F10x" --pd "__MICROLIB SETA 1" -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


