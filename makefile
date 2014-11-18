CFLAGS=-std=c99 -lm -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: scaner.o parser.o string.o stack.o precedent.o main.o
	$(CC) $(CFLAGS) -o $(BIN) scaner.o -lm parser.o stack.o string.o precedent.o main.o

clean:
	$(RM) *.o $(BIN)
