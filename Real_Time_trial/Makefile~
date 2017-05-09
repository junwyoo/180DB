CC=gcc
CFLAGS=-Wall
LDFLAGS= -lmraa -lm
SOURCES=main.c do_not_use_file.c classifiers.c process.c process_testing_data.c  training_file.c

EXECUTABBLES= main.c

all: main

main: main.c process.c classifiers.c process_testing_data.c do_not_use_file.c training_file.c
	$(CC) $(CFLAGS) -o main -w $(SOURCES) $(LDFLAGS)
clean:

