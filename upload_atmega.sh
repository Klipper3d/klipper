#sudo service klipper stop
make && avrdude -cwiring -patmega2560 -V -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
#sudo service klipper start
