CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = queue
OBJS = queue.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: clean