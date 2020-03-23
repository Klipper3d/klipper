echo 196 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio196/direction
echo 0 > /sys/class/gpio/gpio196/value
sleep 0.1
echo 1 > /sys/class/gpio/gpio196/value
