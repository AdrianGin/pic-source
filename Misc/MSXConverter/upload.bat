cd default
avr-objcopy -Iihex MSXConv.hex -Obinary MSXConv.bin
avrdude -pm328p -e -V -U flash:w:MSXConv.hex -c ftisp -P ft0
rem avrdude -pm328p -U lfuse:r:lfuse:r -c ftisp -P ft0 -B300
ftisp -Fr
cd ..