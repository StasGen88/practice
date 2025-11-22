CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = src/contacts
SRCDIR = src

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/contact.c $(SRCDIR)/phone.c
OBJECTS = $(SRCDIR)/main.o $(SRCDIR)/contact.o $(SRCDIR)/phone.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Сборка программы..."
	$(CC) $(OBJECTS) -o $(TARGET)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild