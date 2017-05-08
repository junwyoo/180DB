CC = gcc
CFLAGS = -Wall -g -O2
LDFLAGS = -lmraa -lm
SOURCES = main.c training_data_new.c

default: main

main: main.c training_data_new.c
	$(CC) $(CFLAGS) -o $@ -w $(SOURCES) $(LDFLAGS)

clean:
	rm -f main *~
