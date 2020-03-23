echo 196 > /sys/class/gpio/export
echo 197 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio197/direction
echo 1 > /sys/class/gpio/gpio197/value
sleep 1
stm32flash -i -196,196 /dev/ttyS1
sleep 1
stm32flash -w klipper.bin -v -g 0x00 /dev/ttyS1
echo 0 > /sys/class/gpio/gpio197/value
stm32flash -i -196,196 /dev/ttyS1
