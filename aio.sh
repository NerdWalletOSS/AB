#!/bin/bash
usage(){
	echo "Usage: $0 [-h|-b|-c|-p] -- program to build AB and pack the bin and dependencies" 
	echo "where:" 
	echo "  -h  shows this message" 
	echo "  -b  builds AB and places everything in the bin directory" 
	echo "  -o  Same as -b but doesn't install packages build" 
 	echo "  -c  Cleans out all the binary files" 
    echo "  -p  Builds a tarball in addition to building everything" 
	exit 1 ;
}

clean(){
	rm -rf bin
	rm -f bin.tar.gz
	make -C ./src clean
  make -C ./curl-7.51.0 clean
}

buildall(){
	sudo apt-get install gcc python python-pip -y
	sudo pip install pystatsd
	clean
	set -e
    build
}

build(){
	mkdir bin
	cd ./curl-7.51.0/
	./configure
	make
	cd ../
	mkdir ./bin/libs
	cd ./src
	make
	cp ab_httpd ../bin
	find ./ -name "*.so*" -exec cp {} ../bin/libs \;
	find ./ -name "*.lua" -exec cp {} ../bin \;
  set +e
}

while getopts "bochp" opt;
do
	case $opt in
		h)
      usage
			;;
		p)
			# echo "-f was triggered, Parameter: $OPTARG" >&2
			build
			tar -cvzf bin.tar.gz ./bin
			exit 0
      ;;
    b)
      buildall
      exit 0
      ;;
    o)
      build
      exit 0
      ;;
    c)
      clean
      exit 0
      ;;
    \?)
			echo "Invalid option: -$OPTARG" 1>&2
			exit 1
			;;
		:)
			echo "Option -$OPTARG requires an argument." 1>&2
			exit 1
			;;
	esac
done
usage
