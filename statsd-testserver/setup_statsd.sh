#!/bin/bash
usage(){
	echo "Usage: $0 [-h|-i|-r|-s] -- program to manage statsd"
	echo "where:"
	echo "  -h  shows this message"
	echo "  -i  Installs statsd"
	echo "  -r  Runs statsd"
	echo "  -s  Shuts down statsd and cleans logs"
	echo "  -w  Watch statsd input"
	# echo "  -r  Builds and run AB with all other auxillary service"
	exit 1 ;
}

install_statsd(){
	sudo pip install pystatsd
}

run_statsd(){
	python -c 'import pystatsd; pystatsd.Server().serve()' & 
}

stop_statsd(){
	ps auxwwf | grep "statsd" | awk '{print $2}'  | sudo xargs kill -9
}

watch_statsd(){
	sudo ngrep -qd any . udp dst port 8125
}

while getopts "hirsiw" opt;
do
	case $opt in
		h)
			usage
			;;
		i)
			install_statsd
			exit 0
			;;
			r)
			run_statsd
			exit 0
			;;
			s)
			stop_statsd
			exit 0
			;;
			w)
			watch_statsd
			exit 0
			;;

	esac
done
usage
