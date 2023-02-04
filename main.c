#include "token.h"
#include "parser.h"
#include "scanner.c"
#include "parser.c"
#include "semantics.c"
#include <stdio.h>

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
	}
	else {
		printf("ODD PARSER FAILURE\n");
	}
	checkAll();
	/*signed long x = 1865000000000000;
	unsigned long y = 140000000000000;
	signed long z = 9223372036854775807;
	unsigned long w = 1;
	printf("Test:%ld\n",z+w);*/

	freeRules();
	freeInstanceAndVarTokens();
	// //freeTokens();seg fault
}

