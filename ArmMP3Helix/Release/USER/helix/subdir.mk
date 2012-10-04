################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/helix/mp3dec.c \
../USER/helix/mp3tabs.c 

OBJS += \
./USER/helix/mp3dec.o \
./USER/helix/mp3tabs.o 

C_DEPS += \
./USER/helix/mp3dec.d \
./USER/helix/mp3tabs.d 


# Each subdirectory must supply rules for building sources it contributes
USER/helix/%.o: ../USER/helix/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


