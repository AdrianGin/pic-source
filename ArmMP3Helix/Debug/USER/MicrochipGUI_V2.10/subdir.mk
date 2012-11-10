################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/MicrochipGUI_V2.10/AnalogClock.c \
../USER/MicrochipGUI_V2.10/Button.c \
../USER/MicrochipGUI_V2.10/Chart.c \
../USER/MicrochipGUI_V2.10/CheckBox.c \
../USER/MicrochipGUI_V2.10/DigitalMeter.c \
../USER/MicrochipGUI_V2.10/EditBox.c \
../USER/MicrochipGUI_V2.10/GOL.c \
../USER/MicrochipGUI_V2.10/GOLFontDefault.c \
../USER/MicrochipGUI_V2.10/GOLSchemeDefault.c \
../USER/MicrochipGUI_V2.10/Grid.c \
../USER/MicrochipGUI_V2.10/GroupBox.c \
../USER/MicrochipGUI_V2.10/ListBox.c \
../USER/MicrochipGUI_V2.10/Meter.c \
../USER/MicrochipGUI_V2.10/Palette.c \
../USER/MicrochipGUI_V2.10/Picture.c \
../USER/MicrochipGUI_V2.10/Pictures.c \
../USER/MicrochipGUI_V2.10/Primitive.c \
../USER/MicrochipGUI_V2.10/ProgressBar.c \
../USER/MicrochipGUI_V2.10/RadioButton.c \
../USER/MicrochipGUI_V2.10/RoundDial.c \
../USER/MicrochipGUI_V2.10/Slider.c \
../USER/MicrochipGUI_V2.10/StaticText.c \
../USER/MicrochipGUI_V2.10/Template.c \
../USER/MicrochipGUI_V2.10/TextEntry.c \
../USER/MicrochipGUI_V2.10/TouchScreen.c \
../USER/MicrochipGUI_V2.10/Transitions.c \
../USER/MicrochipGUI_V2.10/Transitions_weak.c \
../USER/MicrochipGUI_V2.10/Window.c \
../USER/MicrochipGUI_V2.10/externalref.c 

OBJS += \
./USER/MicrochipGUI_V2.10/AnalogClock.o \
./USER/MicrochipGUI_V2.10/Button.o \
./USER/MicrochipGUI_V2.10/Chart.o \
./USER/MicrochipGUI_V2.10/CheckBox.o \
./USER/MicrochipGUI_V2.10/DigitalMeter.o \
./USER/MicrochipGUI_V2.10/EditBox.o \
./USER/MicrochipGUI_V2.10/GOL.o \
./USER/MicrochipGUI_V2.10/GOLFontDefault.o \
./USER/MicrochipGUI_V2.10/GOLSchemeDefault.o \
./USER/MicrochipGUI_V2.10/Grid.o \
./USER/MicrochipGUI_V2.10/GroupBox.o \
./USER/MicrochipGUI_V2.10/ListBox.o \
./USER/MicrochipGUI_V2.10/Meter.o \
./USER/MicrochipGUI_V2.10/Palette.o \
./USER/MicrochipGUI_V2.10/Picture.o \
./USER/MicrochipGUI_V2.10/Pictures.o \
./USER/MicrochipGUI_V2.10/Primitive.o \
./USER/MicrochipGUI_V2.10/ProgressBar.o \
./USER/MicrochipGUI_V2.10/RadioButton.o \
./USER/MicrochipGUI_V2.10/RoundDial.o \
./USER/MicrochipGUI_V2.10/Slider.o \
./USER/MicrochipGUI_V2.10/StaticText.o \
./USER/MicrochipGUI_V2.10/Template.o \
./USER/MicrochipGUI_V2.10/TextEntry.o \
./USER/MicrochipGUI_V2.10/TouchScreen.o \
./USER/MicrochipGUI_V2.10/Transitions.o \
./USER/MicrochipGUI_V2.10/Transitions_weak.o \
./USER/MicrochipGUI_V2.10/Window.o \
./USER/MicrochipGUI_V2.10/externalref.o 

C_DEPS += \
./USER/MicrochipGUI_V2.10/AnalogClock.d \
./USER/MicrochipGUI_V2.10/Button.d \
./USER/MicrochipGUI_V2.10/Chart.d \
./USER/MicrochipGUI_V2.10/CheckBox.d \
./USER/MicrochipGUI_V2.10/DigitalMeter.d \
./USER/MicrochipGUI_V2.10/EditBox.d \
./USER/MicrochipGUI_V2.10/GOL.d \
./USER/MicrochipGUI_V2.10/GOLFontDefault.d \
./USER/MicrochipGUI_V2.10/GOLSchemeDefault.d \
./USER/MicrochipGUI_V2.10/Grid.d \
./USER/MicrochipGUI_V2.10/GroupBox.d \
./USER/MicrochipGUI_V2.10/ListBox.d \
./USER/MicrochipGUI_V2.10/Meter.d \
./USER/MicrochipGUI_V2.10/Palette.d \
./USER/MicrochipGUI_V2.10/Picture.d \
./USER/MicrochipGUI_V2.10/Pictures.d \
./USER/MicrochipGUI_V2.10/Primitive.d \
./USER/MicrochipGUI_V2.10/ProgressBar.d \
./USER/MicrochipGUI_V2.10/RadioButton.d \
./USER/MicrochipGUI_V2.10/RoundDial.d \
./USER/MicrochipGUI_V2.10/Slider.d \
./USER/MicrochipGUI_V2.10/StaticText.d \
./USER/MicrochipGUI_V2.10/Template.d \
./USER/MicrochipGUI_V2.10/TextEntry.d \
./USER/MicrochipGUI_V2.10/TouchScreen.d \
./USER/MicrochipGUI_V2.10/Transitions.d \
./USER/MicrochipGUI_V2.10/Transitions_weak.d \
./USER/MicrochipGUI_V2.10/Window.d \
./USER/MicrochipGUI_V2.10/externalref.d 


# Each subdirectory must supply rules for building sources it contributes
USER/MicrochipGUI_V2.10/%.o: ../USER/MicrochipGUI_V2.10/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --split_sections --debug -c --depend=$(@:%.o=%.d) --cpu=Cortex-M3 --apcs=interwork -O3 -Otime --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\CoreSupport" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\src" -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\USER\WM8731" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\real" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\pub" -I"M:\Robotics\Source\ArmMP3Helix\USER\Audio" -I"M:\Robotics\Source\ArmMP3Helix\USER\SD_Card" -I"M:\Robotics\Source\ArmMP3Helix\USER\FATFS_V0.08A\src" -I"M:\Robotics\Source\ArmMP3Helix\USER\MP3Header" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-VIEW" -I"M:\Robotics\Source\ArmMP3Helix\USER\GLCD" -I"M:\Robotics\Source\ArmMP3Helix\USER\TouchPanel" -I"M:\Robotics\Source\ArmMP3Helix\USER\ucCos_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-CPU" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Font" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-LIB" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-II" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\GUI_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\SPI_Flash" -I"M:\Robotics\Source\ArmMP3Helix\USER\Unicode" -I"D:\Keil\ARM\RV31\INC" -I"D:\Keil\ARM\CMSIS\Include" -I"D:\Keil\ARM\Inc\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32_USB-FS-Device_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\USER\USB" -I"M:\Robotics\Source\ArmMP3Helix\USER\FreeRTOS\Source\include" -I"M:\Robotics\Source\ArmMP3Helix\USER\FreeRTOS\Source\portable\RVDS\ARM_CM3" -I"M:\Robotics\Source\ArmMP3Helix\USER\OS_Abstraction" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Graphics" -D__MICROLIB -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000UL --brief_diagnostics --depend_format=unix_escaped --no_implicit_include -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


