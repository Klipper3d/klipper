#!/bin/sh
# System startup script to start the PRU firmware

### BEGIN INIT INFO
# Provides:          klipper_pru
# Required-Start:    $local_fs
# Required-Stop:
# Default-Start:     3 4 5
# Default-Stop:      0 1 2 6
# Short-Description: Klipper_PRU daemon
# Description:       Starts the PRU for Klipper.
### END INIT INFO

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
DESC="klipper_pru startup"
NAME="klipper_pru"

. /lib/lsb/init-functions

case "$1" in
start)  log_daemon_msg "Starting klipper_pru" $NAME
        # Enable ADC module
        echo 'BB-ADC' > /sys/devices/platform/bone_capemgr/slots
        # Start PRU firmware
        if [ ! -c /dev/rpmsg_pru30 ]; then
            rmmod -f pru_rproc
            modprobe pru_rproc
        fi
        ;;
stop|restart|reload|force-reload|status)
        ;;
*)      log_action_msg "Usage: /etc/init.d/klipper_pru {start|stop|status|restart|reload|force-reload}"
        exit 2
        ;;
esac
exit 0
