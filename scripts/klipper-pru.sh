#!/bin/sh
# Script to start the PRU firmware

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

. /lib/lsb/init-functions

pru_stop()
{
    # Shutdown existing Klipper instance (if applicable). The goal is to
    # put the GPIO pins in a safe state.
    if [ -c /dev/rpmsg_pru30 ]; then
        log_daemon_msg "Attempting to shutdown PRU...\n"
        set -e
        ( echo "FORCE_SHUTDOWN" > /dev/rpmsg_pru30 ) 2> /dev/null || ( log_action_msg "Firmware busy! Please shutdown Klipper and then retry.\n" && exit 1 )
        sleep 1
        ( echo "FORCE_SHUTDOWN" > /dev/rpmsg_pru30 ) 2> /dev/null || ( log_action_msg "Firmware busy! Please shutdown Klipper and then retry.\n" && exit 1 )
        sleep 1
        set +e
    fi

    log_daemon_msg "Stopping pru\n"

    (echo 'stop' > /sys/class/remoteproc/remoteproc1/state) 2> /dev/null
    (echo 'stop' > /sys/class/remoteproc/remoteproc2/state) 2> /dev/null

}

pru_start()
{

    while [ ! -f /sys/class/remoteproc/remoteproc1/state ] 
        do
            sleep 2
        done
    while [ ! -f /sys/class/remoteproc/remoteproc2/state ] 
        do
            sleep 2
        done

    if [ -c /dev/rpmsg_pru30 ]; then
        pru_stop
    else
        (echo 'stop' > /sys/class/remoteproc/remoteproc1/state) 2> /dev/null
        (echo 'stop' > /sys/class/remoteproc/remoteproc2/state) 2> /dev/null

    fi
    sleep 1

    log_daemon_msg "Starting pru\n"
    echo 'start' > /sys/class/remoteproc/remoteproc1/state
    echo 'start' > /sys/class/remoteproc/remoteproc2/state

    #log_daemon_msg "Loading ADC module"
    #echo 'BB-ADC' > /sys/devices/platform/bone_capemgr/slots
}


case "$1" in
start)
    pru_start
    ;;
stop)
    pru_stop
    ;;
restart)
    pru_stop
    pru_start
    ;;
reload|force-reload)
    log_daemon_msg "Reloading configuration not supported\n" $NAME
    log_end_msg 1
    ;;
status)
    log_daemon_msg "Status not supported\n" $NAME
    log_end_msg 1
    ;;
*)  log_action_msg "Usage: /usr/local/bin/klipper-pru {start|stop|status|restart|reload|force-reload}"
    exit 2
    ;;
esac
exit 0
