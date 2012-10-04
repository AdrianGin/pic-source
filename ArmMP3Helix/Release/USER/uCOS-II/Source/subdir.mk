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
	armcc --debug -c --depend=$(@:%.o=%.d) --apcs=interwork --diag_style=ide -I"M:\Robotics\Source\ArmMP3Helix\USER" -I"D:\Keil\ARM\RV31\INC" --brief_diagnostics --depend_format=unix_escaped -o "$@" "$<" && \
	echo Generated dependency file $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


