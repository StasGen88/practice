phonebook: main.o tree.o
	gcc -o phonebook main.o tree.o

main.o: main.c tree.h
	gcc -c main.c

tree.o: tree.c tree.h
	gcc -c tree.c

clean:
	rm -f *.o phonebook