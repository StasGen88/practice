CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = permissionEditor
OBJS = permissionEditor.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

permissionEditor.o: permissionEditor.c
	$(CC) $(CFLAGS) -c 3.1.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
