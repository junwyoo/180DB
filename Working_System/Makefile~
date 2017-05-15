CC=gcc
CFLAGS=-Wall
LDFLAGS= -lmraa -lm -lfann
SOURCES=producer.c constants.c LSM9DS0.c constants.c classifers.c main.c features.c strides.c training_file.c turnig_file.c prompt_collect.c training_data_new.c test_neural_network.c train_neural_network.c

EXECUTABBLES= producer.c consumer.c main.c

all: producer consumer main

producer: producer.c LSM9DS0.c  constants.c constants.h 
	$(CC) $(CFLAGS) -o producer -w producer.c LSM9DS0.c constants.c classifiers.c  $(LDFLAGS)

consumer: consumer.c constants.c constants.h classifiers.c strides.c features.c test_neural_network.c
	$(CC) $(CFLAGS) -o consumer -w consumer.c constants.c classifiers.c strides.c features.c test_neural_network.c $(LDFLAGS)

main: main.c LSM9DS0.c constants.c classifiers.c features.c strides.c training_file.c turning_file.c prompt_collect.c training_data_new.c train_neural_network.c
	$(CC) $(CFLAGS) -o main -w main.c LSM9DS0.c constants.c classifiers.c features.c strides.c training_file.c turning_file.c prompt_collect.c training_data_new.c train_neural_network.c $(LDFLAGS)


clean:
	rm -f producer
	rm -f consumer
	rm -f main


