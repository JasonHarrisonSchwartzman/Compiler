#include "token.h"
#include "parser.h"
#include "scanner.c"
#include "parser.c"
#include "semantics.h"
#include "semantics.c"
#include <stdio.h>

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
	checkAll();


	freeRules();
	freeInstanceAndVarTokens();
	// //freeTokens();seg fault
}

