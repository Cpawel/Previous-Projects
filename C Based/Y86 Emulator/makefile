COMPILER = gcc
CCFLAGS = -Wall

all: y86emul

y86emul: y86emul.o
	$(COMPILER) -o y86emul y86emul.o

y86emul.o: y86emul.c
	$(COMPILER) $(CCFLAGS) -g -c y86emul.c

clean:
	rm -rf y86emul *.o
