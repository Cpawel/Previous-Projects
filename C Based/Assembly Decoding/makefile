CC = gcc
CCFLAGS = -Wall -o


all: mystery formula

mystery: mystery.c mystery.h
	$(CC) $(CCFLAGS) mystery mystery.c mystery.h

formula: formula.c nCr.s nCr.h
	$(CC) $(CCFLAGS) formula formula.c nCr.s nCr.h

clean:
	 rm mystery formula

