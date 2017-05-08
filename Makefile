CC = gcc
CFLAGS = -Wall -g -O2 #-lmraa -lfann

default: main

main: main.c 
	$(CC) $(CFLAGS)  main.c training_data_new.c -o $@

clean:
	rm -f main *~
