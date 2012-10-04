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
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


