################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USER/SPI_Flash/FlashDriver.c \
../USER/SPI_Flash/SST25VF016B.c 

OBJS += \
./USER/SPI_Flash/FlashDriver.o \
./USER/SPI_Flash/SST25VF016B.o 

C_DEPS += \
./USER/SPI_Flash/FlashDriver.d \
./USER/SPI_Flash/SST25VF016B.d 


# Each subdirectory must supply rules for building sources it contributes
USER/SPI_Flash/%.o: ../USER/SPI_Flash/%.c ../../ArmMP3Helix
	@echo 'Building file: $<'
	@echo 'Invoking: ARM RealView Compiler'
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


