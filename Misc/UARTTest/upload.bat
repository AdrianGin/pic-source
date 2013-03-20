cd default
avr-objcopy -Iihex UARTTest.hex -Obinary UARTTest.bin
REM avrdude -B 9600 -patmega8 -e -U flash:w:AVRMIDI.hex -c ftisp -P ft0
ftisp -E -fw UARTTest.bin
cd ..