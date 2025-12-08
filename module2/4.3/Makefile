CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = tree
OBJS = tree.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

tree.o: tree.c
	$(CC) $(CFLAGS) -c tree.c

clean:
	rm -f $(OBJS) $(TARGET)
.PHONY: clean