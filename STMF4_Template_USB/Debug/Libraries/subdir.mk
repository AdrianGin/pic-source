################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/Robotics/pic-source/FuncLib/ThirdPartyDrivers/Libraries/syscalls.c 

OBJS += \
./Libraries/syscalls.o 

C_DEPS += \
./Libraries/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/syscalls.o: F:/Robotics/pic-source/FuncLib/ThirdPartyDrivers/Libraries/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DHSE_VALUE=8000000UL -DSTM32F4XX -DSTM32F40_41xxx -DUSE_EMBEDDED_PHY -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\CMSIS\Include" -I"F:\Robotics\pic-source\STMF4_Template_USB\src" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32F4xx_StdPeriph_Driver\inc" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32_USB_OTG_Driver\inc" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32_USB_Device_Library\Core\inc" -I"F:\Robotics\pic-source\STMF4_Template_USB\inc" -I"F:\Robotics\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32_USB_Device_Library\Class\audio\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=soft -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


