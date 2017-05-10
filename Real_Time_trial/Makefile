CC=gcc
CFLAGS=-Wall
LDFLAGS= -lmraa -lm
SOURCES=main.c classifiers.c constants.c training_data_new.c prompt_collect.c test_neural_network.c train_neural_network.c

all: main

main: #main.c process.c classifiers.c constants.c training_data_new.c prompt_collect.c
	$(CC) $(CFLAGS) -o main -w $(SOURCES) $(LDFLAGS)
clean:

