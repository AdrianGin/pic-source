cd default
avr-objcopy -Iihex EdrumBootloader.hex -Obinary EdrumBootloader.bin
avrdude -pm8 -e -V -U flash:w:hidboot.hex -c ftisp -P ft0 
REM ftisp -E -fw EdrumBootloader.bin
ftisp -Fr
cd ..