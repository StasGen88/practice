CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = ip_simulator
SOURCES = seti.c

GATEWAY ?= 192.168.1.1
MASK ?= 255.255.255.0
PACKETS ?= 10

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET) $(GATEWAY) $(MASK) $(PACKETS)

clean:
	rm -f $(TARGET)

.PHONY: run  clean