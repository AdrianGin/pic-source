################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/GLCD/AsciiLib.c \
../USER/GLCD/GLCD.c \
../USER/GLCD/HzLib.c 

OBJS += \
./USER/GLCD/AsciiLib.o \
./USER/GLCD/GLCD.o \
./USER/GLCD/HzLib.o 

C_DEPS += \
./USER/GLCD/AsciiLib.d \
./USER/GLCD/GLCD.d \
./USER/GLCD/HzLib.d 


# Each subdirectory must supply rules for building sources it contributes
USER/GLCD/%.o: ../USER/GLCD/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


