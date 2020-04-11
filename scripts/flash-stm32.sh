echo 196 > /sys/class/gpio/export
echo 197 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio197/direction
echo out > /sys/class/gpio/gpio197/direction
echo 1 > /sys/class/gpio/gpio197/value
echo 0 > /sys/class/gpio/gpio196/value

stm32flash -i -196,196 -w klipper.bin -v -g 0x00 /dev/ttyS1
echo 0 > /sys/class/gpio/gpio197/value
stm32flash -i -196,196 /dev/ttyS1
