#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "syntaxtree.h"
#include "parserinit.c"
#include "parser.h"

void printStack() {
	for (int i = 0; i <= stackTopPointer; i++) {
		printf("%d ", stack[i]->instance);
		if (stack[i]->token != NULL) {
			printf("%d ",stack[i]->token->tokenType);
		}
		if (stack[i]->var != 0) {
			printf("%d ",stack[i]->var);
		}
	}
	printf("\n");
}

void push(token_t instance, Token *token, token_t var, void *ptr) {
	if (var != -1) {
		stack[stackTopPointer]->var = var;
	}
	if (token != NULL) {
		stack[stackTopPointer]->token = token;
	}
	if (instance != -1) {
		stack = realloc(stack,sizeof(void*) * (stackTopPointer + 2));
		stack[++stackTopPointer] = calloc(1,sizeof(struct StackItem));
		stack[stackTopPointer]->instance = instance;
	}
	stack[stackTopPointer]->ptr = ptr;
}

void *pop() {
	if ((stack[stackTopPointer]->token == NULL) && (stack[stackTopPointer]->var == 0)) {
		stack = realloc(stack,sizeof(StackItem) * (stackTopPointer--));
		return NULL;
	}
	else {
		stack[stackTopPointer]->token = NULL;
		stack[stackTopPointer]->var = 0;
		return stack[stackTopPointer]->ptr;
	}
}

/*void push(Token *token) {
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
}*/


void shift(token_t instance, Token *token, token_t var, void *ptr) {
	printf("Shift \n");
	push(instance, token, var, token);
	tokenIndex++;
}

void *callSemanicRule(void *param[], int rule) {
	switch (rule) {
		case 0:
			break;
		case 1: 
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5: 
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 19:
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		case 23:
			break;
		case 24:
			break;
		case 25:
			break;
		case 26:
			break;
		case 27:
			break;
		case 28:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 32:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 36:
			break;
		case 37:
			break;
		case 38:
			break;
		case 39:
			break;
		case 40:
			break;
		case 41:
			break;
		case 42:
			break;
		case 43:
			break;
		case 44:
			break;
		case 45:
			break;
		case 46:
			break;
		case 47:
			break;
		case 48:
			break;
		case 49:
			break;
		case 50:
			break;
		case 51:
			break;
		case 52:
			break;
		case 53:
			break;
		case 54:
			break;
		case 55:
			break;
		case 56:
			break;
		case 57:
			break;
		case 58:
			break;
		case 59:
			break;
		case 60:
			break;
		case 61:
			break;
		case 62:
			break;
		case 63:
			break;
		case 64:
			break;
		case 65:
			break;
		case 66:
			break;
		case 67:
			break;
		case 68:
			break;
		case 69:
			break;
		case 70:
			break;
		case 71:
			break;
		case 72:
			break;
		case 73:
			break;
		case 74:
			break;
		case 75:
			break;
		case 76:
			break;
		case 77:
			break;
		case 78:
			break;
		case 79:
			break;
		case 80:
			break;
		case 81:
			break;
		case 82:
			break;
		case 83:
			break;
		case 84:
			break;
		case 85:
			break;
		case 86:
			break;
		case 87:
			break;
		case 88:
			break;
		case 89:
			break;
		default:
			printf("Rule not found ERROR\n");
			return NULL;
			break;
	}
	return NULL;
}

void reduce(int rule) {
	printf("Reduce by %d\n",rule);
	int ruleLength = rules[rule].length;
	void *pointers[ruleLength];
	int num = ruleLength - 1;
	void *p;
	for (int i = 0; i < ruleLength * 2; i++) {
		if ((p = pop()) != NULL) {
			pointers[num--] = p;
		}
	}
	if (num != -1) printf("Did not fill up pointer array ERROR remaining: %d\n",num+1);
	token_t var = varTokens[rules[rule].var-(TOTAL_TOKENS+NUM_INSTANCES)]->tokenType;
	token_t instance = instanceTokens[instances[stack[stackTopPointer]->instance].gotoAction[var-(TOTAL_TOKENS+NUM_INSTANCES)]]->tokenType;

	void *ptr = 1;

	push(instance,NULL,var,ptr);
}

/*
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
}*/

int parse() {
	push(0,NULL,-1,1);
	printStack();
	while(1) {
		printf("Reading token %d: ",tokenIndex);
		printToken(tokens[tokenIndex]->tokenType);
		printf("\n");
		if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			printf("Skipping whitespace!\n");
			tokenIndex++;
			continue;
		}
		token_t state = stack[stackTopPointer]->instance;
		int actionIndex = tokens[tokenIndex]->tokenType-NUM_INSTANCES;
		Step step = instances[state].actions[actionIndex].step;
		if (step == STEP_ACCEPT) {
			return 1;
		}
		else if (step == STEP_SHIFT) {
			shift(instanceTokens[instances[state].actions[actionIndex].instance]->tokenType, tokens[tokenIndex],-1,tokens[tokenIndex]);
		}
		else if (step == STEP_REDUCE) {
			reduce(instances[state].actions[actionIndex].instance);
		}
		else {
			printf("Token not found %s NUM: %d Token type: ",tokens[tokenIndex]->token,tokens[tokenIndex]->tokenType);
			printToken(tokens[tokenIndex]->tokenType);
			printf("\n");
			return 0;
		}
		printStack();
	}
	return -1;
}
/*
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
}*/
