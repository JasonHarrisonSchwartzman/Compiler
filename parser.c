#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.c"
#define NUM_RULES 14
#define NUM_ACTIONS 9
#define NUM_GOTO 7

typedef enum Step{
	STEP_SHIFT,
	STEP_REDUCE,
	STEP_ERROR
}Step;

struct Action {
	Step step;
	int instance;
};

union Symbol {
	var_t var;
	token_t token;
};

struct Rules {
	var_t var;
	union Symbol *symbol;
	int length;
} Rules;

struct Instance {
	struct Action action[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

extern struct Token **tokens;
extern int numTokens;
int tokenIndex = 0;

struct Rules rules[NUM_RULES];

struct Token **stack;
int stackCapacity = 1;
int stackTopPointer = 0;

void printVar(var_t var) {
	char *str;
	switch(var) {
		case VAR_P:
			str = "P";
			break;
		case VAR_L:
			str = "L";
			break;
		case VAR_D:
			str = "D";
			break;
		case VAR_S:
			str = "S";
			break;
		case VAR_T:
			str = "T";
			break;
		case VAR_E:
			str = "E";
			break;
		case VAR_O:
			str = "O";
			break;
		case VAR_X:
			str = "X";
			break;
		default:
			str = "ERROR VAR NOT FOUND";
			break;
	}
	printf("%s",str);
}

void addRule(int item, var_t var, int length, union Symbol symbol[length]) {
	rules[item].var = var;
	rules[item].length = length;
	for (int i = 0; i < length; i++) {
		rules[item].symbol[i] = symbol[i];
	}
}

void printRule(union Symbol symbol) {
	//char *str;
	/*switch (symbol) {

	}*/
}

void printRules() {
	for (int i = 0; i < NUM_RULES; i++) {
		printVar(rules[i].var);
		printf("->");
		for (int j = 0; j < rules[i].length; j++) {
			printf(" ");
			printRule(rules[i].symbol[j]);
		}
	}
}
int parse() {
	for (int i = 0; i < numTokens; i++) {
		
	}
	return -1;
}

int main(int argc, char *argv[]) {
	scanner(argc, argv);	

}
