CC=gcc
CFLAGS=-Wall
EXE=jcc

all: main.o parser.o parserinit.o scanner.o
	$(CC) $(CFLAGS) -o $(EXE) main.o

main.o: main.c parser.c parserinit.c scanner.c
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c parserinit.c token.h
	$(CC) $(CFLAGS) -c parser.c

parserinit.o: parserinit.c parser.h token.h
	$(CC) $(CFLAGS) -c parserinit.c

scanner.o: scanner.c dfa.c token.h
	$(CC) $(CFLAGS) -c scanner.c

dfa.o: dfa.c dfa.h token.h
	$(CC) $(CFLAGS) -c dfa.c

clean:
	rm *.o $(EXE)
