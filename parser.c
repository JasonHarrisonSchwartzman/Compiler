#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.c"
#define NUM_RULES 15
#define NUM_ACTIONS 9
#define NUM_GOTO 7
#define NUM_INSTANCES 24

typedef enum Step{
	STEP_SHIFT,
	STEP_REDUCE,
	STEP_ERROR,
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
int stackTopPointer = 0;//points to top of the stack

void addInstanceAction(int instanceNum, struct Action action, int actionNum) {
	instances[instanceNum].actions[actionNum].step = action.step;
	instances[instanceNum].actions[actionNum].instance = action.instance;
}
void addInstanceGoto(int instanceNum, int gotoNum, int gotoInstance) {
	instances[instanceNum].gotoAction[gotoNum] = gotoInstance;
}

void initializeInstances() {
	//instance 0
	struct Action action0id = { STEP_SHIFT, 5 };
	struct Action action0int = { STEP_SHIFT, 6 };
	struct Action action0long = { STEP_SHIFT, 7 };
	addInstanceAction(0,action0id,1);
	addInstanceAction(0,action0int,6);
	addInstanceAction(0,action0long,7);
	addInstanceGoto(0,0,1);
	addInstanceGoto(0,1,2);
	addInstanceGoto(0,2,3);
	addInstanceGoto(0,3,4);
	//instance 1
	struct Action action1$ = { STEP_ACCEPT, -1 };
	addInstanceAction(1,action1$,8);
	//instance 2
	struct Action action2id = { STEP_SHIFT, 5 };
	struct Action action2int = { STEP_SHIFT, 6 };
	struct Action action2long = { STEP_SHIFT, 7 };
	struct Action action2$ = { STEP_REDUCE, 2 };
	addInstanceAction(2,action2id,1);
	addInstanceAction(2,action2int,6);
	addInstanceAction(2,action2long,7);
	addInstanceAction(2,action2$,8);
	addInstanceGoto(2,0,8);
	addInstanceGoto(2,1,2);
	addInstanceGoto(2,2,3);
	addInstanceGoto(2,3,4);
	//instance 3
	struct Action action3 = { STEP_REDUCE, 3 };
	addInstanceAction(3,action3,1);
	addInstanceAction(3,action3,6);
	addInstanceAction(3,action3,7);
	addInstanceAction(3,action3,8);
	//instance 4


}

void push(Token *token) {
	stack = realloc(stack,sizeof(Token) * (stackTopPointer + 1));
	stack[stackTopPointer++] = token;
}
void pop() {
	stack = realloc(stack,sizeof(Token) * (--stackTopPointer));
}

Token *getTopOfStack() {
	return stack[stackTopPointer];
}

void freeStack() {
	free(stack);
}

void printToken(token_t token);

void printStack() {
	for (int i = 0; i < stackTopPointer; i++) {
		printToken(stack[i]->tokenType);
	}
}

void createInstanceAndVarTokens() {
	for (int i = 0; i < NUM_INSTANCES; i++) {
		instanceTokens[i] = malloc(sizeof(Token));
		instanceTokens[i]->tokenType = i;
	}
	for (int i = 0; i < NUM_GOTO; i++) {
		varTokens[i] = malloc(sizeof(Token));
		varTokens[i]->tokenType = NUM_INSTANCES + i; 
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
	for (int i = 0; i < NUM_RULES; i++) {
		printRule(rules[i]);
	}
}

int parse() {

	for (int i = 0; i < numTokens; i++) {
		
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
	token_t rule0[1] = { VAR_L };
	addRule(0, VAR_P, 1, rule0);
	token_t rule1[2] = { VAR_D, VAR_L };
	addRule(1, VAR_L, 2, rule1);
	token_t rule2[1] = { VAR_D };
	addRule(2, VAR_L, 1, rule2);
	token_t rule3[1] = { VAR_S };
	addRule(3, VAR_D, 1, rule3);
	token_t rule4[5] = { VAR_T, TOKEN_ID, TOKEN_ASSIGN, VAR_E, TOKEN_SEMICOLON };
	addRule(4, VAR_S, 5, rule4);
	token_t rule5[4] = { TOKEN_ID, TOKEN_ASSIGN, VAR_E, TOKEN_SEMICOLON };
	addRule(5, VAR_S, 4, rule5);
	token_t rule6[1] = { TOKEN_INT };
	addRule(6, VAR_T, 1, rule6);
	token_t rule7[1] = { TOKEN_LONG };
	addRule(7, VAR_T, 1, rule7);
	token_t rule8[1] = { TOKEN_NUM };
	addRule(8, VAR_E, 1, rule8);
	token_t rule9[1] = { TOKEN_ID };
	addRule(9, VAR_E, 1, rule9);
	token_t rule10[3] = { VAR_E, VAR_O, VAR_X };
	addRule(10, VAR_E, 3, rule10);
	token_t rule11[1] = { TOKEN_PLUS };
	addRule(11, VAR_O, 1, rule11);
	token_t rule12[1] = { TOKEN_MINUS };
	addRule(12, VAR_O, 1, rule12);
	token_t rule13[1] = { TOKEN_NUM };
	addRule(13, VAR_X, 1, rule13);
	token_t rule14[1] = { TOKEN_ID };
	addRule(14, VAR_X, 1, rule14);
}

int main(int argc, char *argv[]) {
	scanner(argc, argv);
	initializeRules();
	createInstanceAndVarTokens();
	printRules();
	initializeInstances();





	freeRules();
	freeInstanceAndVarTokens();
	//freeTokens();seg fault
	freeStack();
}
