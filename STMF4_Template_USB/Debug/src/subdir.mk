################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f4xx_it.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/stm32f4xx_it.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/stm32f4xx_it.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F429_439xx -DUSE_USB_OTG_FS -DHSE_VALUE=8000000UL -DSTM32F4XX -I"C:\Users\Adrian.Gin\Source\pic-source\FuncLib\ThirdPartyDrivers\Libraries\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\Users\Adrian.Gin\Source\pic-source\FuncLib\ThirdPartyDrivers\Libraries\CMSIS\Include" -I"C:\Users\Adrian.Gin\Source\pic-source\STMF4_Template\src" -I"C:\Users\Adrian.Gin\Source\pic-source\FuncLib\ThirdPartyDrivers\Libraries\STM32F4xx_StdPeriph_Driver\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=soft -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


