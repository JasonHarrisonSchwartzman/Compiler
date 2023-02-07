#include "token.h"
#include "parser.h"
#include "scanner.c"
#include "parser.c"
#include "semantics.c"
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
	checkAll();

	freeRules();
	freeInstanceAndVarTokens();
	// //freeTokens();seg fault
}

