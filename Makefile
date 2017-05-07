CC = gcc
CFLAGS = -Wall -g -O2 #-lmraa -lfann

default: main

extract: main.c 
	$(CC) $(CFLAGS)  main.c -o $@

clean:
	rm -f main *~
