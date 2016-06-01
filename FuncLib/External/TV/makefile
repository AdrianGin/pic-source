#Makefile template for..!
#


# AVR-GCC Options
CC = avr-gcc
FLAGS = $(WARNINGS) $(DEBUG) $(OPTIMISE)

WARNINGS = -Wall
DEBUG = -gstabs
OPTIMISE = -Os

COMPILE = -c
OUTPUT = -o
MMCU = atmega8
# End of AVR-GCC Options



# AVR Elf to Hex conversion options
OBJCOPY = avr-objcopy
OBJCOPY_FLAGS = -O ihex -R .eeprom
# End of AVR Elf to Hex conversion options

# AVR-Linker options
LD = avr-gcc
LD_FLAGS = $(WARNINGS) $(DEBUG) $(OPTIMISE)
# End of AVR-Linker options

# AVR-Archiver options
AR = avr-ar
AR_FLAGS = rc
AR_INDEX = avr-ranlib

LIBRARY_PATH = D:\WinAVR\dev
LIBRARY_NAME = \libmylib.a

LIB_PATH = D:\WinAVR\dev\libmylib.a

# End of AVR-Archiver options

# AVR-Flash Programmer Options
PRG = sp12
PRG_FLAGS = -wpf 
PRG_FUSES = -wF10100100 -T1
PRG_ERASE_FLAG = -E -T1
# AVR-Flash Programmer Options 



#Source File paths
#Add the full path name to each source under SOURCE_FILES followed by a '\'#

MAIN_FILE = main.c
MAIN_OBJECT = $(MAIN_FILE:.c=.o)

SOURCE_FILES = ..\Pause\pause.c \
					..\Uart\Tx.c \
					..\Uart\Rx.c \
					..\pwm\pwm.c\
					..\ADC\adc.c \

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


# Compiles all source files, links them, then uploads to micro#
# Note that linking depends on the library files produced
# So ensure that $(LIB_PATH) is upto date by invoking >'make lib'
all: upload
	@echo "Make complete!"


# Compile all source files to produce object files #
compile: $(OBJECTS) $(MAIN_OBJECT) 

#Compiling the source files	
%.o : %.h %.c
	$(CC) $(FLAGS) $(COMPILE) -mmcu=$(MMCU) $*.c $(OUTPUT) $@

#Compiling the main() file
$(MAIN_OBJECT): $(MAIN_FILE)
	@echo "Compiling the source file with main()"
	$(CC) $(FLAGS) $(COMPILE) -mmcu=$(MMCU) $*.c $(OUTPUT) $@


#Create a library for the .o files
lib: $(LIB_PATH)
$(LIB_PATH): $(OBJECTS)
	@echo "Creating a library for source files excluding the file containing"
	@echo "main() function..."
	@echo ""
	$(AR) $(AR_FLAGS) $(LIB_PATH) $(OBJECTS)
	@echo ""
	@echo "Indexing library...."
	$(AR_INDEX) $(LIB_PATH)


# Linking the source files to generate an executable
link: $(PROGRAM).elf
$(PROGRAM).elf: $(LIB_PATH) $(MAIN_OBJECT)
	@echo "Linking the object and library files..."
	$(LD) $(LD_FLAGS) -mmcu=$(MMCU) $(MAIN_OBJECT) $(LIB_PATH) $(OUTPUT) $(PROGRAM).elf

# Copy the executable to a hex file
copy: $(PROGRAM).hex 
$(PROGRAM).hex: $(PROGRAM).elf 
	@echo "Ouputting Hex file"
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(PROGRAM).elf $(PROGRAM).hex
#	
	
	
#Upload the hex file to the connected micro	
upload: copy size
	@echo "Uploading hex file to microcontroller..."
	$(PRG) $(PRG_FLAGS) $(PROGRAM).hex $(PRG_FUSES)


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


	

