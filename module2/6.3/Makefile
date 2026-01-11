CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -ldl

LIBDIR = libs
LIBS = add subtract multiply divide

LIBSO = $(addprefix $(LIBDIR)/, $(addsuffix .so, $(LIBS)))

all: $(LIBSO) calculator

calculator: main.o operation_manager.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: main.c operation_manager.h
	$(CC) $(CFLAGS) -c main.c

operation_manager.o: operation_manager.c operation_manager.h
	$(CC) $(CFLAGS) -c operation_manager.c

$(LIBDIR)/%.so: $(LIBDIR)/%.c
	$(CC) $(CFLAGS) -shared $< -o $@

clean:
	rm -f *.o calculator $(LIBDIR)/*.so
