CFLAGS = -Wall -g -O2
LDFLAGS = -lmraa -lm
SOURCES = main.c training_data_new.c LSM9DS0.c

default: main

main: main.c training_data_new.c LSM9DS0.c
	$(CC) $(LDFLAGS) $(CFLAGS) -o main -w $(SOURCES)

clean:
	rm -f main *~
