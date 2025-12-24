CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99
AR = ar
ARFLAGS = rcs

TARGET = contact_app
LIBRARY = libcontact.a

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRCS = $(SRC_DIR)/contact.c
OBJS = $(BUILD_DIR)/contact.o

.PHONY: all clean

all: $(LIBRARY) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(TARGET): main.c $(LIBRARY)
	$(CC) $(CFLAGS) main.c -L. -lcontact -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIBRARY) $(TARGET)

run: $(TARGET)
	./$(TARGET)