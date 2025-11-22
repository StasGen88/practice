CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = 2.2
OBJS = 2.2.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

2.2.o: 2.2.c
	$(CC) $(CFLAGS) -c 2.2.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean