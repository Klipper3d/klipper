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
KLIPPER_HOST_MCU=/usr/local/bin/klipper_mcu
KLIPPER_HOST_ARGS="-w -r"
PIDFILE=/var/run/klipper_mcu.pid
RPROC0=/sys/class/remoteproc/remoteproc1
RPROC1=/sys/class/remoteproc/remoteproc2

. /lib/lsb/init-functions

pru_stop()
{
    # Shutdown existing Klipper instance (if applicable). The goal is to
    # put the GPIO pins in a safe state.
    if [ -c /dev/rpmsg_pru30 ]; then
        log_daemon_msg "Attempting to shutdown PRU..."
        set -e
        ( echo "FORCE_SHUTDOWN" > /dev/rpmsg_pru30 ) 2> /dev/null || ( log_action_msg "Firmware busy! Please shutdown Klipper and then retry." && exit 1 )
        sleep 1
        ( echo "FORCE_SHUTDOWN" > /dev/rpmsg_pru30 ) 2> /dev/null || ( log_action_msg "Firmware busy! Please shutdown Klipper and then retry." && exit 1 )
        sleep 1
        set +e
    fi

    log_daemon_msg "Stopping pru"
        echo 'stop' > $RPROC0/state
        echo 'stop' > $RPROC1/state
}

pru_start()
{
    if [ -c /dev/rpmsg_pru30 ]; then
        pru_stop
    else
        echo 'stop' > $RPROC0/state
        echo 'stop' > $RPROC1/state
    fi
    sleep 1

    log_daemon_msg "Starting pru"
        echo 'start' > $RPROC0/state
        echo 'start' > $RPROC1/state

    # log_daemon_msg "Loading ADC module"
    # echo 'BB-ADC' > /sys/devices/platform/bone_capemgr/slots
}

mcu_host_stop()
{
    # Shutdown existing Klipper instance (if applicable). The goal is to
    # put the GPIO pins in a safe state.
    if [ -c /tmp/klipper_host_mcu ]; then
        log_daemon_msg "Attempting to shutdown host mcu..."
        set -e
        ( echo "FORCE_SHUTDOWN" > /tmp/klipper_host_mcu ) 2> /dev/null || ( log_action_msg "Firmware busy! Please shutdown Klipper and then retry." && exit 1 )
        sleep 1
        ( echo "FORCE_SHUTDOWN" > /tmp/klipper_host_mcu ) 2> /dev/null || ( log_action_msg "Firmware busy! Please shutdown Klipper and then retry." && exit 1 )
        sleep 1
        set +e
    fi

    log_daemon_msg "Stopping klipper host mcu" $NAME
    killproc -p $PIDFILE $KLIPPER_HOST_MCU
}

mcu_host_start()
{
    [ -x $KLIPPER_HOST_MCU ] || return

    if [ -c /tmp/klipper_host_mcu ]; then
        mcu_host_stop
    fi

    log_daemon_msg "Starting klipper MCU" $NAME
    start-stop-daemon --start --quiet --exec $KLIPPER_HOST_MCU \
                      --background --pidfile $PIDFILE --make-pidfile \
                      -- $KLIPPER_HOST_ARGS
    log_end_msg $?
}

case "$1" in
start)
    pru_start
    mcu_host_start
    ;;
stop)
    pru_stop
    mcu_host_stop
    ;;
restart)
    $0 stop
    $0 start
    ;;
reload|force-reload)
    log_daemon_msg "Reloading configuration not supported" $NAME
    log_end_msg 1
    ;;
status)
    status_of_proc -p $PIDFILE $KLIPPER_HOST_MCU $NAME && exit 0 || exit $?
    ;;
*)  log_action_msg "Usage: /etc/init.d/klipper {start|stop|status|restart|reload|force-reload}"
    exit 2
    ;;
esac
exit 0
