CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = program
SOURCE = 1.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)


run: $(TARGET)
	./$(TARGET) 10 hello 25.5 world 30 test 15.75

clean:
	rm -f $(TARGET)

.PHONY: clean run