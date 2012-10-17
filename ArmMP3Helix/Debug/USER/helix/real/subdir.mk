################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/helix/real/bitstream.c \
../USER/helix/real/buffers.c \
../USER/helix/real/dct32.c \
../USER/helix/real/dequant.c \
../USER/helix/real/dqchan.c \
../USER/helix/real/huffman.c \
../USER/helix/real/hufftabs.c \
../USER/helix/real/imdct.c \
../USER/helix/real/scalfact.c \
../USER/helix/real/stproc.c \
../USER/helix/real/subband.c \
../USER/helix/real/trigtabs.c 

OBJS += \
./USER/helix/real/bitstream.o \
./USER/helix/real/buffers.o \
./USER/helix/real/dct32.o \
./USER/helix/real/dequant.o \
./USER/helix/real/dqchan.o \
./USER/helix/real/huffman.o \
./USER/helix/real/hufftabs.o \
./USER/helix/real/imdct.o \
./USER/helix/real/scalfact.o \
./USER/helix/real/stproc.o \
./USER/helix/real/subband.o \
./USER/helix/real/trigtabs.o 

C_DEPS += \
./USER/helix/real/bitstream.d \
./USER/helix/real/buffers.d \
./USER/helix/real/dct32.d \
./USER/helix/real/dequant.d \
./USER/helix/real/dqchan.d \
./USER/helix/real/huffman.d \
./USER/helix/real/hufftabs.d \
./USER/helix/real/imdct.d \
./USER/helix/real/scalfact.d \
./USER/helix/real/stproc.d \
./USER/helix/real/subband.d \
./USER/helix/real/trigtabs.d 


# Each subdirectory must supply rules for building sources it contributes
USER/helix/real/%.o: ../USER/helix/real/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --split_sections --debug -c --depend=$(@:%.o=%.d) --cpu=Cortex-M3 --apcs=interwork -O0 --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\CoreSupport" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32_USB-FS-Device_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\src" -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\USER\WM8731" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\real" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\pub" -I"M:\Robotics\Source\ArmMP3Helix\USER\Audio" -I"M:\Robotics\Source\ArmMP3Helix\USER\SD_Card" -I"M:\Robotics\Source\ArmMP3Helix\USER\FATFS_V0.08A\src" -I"M:\Robotics\Source\ArmMP3Helix\USER\MP3Header" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\inc" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-VIEW" -I"M:\Robotics\Source\ArmMP3Helix\USER\GLCD" -I"M:\Robotics\Source\ArmMP3Helix\USER\TouchPanel" -I"M:\Robotics\Source\ArmMP3Helix\USER\ucCos_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-CPU" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Font" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-LIB" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-II" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\GUI_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\USB" -I"M:\Robotics\Source\ArmMP3Helix\USER\SPI_Flash" -I"M:\Robotics\Source\ArmMP3Helix\USER\Unicode" -I"D:\Keil\ARM\RV31\INC" -I"D:\Keil\ARM\CMSIS\Include" -I"D:\Keil\ARM\Inc\ST\STM32F10x" -D__MICROLIB -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


