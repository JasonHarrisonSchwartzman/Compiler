#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "parserinit.c"
#include "parser.h"

struct StackItem {
	token_t instance;
	Token token;
	token_t var;
	void *ptr;
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

void printStack() {
	for (int i = 0; i <= stackTopPointer; i++) {
		printf("%d ", stack[i]->tokenType);
	}
	printf("\n");
}

void freeStack() {
	free(stack);
}



void shift(Token *token, Token *instance) {
	printf("Shift \n");
	push(token);
	push(instance);
	tokenIndex++;
}

void reduce(int rule) {
	printf("Reduce by %d\n",rule);
	for (int i = 0; i < rules[rule].length * 2; i++) {
		pop();
	}
	push(varTokens[rules[rule].var-(TOTAL_TOKENS+NUM_INSTANCES)]);
	push(instanceTokens[instances[getSecondTopOfStack()->tokenType].gotoAction[getTopOfStack()->tokenType-(TOTAL_TOKENS+NUM_INSTANCES)]]);
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
