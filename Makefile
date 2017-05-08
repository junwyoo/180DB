CC = gcc
<<<<<<< HEAD
CFLAGS = -Wall -g -O2 
=======
CFLAGS = -Wall -g -O2
LDFLAGS = -lmraa -lm
SOURCES = main.c training_data_new.c
>>>>>>> 87f8cf04827ab18e6d0ea7d62e8ad11eb95beb32

default: main

main: main.c training_data_new.c
	$(CC) $(CFLAGS) -o $@ -w $(SOURCES) $(LDFLAGS)

clean:
	rm -f main *~
