CC=gcc
CFLAGS=-Wall
EXE=jcc
all: parser

parser: parser.o scanner.o parserinit.o
	$(CC) $(CFLAGS) -o $(EXE) parser.o

parser.o: parser.c scanner.c token.h
	$(CC) $(CFLAGS) -c parser.c

parserinit.o: parserinit.c
	$(CC) $(CFLAGS) -c parserinit.o

scanner.o: scanner.c dfa.h dfa.c token.h
	$(CC) $(CFLAGS) -c scanner.c

dfa.o: dfa.c dfa.h token.h
	$(CC) $(CFLAGS) -c dfa.c

clean:
	rm *.o $(EXE)
