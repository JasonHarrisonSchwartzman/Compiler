#include "parser.c"
#include "parserinit.c"
#include "token.h"
#include "parser.h"
#include "scanner.c"

int main(int argc, char *argv[]) {
	scanner(argc, argv);
	initializeRules();
	createInstanceAndVarTokens();
	printRules();
	initializeRules();
	createInstanceAndVarTokens();
	printRules();
	initializeInstances();
	printTokens();
	int result = parse();
	if (result == 1) {
		printf("SUCCESS!\n");
	}
	else if (result == 0) {
	 	printf("FAILURE\n");
	}
	else {
		printf("ODD FAILURE\n");
	}


	freeRules();
	freeInstanceAndVarTokens();
	// //freeTokens();seg fault
	freeStack();
}

