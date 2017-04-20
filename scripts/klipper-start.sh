#!/bin/sh
# System startup script for Klipper 3d-printer host code

### BEGIN INIT INFO
# Provides:          klipper
# Required-Start:    $local_fs
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Klipper daemon
# Description:       Starts the Klipper daemon.
### END INIT INFO

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
DESC="klipper daemon"
NAME="klipper"
DEFAULTS_FILE=/etc/default/klipper
PIDFILE=/var/run/klipper.pid

. /lib/lsb/init-functions

# Read defaults file
[ -r $DEFAULTS_FILE ] && . $DEFAULTS_FILE

case "$1" in
start)  log_daemon_msg "Starting klipper" $NAME
        start-stop-daemon --start --quiet --exec $KLIPPY_EXEC \
                          --background --pidfile $PIDFILE --make-pidfile \
                          --chuid $KLIPPY_USER --user $KLIPPY_USER \
                          -- $KLIPPY_ARGS
        log_end_msg $?
        ;;
stop)   log_daemon_msg "Stopping klipper" $NAME
        killproc -p $PIDFILE $KLIPPY_EXEC
        RETVAL=$?
        [ $RETVAL -eq 0 ] && [ -e "$PIDFILE" ] && rm -f $PIDFILE
        log_end_msg $RETVAL
        ;;
restart) log_daemon_msg "Restarting klipper" $NAME
        $0 stop
        $0 start
        ;;
reload|force-reload)
        log_daemon_msg "Reloading configuration not supported" $NAME
        log_end_msg 1
        ;;
status)
        status_of_proc -p $PIDFILE $KLIPPY_EXEC $NAME && exit 0 || exit $?
        ;;
*)      log_action_msg "Usage: /etc/init.d/klipper {start|stop|status|restart|reload|force-reload}"
        exit 2
        ;;
esac
exit 0
