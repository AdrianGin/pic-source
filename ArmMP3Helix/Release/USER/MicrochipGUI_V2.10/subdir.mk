################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/MicrochipGUI_V2.10/Pictures.c \
../USER/MicrochipGUI_V2.10/externalref.c 

OBJS += \
./USER/MicrochipGUI_V2.10/Pictures.o \
./USER/MicrochipGUI_V2.10/externalref.o 

C_DEPS += \
./USER/MicrochipGUI_V2.10/Pictures.d \
./USER/MicrochipGUI_V2.10/externalref.d 


# Each subdirectory must supply rules for building sources it contributes
USER/MicrochipGUI_V2.10/%.o: ../USER/MicrochipGUI_V2.10/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


