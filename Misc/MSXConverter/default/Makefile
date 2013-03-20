###############################################################################
# Makefile for the project MSXConv
###############################################################################

## General Flags
PROJECT = MSXConv
MCU = atmega328p
TARGET = MSXConv.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99         -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=MSXConv.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"M:\Robotics\Source\Misc\MSXConverter\..\..\FuncLib\Peripherals" -I"M:\Robotics\Source\Misc\MSXConverter\..\..\FuncLib\Other" 

## Objects that must be built in order to link
OBJECTS = MSXConv.o hardUart.o ringbuffer.o MSX_Mouse.o PS2_Mouse.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) MSXConv.hex MSXConv.eep MSXConv.lss size

## Compile
MSXConv.o: ../MSXConv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

hardUart.o: ../../../FuncLib/Peripherals/hardUART/hardUart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ringbuffer.o: ../../../FuncLib/Other/RingBuffer/ringbuffer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MSX_Mouse.o: ../MSX_Mouse.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PS2_Mouse.o: ../PS2_Mouse.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) MSXConv.elf dep/* MSXConv.hex MSXConv.eep MSXConv.lss MSXConv.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

