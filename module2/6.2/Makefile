CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99 -fPIC
LDFLAGS = -shared

TARGET = contact_app
LIBRARY = libcontact.so

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRCS = $(SRC_DIR)/contact.c
OBJS = $(BUILD_DIR)/contact.o

.PHONY: all clean run

all: $(LIBRARY) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET): main.c $(LIBRARY)
	$(CC) $(CFLAGS) main.c -L. -lcontact -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIBRARY) $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./$(TARGET)