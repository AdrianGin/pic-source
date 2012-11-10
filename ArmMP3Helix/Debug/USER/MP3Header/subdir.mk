################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/MP3Header/MP3header.c 

OBJS += \
./USER/MP3Header/MP3header.o 

C_DEPS += \
./USER/MP3Header/MP3header.d 


# Each subdirectory must supply rules for building sources it contributes
USER/MP3Header/%.o: ../USER/MP3Header/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --split_sections --debug -c --depend=$(@:%.o=%.d) --cpu=Cortex-M3 --apcs=interwork -O3 -Otime --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\CoreSupport" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\src" -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\USER\WM8731" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\real" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\pub" -I"M:\Robotics\Source\ArmMP3Helix\USER\Audio" -I"M:\Robotics\Source\ArmMP3Helix\USER\SD_Card" -I"M:\Robotics\Source\ArmMP3Helix\USER\FATFS_V0.08A\src" -I"M:\Robotics\Source\ArmMP3Helix\USER\MP3Header" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-VIEW" -I"M:\Robotics\Source\ArmMP3Helix\USER\GLCD" -I"M:\Robotics\Source\ArmMP3Helix\USER\TouchPanel" -I"M:\Robotics\Source\ArmMP3Helix\USER\ucCos_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-CPU" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Font" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-LIB" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-II" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\GUI_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\SPI_Flash" -I"M:\Robotics\Source\ArmMP3Helix\USER\Unicode" -I"D:\Keil\ARM\RV31\INC" -I"D:\Keil\ARM\CMSIS\Include" -I"D:\Keil\ARM\Inc\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32_USB-FS-Device_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\USER\USB" -I"M:\Robotics\Source\ArmMP3Helix\USER\FreeRTOS\Source\include" -I"M:\Robotics\Source\ArmMP3Helix\USER\FreeRTOS\Source\portable\RVDS\ARM_CM3" -I"M:\Robotics\Source\ArmMP3Helix\USER\OS_Abstraction" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Graphics" -D__MICROLIB -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000UL --brief_diagnostics --depend_format=unix_escaped --no_implicit_include -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


