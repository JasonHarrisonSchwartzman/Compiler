CC=gcc
CFLAGS=-Wall
EXE=jcc

all: main.o parser.o parserinit.o scanner.o semantics.o jcc.entitlements
	$(CC) $(CFLAGS) -o $(EXE) main.o -g
	codesign -s - -f --entitlements jcc.entitlements jcc
jcc.entitlements:
	/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" jcc.entitlements
main.o: main.c parser.c parserinit.c scanner.c semantics.c
	$(CC) $(CFLAGS) -c main.c

semantics.o: semantics.c syntaxtree.h
	$(CC) $(CLFAGS) -c semantics.c

parser.o: parser.c parserinit.c token.h syntaxtree.h
	$(CC) $(CFLAGS) -c parser.c

parserinit.o: parserinit.c parser.h token.h
	$(CC) $(CFLAGS) -c parserinit.c

scanner.o: scanner.c dfa.c token.h
	$(CC) $(CFLAGS) -c scanner.c

dfa.o: dfa.c dfa.h token.h
	$(CC) $(CFLAGS) -c dfa.c

clean:
	rm *.o $(EXE)
