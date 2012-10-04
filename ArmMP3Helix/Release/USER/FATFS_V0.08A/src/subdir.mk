################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/FATFS_V0.08A/src/diskio.c \
../USER/FATFS_V0.08A/src/ff.c 

OBJS += \
./USER/FATFS_V0.08A/src/diskio.o \
./USER/FATFS_V0.08A/src/ff.o 

C_DEPS += \
./USER/FATFS_V0.08A/src/diskio.d \
./USER/FATFS_V0.08A/src/ff.d 


# Each subdirectory must supply rules for building sources it contributes
USER/FATFS_V0.08A/src/%.o: ../USER/FATFS_V0.08A/src/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


