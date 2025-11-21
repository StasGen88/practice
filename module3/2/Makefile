CC = gcc
CFLAGS = -Wall -Wextra -std=c99

PROGRAMS = sum 2 maxnumber

all: $(PROGRAMS)

sum: sum.o
	$(CC) $(CFLAGS) -o sum sum.o

2: 2.o
	$(CC) $(CFLAGS) -o 2 2.o

maxnumber: maxnumber.o
	$(CC) $(CFLAGS) -o maxnumber maxnumber.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f *.o $(PROGRAMS)

rebuild: clean all

.PHONY: all clean rebuild