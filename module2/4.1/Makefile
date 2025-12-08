CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = phonebook
SRCS = main.c contact.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c contact.h
	$(CC) $(CFLAGS) -c main.c

contact.o: contact.c contact.h
	$(CC) $(CFLAGS) -c contact.c

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run