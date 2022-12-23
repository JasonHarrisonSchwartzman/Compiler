CC=gcc
CFLAGS=-Wall
EXE=jcc
all: parser

parser: parser.o scanner.o
	$(CC) $(CFLAGS) -o $(EXE) parser.o

parser.o: parser.c scanner.c token.h

scanner.o: scanner.c dfa.h dfa.c
	$(CC) $(CFLAGS) -c scanner.c

dfa.o: dfa.c dfa.h
	$(CC) $(CFLAGS) -c dfa.c

clean:
	rm *.o $(EXE)
