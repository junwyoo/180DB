CC = gcc
CFLAG = -Wall -g -O2

default: main producer consumer

main:
	$(CC) $(CFLAGS) -o main main.c functions.c

producer:
	$(CC) $(CFLAGS) -o producer producer.c

consumer:
	$(CC) $(CFLAGS) -o consumer consumer.c

clean:
	rm -rf main consumer producer *~
