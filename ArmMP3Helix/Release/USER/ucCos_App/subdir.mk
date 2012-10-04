################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/ucCos_App/OS_AppHooks.c \
../USER/ucCos_App/uctsk_MP3Decode.c \
../USER/ucCos_App/uctsk_MicrochipGUI.c \
../USER/ucCos_App/uctsk_OSView.c \
../USER/ucCos_App/uctsk_ReadButton.c \
../USER/ucCos_App/uctsk_TouchScreen.c 

OBJS += \
./USER/ucCos_App/OS_AppHooks.o \
./USER/ucCos_App/uctsk_MP3Decode.o \
./USER/ucCos_App/uctsk_MicrochipGUI.o \
./USER/ucCos_App/uctsk_OSView.o \
./USER/ucCos_App/uctsk_ReadButton.o \
./USER/ucCos_App/uctsk_TouchScreen.o 

C_DEPS += \
./USER/ucCos_App/OS_AppHooks.d \
./USER/ucCos_App/uctsk_MP3Decode.d \
./USER/ucCos_App/uctsk_MicrochipGUI.d \
./USER/ucCos_App/uctsk_OSView.d \
./USER/ucCos_App/uctsk_ReadButton.d \
./USER/ucCos_App/uctsk_TouchScreen.d 


# Each subdirectory must supply rules for building sources it contributes
USER/ucCos_App/%.o: ../USER/ucCos_App/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


