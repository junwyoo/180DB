#!/bin/bash

if [ $1 ] && [ $2 ]; then
		server_ip_address=$1
		username=$2
else
		echo "Usage: sh run_client <SERVER_IP_ADDRESS> <USERNAME>"
		exit 1
fi

portno=1000;

echo ""
echo ">>>> COMPILING <<<<"
make clean
make

echo ""
echo ">>>> RUNNING CLIENT PROGRAM <<<<"
./client \
	$server_ip_address \
	$portno \
	$username