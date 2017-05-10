CC = gcc
CFLAGS = -Wall -g -O2
LDFLAGS = -lmraa -lm
SOURCES = main.c training_data_new.c LSM9DS0.c

default: main

main: prompt_collect.c training_data_new.c LSM9DS0.c
	$(CC) $(LDFLAGS) $(CFLAGS) -o main $(SOURCES)

clean:
	rm -f main *~
