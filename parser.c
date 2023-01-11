#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "syntaxtree.h"
#include "parserinit.c"
#include "parser.h"

void printStack2() {
	for (int i = 0; i <= stack2TopPointer; i++) {
		printf("%d ", stack2[i]->instance);
		if (stack2[i]->token != NULL) {
			printf("%d ",stack2[i]->token->tokenType);
		}
		if (stack2[i]->var != 0) {
			printf("%d ",stack2[i]->var);
		}
	}
	printf("\n");
}

void push2(token_t instance, Token *token, token_t var) {
	if (var != -1) {
		stack2[stack2TopPointer]->var = var;
	}
	if (token != NULL) {
		stack2[stack2TopPointer]->token = token;
	}
	if (instance != -1) {
		stack2 = realloc(stack2,sizeof(void*) * (stack2TopPointer + 2));
		stack2[++stack2TopPointer] = calloc(1,sizeof(struct StackItem));
		stack2[stack2TopPointer]->instance = instance;
	}
}

void pop2() {
	if ((stack2[stack2TopPointer]->token == NULL) && (stack2[stack2TopPointer]->var == 0)) {
		stack2 = realloc(stack2,sizeof(StackItem) * (stack2TopPointer--));
	}
	else {
		stack2[stack2TopPointer]->token = NULL;
		stack2[stack2TopPointer]->var = 0;
	}
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

void printStack() {
	for (int i = 0; i <= stackTopPointer; i++) {
		printf("%d ", stack[i]->tokenType);
	}
	printf("\n");
}

void freeStack() {
	free(stack);
}

void shift2(token_t instance, Token *token, token_t var) {
	printf("Shift \n");
	push2(instance, token, var);
	tokenIndex++;
}

void reduce2(int rule) {
	printf("Reduce by %d\n",rule);
	for (int i = 0; i < rules[rule].length * 2; i++) {
		pop2();
	}
	token_t var = varTokens[rules[rule].var-(TOTAL_TOKENS+NUM_INSTANCES)]->tokenType;
	token_t instance = instanceTokens[instances[stack2[stack2TopPointer]->instance].gotoAction[var-(TOTAL_TOKENS+NUM_INSTANCES)]]->tokenType;
	push2(instance,NULL,var);
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

int parse2() {
	push2(0,NULL,-1);
	printStack2();
	while(1) {
		printf("Reading token %d: ",tokenIndex);
		printToken(tokens[tokenIndex]->tokenType);
		printf("\n");
		if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			printf("Skipping whitespace!\n");
			tokenIndex++;
			continue;
		}
		token_t state = stack2[stack2TopPointer]->instance;
		int actionIndex = tokens[tokenIndex]->tokenType-NUM_INSTANCES;
		Step step = instances[state].actions[actionIndex].step;
		if (step == STEP_ACCEPT) {
			return 1;
		}
		else if (step == STEP_SHIFT) {
			shift2(instanceTokens[instances[state].actions[actionIndex].instance]->tokenType, tokens[tokenIndex],-1);
		}
		else if (step == STEP_REDUCE) {
			reduce2(instances[state].actions[actionIndex].instance);
		}
		else {
			printf("Token not found %s NUM: %d Token type: ",tokens[tokenIndex]->token,tokens[tokenIndex]->tokenType);
			printToken(tokens[tokenIndex]->tokenType);
			printf("\n");
			return 0;
		}
		printStack2();
	}
	return -1;
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
			printf("Token not found: %s NUM:  %d Token type: ",tokens[tokenIndex]->token,tokens[tokenIndex]->tokenType);
			printToken(tokens[tokenIndex]->tokenType);
			printf("\n");
			return 0;
		}
		printStack();
	}
	return -1;
}
