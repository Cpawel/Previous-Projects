all: malloc

malloc: memgrind.c mymalloc.c mymalloc.h 

	gcc -Wall -g -o memgrind memgrind.c mymalloc.c mymalloc.h

clean: 
	rm -rf *.o malloc
