#!/bin/sh

start() {
	ps -e | grep perf | grep -v perf.sh | grep -v grep
	if [ $? -ne 0 ]; then
		echo "start ..."
		./perfd_sync &
		# sleep 0.1s
		./perfc_sync
	else
		echo "already runing....."
		# ./clic
	fi
}

stop() {
	echo "stop ..."
	# ps -ef | grep test | awk '{ print $3 } ' | xargs kill -9
	pkill -9 perfd_sync
}

restart() {
	stop
	start
}

if [ "$1" = "start" ]; then
	start
elif [ "$1" = "stop" ]; then
	stop
elif [ "$1" = "restart" ]; then
	restart
else
	restart
	#echo "Please make sure the option is start/stop/restart."
fi
