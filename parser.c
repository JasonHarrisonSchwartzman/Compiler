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

void *callSemanticRule(void *param[], int rule) {
	switch (rule) {
		case 0:
			break;
		case 1: 
			return addDeclarations(param[1],param[0]);
		case 2:
			return addDeclarations(NULL,param[0]);
		case 3:
			return addDeclaration(param[0],NULL);
		case 4:
			return addDeclaration(NULL,param[0]);
		case 5: 
			return addVarDecl(param[0],param[1]);
		case 6:
			return NULL;
		case 7:
			return NULL;
		case 8:
			return addSign(SIGNED);
		case 9:
			return addSign(UNSIGNED);
		case 10:
			return addDataType(INT);
		case 11:
			return addDataType(LONG);
		case 12:
			return addDataType(SHORT);
		case 13:
			return addDataType(CHAR);
		case 14:
			return addDataType(DOUBLE);
		case 15:
			return addDecl(param[0],NULL,NULL);
		case 16:
			return addDecl(param[0],param[1],NULL);
		case 17:
			return addName(((struct Token*)param[0])->token,atoi(((struct Token*)param[2])->token),-1);
		case 18:
			return addName(((struct Token*)param[0])->token,-1,-1);
		case 19:
			return addName(((struct Token*)param[1])->token,-1,1);
		case 20:
			return addName(((struct Token*)param[1])->token,atoi(((struct Token*)param[3])->token),1);
		case 21:
			return NULL; //numbers and decimals and what not
		case 22:
			return NULL;
		case 23:
			return NULL;
		case 24:
			return NULL;
		case 25:
			return NULL; // need to add id field
		case 26:
			return addParam(param[3],param[0],param[1]);
		case 27:
			return addParam(NULL,param[0],param[1]);
		case 28:
			return NULL;
		case 29:
			return NULL;
		case 30:
			return NULL;
		case 31:
			return addStatements(param[1],param[0]);
		case 32:
			return addStatements(NULL,param[0]);
		case 33:
			return addStatement(IF,NULL,NULL,NULL,param[0]);
		case 34:
			return addStatement(((struct Loop*)param[0])->stmt,NULL,NULL,param[0],NULL);
		case 35:
			return addStatement(RETURN,NULL,param[0],NULL,NULL);
		case 36:
			return addStatement(((struct FunctionStatement*)param[0])->stmt,param[0],NULL,NULL,NULL);
		case 37:
			return addFunctionStatement(FUNCCALL,param[0],NULL,NULL);
		case 38:
			return addFunctionStatement(DECLARATION,NULL,addVarDecl(param[0],param[1]),NULL);
		case 39:
			return addDecl(param[0],NULL,NULL);
		case 40:
			return addDecl(param[0],NULL,param[1]);
		case 41:
			return addExpr(NULL,NULL,NULL,param[0]);
		case 42:
			return addExpr(param[0],NULL,param[1],param[2]);
		case 43:
			return addExpr(param[0],param[1],NULL,param[2]);
		case 44:
			return addOperation(PLUS);
		case 45:
			return addOperation(MULT);
		case 46:
			return addOperation(MOD);
		case 47:
			return addOperation(DIV);
		case 48:
			return addOperation(MINUS);
		case 49:
			return addOperation(BITWISEAND);
		case 50:
			return addOperation(BITWISEOR);
		case 51:
			return addOperation(BITWISEXOR);
		case 52:
			return addConditional(EQUAL);
		case 53:
			return addConditional(LESSEQUAL);
		case 54:
			return addConditional(GREATEQUAL);
		case 55:
			return addConditional(GREAT);
		case 56:
			return addConditional(LESS);
		case 57:
			return addConditional(AND);
		case 58:
			return addConditional(NOT);
		case 59:
			return addConditional(OR);
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

	void *ptr = callSemanticRule(pointers,rule);
	if (!ptr) ptr = 1;
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
