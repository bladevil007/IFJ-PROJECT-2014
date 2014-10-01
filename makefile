CFLAGS=-std=c99 -Wall -pedantic -g
BIN=project
CC=gcc
RM=rm -f

ALL: scaner.o main.o
	$(CC) $(CFLAGS) -o $(BIN) scaner.o main.o

clean:
	$(RM) *.o $(BIN)
