CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pthread

all: producer consumer

producer: producer.c
	$(CC) $(CFLAGS) producer.c -o producer

consumer: consumer.c
	$(CC) $(CFLAGS) consumer.c -o consumer

clean:
	rm -f producer consumer data.txt
