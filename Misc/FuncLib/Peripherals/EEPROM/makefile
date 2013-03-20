#Makefile template for..!
#


# AVR-GCC Options
CC = avr-gcc
FLAGS = -Wall -g 
COMPILE = -c
OUTPUT = -o
MMCU = atmega8
# AVR-GCC Options



# AVR-Linker options
LD = avr-objcopy
LD_FLAGS = -O ihex -R .eeprom
# AVR-Linker options




# AVR-Flash Programmer Options
PRG = sp12
PRG_FLAGS = -wpf 
PRG_FUSES = -wF10100100 -T1
PRG_ERASE_FLAG = -E -T1
# AVR-Flash Programmer Options 



#Source File paths
#Add the full path name to each source under SOURCE_FILES followed by a '\'#

MAIN_FILE = D:\WinAVR\dev\EEPROM\eeprom.c
MAIN_OBJECT = $(MAIN_FILE:.c=.o)

SOURCE_FILES = D:\WinAVR\dev\Pause\pause.c \
               D:\WinAVR\dev\UART\Tx.c 

#Source File paths

#Header and Object File paths
HEADER_FILES = $(SOURCE_FILES:.c=.h)
OBJECTS = $(SOURCE_FILES:.c=.o) 
#Header and Object File paths

#Project name:
PROGRAM = main
#Project name
	
#Remove file command#	
RM = rm
#Remove file command#

all: $(PROGRAM) size
	@echo "Make complete!"
	
#Upload the hex file to the connected micro	
$(PROGRAM): $(PROGRAM).elf
	$(PRG) $(PRG_FLAGS) $(PROGRAM).hex $(PRG_FUSES)
#Upload

#Compiling the source files	
%.o : %.h %.c
	$(CC) $(FLAGS) $(COMPILE) -mmcu=$(MMCU) $*.c -o $@
#Compiling the source files

#Compiling the main() file
$(MAIN_OBJECT): $(MAIN_FILE)
	$(CC) $(FLAGS) $(COMPILE) -mmcu=$(MMCU) $*.c -o $@
#Compiling the main() file	

#Linking the source files to generate a hex file
$(PROGRAM).elf: $(OBJECTS) $(MAIN_OBJECT) 
	$(CC) $(FLAGS) -mmcu=$(MMCU) $^ $(OUTPUT) $(PROGRAM).elf
	$(LD) $(LD_FLAGS) $(PROGRAM).elf $(PROGRAM).hex
#Linking the source files to generate a hex file

#Compute the size of the hex file
size:
	avr-size $(PROGRAM).hex
#size

#Delete all object files and delete the hex file
clean:
	@echo "Cleaning up objects, elfs and hex files." 
	$(RM) $(OBJECTS) $(MAIN_OBJECT)
	$(RM) *.elf *.hex
#clean

#Erase the connected micro	
erase:
	$(PRG) $(PRG_ERASE_FLAG)
#Erase


