#!/bin/bash

if [ $1 ] && [ $2 ]; then
		num_clients=$1
		username=$2
else
		echo "Usage: sh run_server <NUM_CLIENTS> <USERNAME>"
		exit 1
fi

portno=1000;

echo ""
echo ">>>> COMPILING <<<<"
make clean
make

echo ""
echo ">>>> RUNNING SERVER PROGRAM <<<<"
./server \
	$portno \
	$num_clients \
	$username