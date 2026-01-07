CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

all: producer consumer

producer: producer.c
	$(CC) $(CFLAGS) producer.c -o producer

consumer: consumer.c
	$(CC) $(CFLAGS) consumer.c -o consumer

clean:
	rm -f producer consumer *.o
