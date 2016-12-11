set filename=%1

avr-objcopy -j .text -j .data %filename%.elf -Obinary %filename%.bin
avr-objcopy -j .text -j .data %filename%.elf -Oihex %filename%.hex
avr-size %filename%.elf
avr-objdump -h -S %filename%.elf > %filename%.lst
REM avrdude -c arduino -Pcom8 -b57600 -pm328p -e -U flash:w:%filename%.hex:i