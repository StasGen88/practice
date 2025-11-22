CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = calculator
OBJS = main.o operations.o operation_manager.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

main.o: main.c operations.h operation_manager.h
	$(CC) $(CFLAGS) -c main.c

operations.o: operations.c operations.h
	$(CC) $(CFLAGS) -c operations.c

operation_manager.o: operation_manager.c operation_manager.h operations.h
	$(CC) $(CFLAGS) -c operation_manager.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean