#!/bin/bash

. /etc/rc.d/init.d/functions

TARGET="./pinocchio"

DAEMON=./PINOCCHIO

RETVAL=0
prog="PINOCCHIO"
PID_FILE="./pinocchio.pid"

start()
{
	echo -n $"Starting $prog: "

	$DAEMON >/dev/null 2>&1 & 
	success || failure
	RETVALE=$?
	echo
}

stop()
{
	echo -n $"Stopping $prog: "
	killproc $DAEMON
	RETVAL=$?
	echo
}

restart()
{
	stop
	start
}


case "$1" in
	start)
		start
		;;
	stop)
		stop
		;;
	restart)
		stop
		start
		;;
	status)
		status $DAEMON
		RETVAL=$?
		;;
	*)
		echo $"Usage: $0 {start|stop|restart|status}"
		exit 3
esac

exit $RETVAL
