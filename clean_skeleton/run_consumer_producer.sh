#!/bin/bash

if [ $1 ]; then
		username=$1
else
		exit 1
fi

./producer $username &
PRODUCER_ID=$!
./consumer $username &
CONSUMER_ID=$!
echo "Producer PID:" $PRODUCER_ID "Consumer PID:" $CONSUMER_ID
read -p "Press any key to kill process..."$'\n' -n1 -s
kill -9 $PRODUCER_ID $CONSUMER_ID
