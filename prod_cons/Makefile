CC = gcc
CFLAGS = -g -Wall -O2 #-lmraa -lfann

default: consumer producer

consumer: consumer.c
	$(CC) $(CFLAGS) consumer.c -o $@

producer: producer.c
	$(CC) $(CFLAGS) producer.c -o $@

clean:
	rm -f consumer producer test_*.csv
