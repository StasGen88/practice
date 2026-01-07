CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lrt

TARGETS = progA progB

.PHONY: all clean

all: $(TARGETS)

progA: progA.c
	$(CC) $(CFLAGS) progA.c -o progA $(LDFLAGS)

progB: progB.c
	$(CC) $(CFLAGS) progB.c -o progB $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o
