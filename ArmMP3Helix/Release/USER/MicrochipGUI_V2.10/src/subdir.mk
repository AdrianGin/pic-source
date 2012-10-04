################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/MicrochipGUI_V2.10/src/Button.c \
../USER/MicrochipGUI_V2.10/src/Chart.c \
../USER/MicrochipGUI_V2.10/src/CheckBox.c \
../USER/MicrochipGUI_V2.10/src/DigitalMeter.c \
../USER/MicrochipGUI_V2.10/src/EditBox.c \
../USER/MicrochipGUI_V2.10/src/GOL.c \
../USER/MicrochipGUI_V2.10/src/GOLFontDefault.c \
../USER/MicrochipGUI_V2.10/src/Grid.c \
../USER/MicrochipGUI_V2.10/src/GroupBox.c \
../USER/MicrochipGUI_V2.10/src/ListBox.c \
../USER/MicrochipGUI_V2.10/src/Meter.c \
../USER/MicrochipGUI_V2.10/src/Palette.c \
../USER/MicrochipGUI_V2.10/src/Picture.c \
../USER/MicrochipGUI_V2.10/src/Primitive.c \
../USER/MicrochipGUI_V2.10/src/ProgressBar.c \
../USER/MicrochipGUI_V2.10/src/RadioButton.c \
../USER/MicrochipGUI_V2.10/src/RoundDial.c \
../USER/MicrochipGUI_V2.10/src/Slider.c \
../USER/MicrochipGUI_V2.10/src/StaticText.c \
../USER/MicrochipGUI_V2.10/src/Template.c \
../USER/MicrochipGUI_V2.10/src/TextEntry.c \
../USER/MicrochipGUI_V2.10/src/TouchScreen.c \
../USER/MicrochipGUI_V2.10/src/Window.c \
../USER/MicrochipGUI_V2.10/src/drvTFT001.c 

OBJS += \
./USER/MicrochipGUI_V2.10/src/Button.o \
./USER/MicrochipGUI_V2.10/src/Chart.o \
./USER/MicrochipGUI_V2.10/src/CheckBox.o \
./USER/MicrochipGUI_V2.10/src/DigitalMeter.o \
./USER/MicrochipGUI_V2.10/src/EditBox.o \
./USER/MicrochipGUI_V2.10/src/GOL.o \
./USER/MicrochipGUI_V2.10/src/GOLFontDefault.o \
./USER/MicrochipGUI_V2.10/src/Grid.o \
./USER/MicrochipGUI_V2.10/src/GroupBox.o \
./USER/MicrochipGUI_V2.10/src/ListBox.o \
./USER/MicrochipGUI_V2.10/src/Meter.o \
./USER/MicrochipGUI_V2.10/src/Palette.o \
./USER/MicrochipGUI_V2.10/src/Picture.o \
./USER/MicrochipGUI_V2.10/src/Primitive.o \
./USER/MicrochipGUI_V2.10/src/ProgressBar.o \
./USER/MicrochipGUI_V2.10/src/RadioButton.o \
./USER/MicrochipGUI_V2.10/src/RoundDial.o \
./USER/MicrochipGUI_V2.10/src/Slider.o \
./USER/MicrochipGUI_V2.10/src/StaticText.o \
./USER/MicrochipGUI_V2.10/src/Template.o \
./USER/MicrochipGUI_V2.10/src/TextEntry.o \
./USER/MicrochipGUI_V2.10/src/TouchScreen.o \
./USER/MicrochipGUI_V2.10/src/Window.o \
./USER/MicrochipGUI_V2.10/src/drvTFT001.o 

C_DEPS += \
./USER/MicrochipGUI_V2.10/src/Button.d \
./USER/MicrochipGUI_V2.10/src/Chart.d \
./USER/MicrochipGUI_V2.10/src/CheckBox.d \
./USER/MicrochipGUI_V2.10/src/DigitalMeter.d \
./USER/MicrochipGUI_V2.10/src/EditBox.d \
./USER/MicrochipGUI_V2.10/src/GOL.d \
./USER/MicrochipGUI_V2.10/src/GOLFontDefault.d \
./USER/MicrochipGUI_V2.10/src/Grid.d \
./USER/MicrochipGUI_V2.10/src/GroupBox.d \
./USER/MicrochipGUI_V2.10/src/ListBox.d \
./USER/MicrochipGUI_V2.10/src/Meter.d \
./USER/MicrochipGUI_V2.10/src/Palette.d \
./USER/MicrochipGUI_V2.10/src/Picture.d \
./USER/MicrochipGUI_V2.10/src/Primitive.d \
./USER/MicrochipGUI_V2.10/src/ProgressBar.d \
./USER/MicrochipGUI_V2.10/src/RadioButton.d \
./USER/MicrochipGUI_V2.10/src/RoundDial.d \
./USER/MicrochipGUI_V2.10/src/Slider.d \
./USER/MicrochipGUI_V2.10/src/StaticText.d \
./USER/MicrochipGUI_V2.10/src/Template.d \
./USER/MicrochipGUI_V2.10/src/TextEntry.d \
./USER/MicrochipGUI_V2.10/src/TouchScreen.d \
./USER/MicrochipGUI_V2.10/src/Window.d \
./USER/MicrochipGUI_V2.10/src/drvTFT001.d 


# Each subdirectory must supply rules for building sources it contributes
USER/MicrochipGUI_V2.10/src/%.o: ../USER/MicrochipGUI_V2.10/src/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


