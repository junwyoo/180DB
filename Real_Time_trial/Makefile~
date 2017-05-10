CC=gcc
CFLAGS=-Wall
LDFLAGS= -lmraa -lm
SOURCES=main.c classifiers.h constants.h training_data_new.h prompt_collect.h

EXECUTABBLES= main.c

all: main

main: #main.c # process.c classifiers.c constants.c training_data_new.c prompt_collect.c
	$(CC) $(CFLAGS) -o main -w $(SOURCES) $(LDFLAGS)
clean:

