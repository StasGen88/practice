CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = program
SOURCE = 3.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)


run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean run