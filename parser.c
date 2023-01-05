#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.c"
#include "parserinit.c"
#include "parser.h"
#define NUM_RULES 87
#define NUM_ACTIONS 48
#define NUM_GOTO 32
#define NUM_INSTANCES 300
#define TOTAL_TOKENS 50

struct StackItem {
	token_t instance;
	Token token;
	
} StackItem;


typedef enum type_t {
	INT,
	LONG,
	SHORT,
	CHAR,
	DOUBLE
} type_t;

typedef enum signed_t {
	SIGNED,
	UNSIGNED
} signed_t;



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
			printf("Token not found: %s NUM:  %d\n",tokens[tokenIndex]->token,tokens[tokenIndex]->tokenType);
			return 0;
		}
		printStack();
	}
	return -1;
}

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
