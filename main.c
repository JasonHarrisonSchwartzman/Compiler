#include "token.h"
#include "parser.h"
#include "scanner.c"
#include "parser.c"
#include "semantics.c"
#include "ir.c"
#include "assembly.c"
#include <stdio.h>

/**
 * Putting everything together the main file is the command and control of the various parts of the compiler.
 */

int main(int argc, char *argv[]) {
	scanner(argc, argv);
	if (!scannerPass) {
		printf("SCANNER FAILURE.\n");
		exit(1);
	}
	printTokens();
	initializeRules();
	//printRules();
	createInstanceAndVarTokens();
	initializeInstances();
	int result = parse();
	if (result == 1) {
		printf("SUCCESS!\n");
	}
	else if (result == 0) {
	 	printf("PARSER FAILURE.\n");
		exit(1);
	}
	else {
		printf("ODD PARSER FAILURE\n");
		exit(1);
	}
	int semanticResult = checkAll();
	if (semanticResult == 1) {
		printf("SEMANTIC SUCCESS\n");
	}
	else {
		printf("SEMANTIC FAILURE\n");
		exit(1);
	}
	createIR();
	printf("Quads before optimizations\n");
	printQuads();
	printf("IR finished\n");
	optimize();
	printQuads();

	
	generateCode();

	freeRules();
	freeInstanceAndVarTokens();

	//writing to executable
	FILE *f = fopen(concatenateStrings(argv[2],".s"),"w");
    printData(f);
    printCode(f);
    fclose(f);
	char *assembler = concatenateStrings("as -g -o ", argv[2]);
	assembler = concatenateStrings(assembler, ".o ");
	assembler = concatenateStrings(assembler, argv[2]);
	assembler = concatenateStrings(assembler, ".s");
	system(assembler);
	char *linker = concatenateStrings("ld -o ",argv[2]);
	linker = concatenateStrings(linker, " ");
	linker = concatenateStrings(linker, argv[2]);
	linker = concatenateStrings(linker, ".o");
	system(linker);
	// //freeTokens();seg fault
}

