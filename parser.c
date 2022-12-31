#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.c"
#define NUM_RULES 15
#define NUM_ACTIONS 9
#define NUM_GOTO 7
#define NUM_INSTANCES 24
#define TOTAL_TOKENS 26

typedef enum Step{
	STEP_ERROR,
	STEP_REDUCE,
	STEP_SHIFT,
	STEP_ACCEPT
}Step;

struct Action {
	Step step;
	int instance;//or rule in grammar
};

struct Rule {
	token_t var;
	token_t *symbols;//tokens and variables
	int length;
} Rule;

struct Instance {
	struct Action actions[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

struct Instance instances[NUM_INSTANCES];

struct Token *instanceTokens[NUM_INSTANCES];//instance tokens (for stack)
struct Token *varTokens[NUM_GOTO];//variable (nonterminal) tokens (for stack)
extern struct Token **tokens;//input of tokens
extern int numTokens;
int tokenIndex = 0;//for reading input

struct Rule rules[NUM_RULES];//grammar

struct Token **stack;
int stackCapacity = 0;
int stackTopPointer = -1;//points to top of the stack

void addInstanceAction(int instanceNum, struct Action action, int actionNum) {
	instances[instanceNum].actions[actionNum].step = action.step;
	instances[instanceNum].actions[actionNum].instance = action.instance;
}
void addInstanceGoto(int instanceNum, int gotoNum, int gotoInstance) {
	instances[instanceNum].gotoAction[gotoNum] = gotoInstance;
}

void initializeInstances() {
	//EXAMPLE
	/*struct Action action0id = { STEP_SHIFT, 5 };
	struct Action action0int = { STEP_SHIFT, 6 };
	struct Action action0long = { STEP_SHIFT, 7 };
	addInstanceAction(0,action0id,1);
	addInstanceAction(0,action0int,6);
	addInstanceAction(0,action0long,7);
	addInstanceGoto(0,0,1);
	addInstanceGoto(0,1,2);
	addInstanceGoto(0,2,3);
	addInstanceGoto(0,3,4);
	*/


}

void push(Token *token) {
	stack = realloc(stack,sizeof(Token) * (stackTopPointer + 2));
	stack[++stackTopPointer] = token;
}
void pop() {
	stack = realloc(stack,sizeof(Token) * (stackTopPointer--));
}

Token *getTopOfStack() {
	return stack[stackTopPointer];
}

Token *getSecondTopOfStack() {
	return stack[stackTopPointer-1];
}

void freeStack() {
	free(stack);
}

void printToken(token_t token);

void createInstanceAndVarTokens() {
	for (int i = 0; i < NUM_INSTANCES; i++) {
		instanceTokens[i] = malloc(sizeof(Token));
		instanceTokens[i]->tokenType = i;
	}
	for (int i = 0; i < NUM_GOTO; i++) {
		varTokens[i] = malloc(sizeof(Token));
		varTokens[i]->tokenType = NUM_INSTANCES + TOTAL_TOKENS + i; 
	}
}

void freeInstanceAndVarTokens() {
	for (int i = 0; i < NUM_INSTANCES; i++) {
		free(instanceTokens[i]);
	}
	for (int i = 0; i < NUM_GOTO; i++) {
		free(varTokens[i]);
	}
}
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
		case TOKEN_SIGNED:
			str = "signed";
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
	rules[item].symbols = malloc(sizeof(token_t) * length);
	for (int i = 0; i < length; i++) {
		rules[item].symbols[i] = symbol[i];
	}
}

void printRule(struct Rule rule) {
	printToken(rule.var);
	printf("->");
	for (int i = 0; i < rule.length; i++) {
		printf(" ");
		printToken(rule.symbols[i]);
	}
	printf("\n");
}

void printRules() {
	printf("----------GRAMMAR-----------\n");
	for (int i = 0; i < NUM_RULES; i++) {
		printRule(rules[i]);
	}
}

void shift(Token *token, Token *instance) {
	printf("Shift \n");
	push(token);
	push(instance);
	tokenIndex++;
}
void printStack();

void reduce(int rule) {
	printf("Reduce by %d\n",rule);
	for (int i = 0; i < rules[rule].length * 2; i++) {
		pop();
	}
	push(varTokens[rules[rule].var-(TOTAL_TOKENS+NUM_INSTANCES)]);
	push(instanceTokens[instances[getSecondTopOfStack()->tokenType].gotoAction[getTopOfStack()->tokenType-(TOTAL_TOKENS+NUM_INSTANCES)]]);
}

void printStack() {
	for (int i = 0; i <= stackTopPointer; i++) {
		printf("%d ", stack[i]->tokenType);
	}
	printf("\n");
}

int parse() {
	push(instanceTokens[0]);
	printf("Beggining parser...\n");
	while(1) {
		//ignoring whitespace
		printf("Reading token %d: ",tokenIndex);
		printToken(tokens[tokenIndex]->tokenType);
		printf("\n");
		if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			printf("Skipping whitespace!\n");
			tokenIndex++;
			continue;
		}
		//reading shift
		token_t token = getTopOfStack()->tokenType;
		int actionIndex = tokens[tokenIndex]->tokenType-NUM_INSTANCES;
		Step step = instances[token].actions[actionIndex].step;
		//printf("Instance: %d | ActionIndex: %d | step: %d\n",token,actionIndex,step);
		if (step == STEP_ACCEPT) {
			return 1;
		}
		else if (step == STEP_SHIFT) {
			shift(tokens[tokenIndex],instanceTokens[instances[token].actions[actionIndex].instance]);
		}
		else if (step == STEP_REDUCE) {
			reduce(instances[token].actions[actionIndex].instance);
		}
		else {
			printf("Token not found\n");
			return 0;
		}
		printStack();
	}
	return -1;
}

void freeRules() {
	for (int i = 0; i < NUM_RULES; i++) {
		free(rules[i].symbols);
	}
}
/*
 * initializing grammar
 */
void initializeRules() {
	/*EXAMPLE
	token_t rule0[1] = { VAR_L };
	addRule(0, VAR_P, 1, rule0);*/

}
struct Test {
	int x;
};

struct Test test;

int main(int argc, char *argv[]) {
	scanner(argc, argv);
	test.x = 5;
	initializeRules();
	createInstanceAndVarTokens();
	//printRules();

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
	//freeTokens();seg fault
	freeStack();
}
