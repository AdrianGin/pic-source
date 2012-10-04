################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/FATFS_V0.08A/src/option/cc932.c \
../USER/FATFS_V0.08A/src/option/cc936.c \
../USER/FATFS_V0.08A/src/option/cc949.c \
../USER/FATFS_V0.08A/src/option/cc950.c \
../USER/FATFS_V0.08A/src/option/ccsbcs.c \
../USER/FATFS_V0.08A/src/option/syscall.c 

OBJS += \
./USER/FATFS_V0.08A/src/option/cc932.o \
./USER/FATFS_V0.08A/src/option/cc936.o \
./USER/FATFS_V0.08A/src/option/cc949.o \
./USER/FATFS_V0.08A/src/option/cc950.o \
./USER/FATFS_V0.08A/src/option/ccsbcs.o \
./USER/FATFS_V0.08A/src/option/syscall.o 

C_DEPS += \
./USER/FATFS_V0.08A/src/option/cc932.d \
./USER/FATFS_V0.08A/src/option/cc936.d \
./USER/FATFS_V0.08A/src/option/cc949.d \
./USER/FATFS_V0.08A/src/option/cc950.d \
./USER/FATFS_V0.08A/src/option/ccsbcs.d \
./USER/FATFS_V0.08A/src/option/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
USER/FATFS_V0.08A/src/option/%.o: ../USER/FATFS_V0.08A/src/option/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --cpu=Cortex-M3 --apcs=interwork -O0 --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\CoreSupport" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\src" -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\USER\WM8731" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\real" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\pub" -I"M:\Robotics\Source\ArmMP3Helix\USER\Audio" -I"M:\Robotics\Source\ArmMP3Helix\USER\SD_Card" -I"M:\Robotics\Source\ArmMP3Helix\USER\FATFS_V0.08A\src" -I"M:\Robotics\Source\ArmMP3Helix\USER\MP3Header" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\inc" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-VIEW" -I"M:\Robotics\Source\ArmMP3Helix\USER\GLCD" -I"M:\Robotics\Source\ArmMP3Helix\USER\TouchPanel" -I"M:\Robotics\Source\ArmMP3Helix\USER\ucCos_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-CPU" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Font" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-LIB" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-II" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\GUI_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\SPI_Flash" -I"M:\Robotics\Source\ArmMP3Helix\USER\Unicode" -I"D:\Keil\ARM\RV31\Inc" -I"D:\Keil\ARM\CMSIS\Include" -I"D:\Keil\ARM\Inc\ST\STM32F10x" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


