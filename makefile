CC=gcc
CFLAGS=-Wall
EXE=jcc

jcc: obj/main.o obj/parser.o obj/parserinit.o obj/scanner.o obj/dfa.o obj/semantics.o obj/ir.o obj/assembly.o
	$(CC) $(CFLAGS) -o $(EXE) obj/main.o -g

obj/main.o: main.c parser.c parserinit.c scanner.c semantics.c dfa.c ir.c token.h
	$(CC) $(CFLAGS) -c main.c -o obj/main.o

obj/assembly.o: assembly.c ir.c
	$(CC) $(CFLAGS) -c assembly.c -o obj/assembly.o

obj/ir.o: ir.c syntaxtree.h
	$(CC) $(CFLAGS) -c ir.c -o obj/ir.o

obj/semantics.o: semantics.c syntaxtree.h
	$(CC) $(CFLAGS) -c semantics.c -o obj/semantics.o

obj/parser.o: parser.c parserinit.c token.h syntaxtree.h
	$(CC) $(CFLAGS) -c parser.c -o obj/parser.o

obj/parserinit.o: parserinit.c parser.h token.h
	$(CC) $(CFLAGS) -c parserinit.c -o obj/parserinit.o

obj/scanner.o: scanner.c dfa.c token.h
	$(CC) $(CFLAGS) -c scanner.c -o obj/scanner.o

obj/dfa.o: dfa.c token.h
	$(CC) $(CFLAGS) -c dfa.c -o obj/dfa.o

clean:
	rm obj/*.o $(EXE)

