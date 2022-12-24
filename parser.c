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

struct Rule {
	token_t var;
	token_t *symbol;//tokens and variables
	int length;
} Rule;

struct Instance {
	struct Action action[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

extern struct Token **tokens;
extern int numTokens;
int tokenIndex = 0;

struct Rule rules[NUM_RULES];

struct Token **stack;
int stackCapacity = 1;
int stackTopPointer = 0;

void printToken(token_t token) {
	char *str;
	switch(token) {
		case TOKEN_NUM:
			str = "num";
			break;
		case TOKEN_ID:
			str = "id";
			break;
		case TOKEN_PLUS:
			str = "+";
			break;
		case TOKEN_MINUS:
			str = "-";
			break;
		case TOKEN_ASSIGN:
			str = "=";
			break;
		case TOKEN_SEMICOLON:
			str = ";";
			break;
		case TOKEN_INT:
			str = "int";
			break;
		case TOKEN_LONG:
			str = "long";
			break;
		case TOKEN_DOLLAR:
			str = "$";
			break;
		case TOKEN_EOF:
			str = "eof";
			break;
		case TOKEN_WHITESPACE:
			str = "[SPACE]";
			break;
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

void addRule(int item, token_t var, int length, token_t symbol[length]) {
	rules[item].var = var;
	rules[item].length = length;
	for (int i = 0; i < length; i++) {
		rules[item].symbol[i] = symbol[i];
	}
}

void printRule(struct Rule rule) {
	printToken(rule.var);
	printf("->");
	for (int i = 0; i < rule.length; i++) {
		printf(" ");
		printToken(rule.symbol[i]);
	}
	printf("\n");
}

void printRules() {
	for (int i = 0; i < NUM_RULES; i++) {
		printRule(rules[i]);
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
