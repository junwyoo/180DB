./producer &
PRODUCER_ID=$!
./consumer &
CONSUMER_ID=$!
read -p "Press any key to kill producer/consumer... " -n1 -s
kill $PRODUCER_ID $CONSUMER_ID
