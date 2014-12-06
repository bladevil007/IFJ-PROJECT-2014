CFLAGS=-std=c99 -lm -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: scaner.o parser.o string.o stack.o stack_ramec.o precedent.o ial.o codegenerate.o interpreter.o  main.o
	$(CC) $(CFLAGS) -o $(BIN) scaner.o -lm parser.o stack.o string.o ial.o precedent.o codegenerate.o stack_ramec.o interpreter.o main.o

clean:
	$(RM) *.o $(BIN)
