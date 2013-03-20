#Makefile template for..!
#

CC = avr-gcc
FLAGS = -Wall -g 
MMCU = atmega8

OBJ = avr-objcopy
OBJ_FLAGS = -O ihex -R .eeprom

PRG = sp12
PRG_FLAGS = -wpf 
PRG_FUSES = -wF10100100 -T1
all: main size
	@echo "Make complete!"
	
main: 
	$(CC) $(FLAGS) -mmcu=$(MMCU) pwmexample.c pwm.c pause.c -o cmd.elf
	$(OBJ) $(OBJ_FLAGS) cmd.elf temp.hex	
	$(PRG) $(PRG_FLAGS) temp.hex $(PRG_FUSES)
 

size:
	avr-size temp.hex