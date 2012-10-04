################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/uCOS-II/Source/os_core.c \
../USER/uCOS-II/Source/os_flag.c \
../USER/uCOS-II/Source/os_mbox.c \
../USER/uCOS-II/Source/os_mem.c \
../USER/uCOS-II/Source/os_mutex.c \
../USER/uCOS-II/Source/os_q.c \
../USER/uCOS-II/Source/os_sem.c \
../USER/uCOS-II/Source/os_task.c \
../USER/uCOS-II/Source/os_time.c \
../USER/uCOS-II/Source/os_tmr.c 

OBJS += \
./USER/uCOS-II/Source/os_core.o \
./USER/uCOS-II/Source/os_flag.o \
./USER/uCOS-II/Source/os_mbox.o \
./USER/uCOS-II/Source/os_mem.o \
./USER/uCOS-II/Source/os_mutex.o \
./USER/uCOS-II/Source/os_q.o \
./USER/uCOS-II/Source/os_sem.o \
./USER/uCOS-II/Source/os_task.o \
./USER/uCOS-II/Source/os_time.o \
./USER/uCOS-II/Source/os_tmr.o 

C_DEPS += \
./USER/uCOS-II/Source/os_core.d \
./USER/uCOS-II/Source/os_flag.d \
./USER/uCOS-II/Source/os_mbox.d \
./USER/uCOS-II/Source/os_mem.d \
./USER/uCOS-II/Source/os_mutex.d \
./USER/uCOS-II/Source/os_q.d \
./USER/uCOS-II/Source/os_sem.d \
./USER/uCOS-II/Source/os_task.d \
./USER/uCOS-II/Source/os_time.d \
./USER/uCOS-II/Source/os_tmr.d 


# Each subdirectory must supply rules for building sources it contributes
USER/uCOS-II/Source/%.o: ../USER/uCOS-II/Source/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --split_sections --debug -c --depend=$(@:%.o=%.d) --cpu=Cortex-M3 --apcs=interwork -Otime --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\CoreSupport" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32_USB-FS-Device_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\USER\FreeRTOS\Source\include" -I"M:\Robotics\Source\ArmMP3Helix\USER\FreeRTOS\Source\portable\RVDS\ARM_CM3" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\inc" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\STM32F10x_StdPeriph_Driver\src" -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"M:\Robotics\Source\ArmMP3Helix\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"M:\Robotics\Source\ArmMP3Helix\USER\WM8731" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\real" -I"M:\Robotics\Source\ArmMP3Helix\USER\helix\pub" -I"M:\Robotics\Source\ArmMP3Helix\USER\Audio" -I"M:\Robotics\Source\ArmMP3Helix\USER\SD_Card" -I"M:\Robotics\Source\ArmMP3Helix\USER\FATFS_V0.08A\src" -I"M:\Robotics\Source\ArmMP3Helix\USER\MP3Header" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\inc" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-VIEW" -I"M:\Robotics\Source\ArmMP3Helix\USER\GLCD" -I"M:\Robotics\Source\ArmMP3Helix\USER\TouchPanel" -I"M:\Robotics\Source\ArmMP3Helix\USER\ucCos_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-CPU" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\Font" -I"M:\Robotics\Source\ArmMP3Helix\USER\uC-LIB" -I"M:\Robotics\Source\ArmMP3Helix\USER\uCOS-II" -I"M:\Robotics\Source\ArmMP3Helix\USER\MicrochipGUI_V2.10\GUI_App" -I"M:\Robotics\Source\ArmMP3Helix\USER\USB" -I"M:\Robotics\Source\ArmMP3Helix\USER\SPI_Flash" -I"M:\Robotics\Source\ArmMP3Helix\USER\Unicode" -I"D:\Keil\ARM\RV31\INC" -I"D:\Keil\ARM\CMSIS\Include" -I"D:\Keil\ARM\Inc\ST\STM32F10x" -D__MICROLIB -DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '

