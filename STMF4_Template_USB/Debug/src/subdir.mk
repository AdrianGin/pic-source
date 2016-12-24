################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f4xx_it.c \
../src/system_stm32f4xx.c \
../src/usb_bsp.c \
../src/usbd_desc.c \
../src/usbd_usr.c 

OBJS += \
./src/main.o \
./src/stm32f4xx_it.o \
./src/system_stm32f4xx.o \
./src/usb_bsp.o \
./src/usbd_desc.o \
./src/usbd_usr.o 

C_DEPS += \
./src/main.d \
./src/stm32f4xx_it.d \
./src/system_stm32f4xx.d \
./src/usb_bsp.d \
./src/usbd_desc.d \
./src/usbd_usr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DHSE_VALUE=8000000UL -DSTM32F4XX -DSTM32F40_41xxx -DUSE_EMBEDDED_PHY -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\CMSIS\Include" -I"F:\Robotics\pic-source\STMF4_Template_USB\src" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32_USB_OTG_Driver\inc" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32_USB_Device_Library\Core\inc" -I"F:\Robotics\pic-source\STMF4_Template_USB\inc" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32_USB_Device_Library\Class\audio\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=soft -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


