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
	struct Action action4id = { STEP_SHIFT, 9 };
	addInstanceAction(4,action4id,1);
	//instance 5
	struct Action action5Assign = { STEP_SHIFT, 10};
	addInstanceAction(5,action5Assign,4);
	//instance 6
	struct Action action6id = { STEP_REDUCE, 6};
	addInstanceAction(6,action6id,1);
	//instance 7
	struct Action action7id = { STEP_REDUCE, 7};
	addInstanceAction(7,action7id,1);
	//instance 8
	struct Action action8$ = { STEP_REDUCE, 1};
	addInstanceAction(8,action8$,8);
	//instance 9
	struct Action action9Equal = { STEP_SHIFT, 11 };
	addInstanceAction(9,action9Equal,4);
	//instance 10
	struct Action action10num = { STEP_SHIFT, 13 };
	struct Action action10id = { STEP_SHIFT, 14 };
	addInstanceAction(10,action10num,0);
	addInstanceAction(10,action10id,1);
	addInstanceGoto(10,4,12);
	//instance 11
	addInstanceAction(11,action10num,0);
	addInstanceAction(11,action10id,1);
	addInstanceGoto(10,4,15);
	//instance 12
	struct Action action12Plus = { STEP_SHIFT, 18};
	struct Action action12Minus = { STEP_SHIFT, 19};
	struct Action action12Semi = { STEP_SHIFT, 16};
	addInstanceAction(12,action12Plus,2);
	addInstanceAction(12,action12Minus,3);
	addInstanceAction(12,action12Semi,5);
	addInstanceGoto(12,5,17);
	//instance 13
	struct Action action13 = { STEP_REDUCE, 8 };
	addInstanceAction(13,action13,2);
	addInstanceAction(13,action13,3);
	addInstanceAction(13,action13,5);
	//instance 14
	struct Action action14 = { STEP_REDUCE, 9};
	addInstanceAction(14,action14,2);
	addInstanceAction(14,action14,3);
	addInstanceAction(14,action14,5);
	//instance 15
	struct Action action15Plus = { STEP_SHIFT, 18 };
	struct Action action15Minus = { STEP_SHIFT, 19 };
	struct Action action15Semi = { STEP_SHIFT, 20 };
	addInstanceAction(15,action15Plus,2);
	addInstanceAction(15,action15Minus,3);
	addInstanceAction(15,action15Semi,5);
	addInstanceGoto(15,5,17);
	//instance 16
	struct Action action16 = { STEP_REDUCE, 5};
	addInstanceAction(16,action16,1);
	addInstanceAction(16,action16,6);
	addInstanceAction(16,action16,7);
	addInstanceAction(16,action16,8);
	//instance 17
	struct Action action17num = { STEP_SHIFT, 22 };
	struct Action action17id = { STEP_SHIFT, 23 };
	addInstanceAction(17,action17num,0);
	addInstanceAction(17,action17id,1);
	addInstanceGoto(17,6,21);
	//instance 18
	struct Action action18 = { STEP_REDUCE, 11 };
	addInstanceAction(18,action18,0);
	addInstanceAction(18,action18,1);
	//instance 19
	struct Action action19 = { STEP_REDUCE, 12};
	addInstanceAction(19,action19,0);
	addInstanceAction(19,action19,1);
	//instance 20
	struct Action action20 = { STEP_REDUCE, 4};
	addInstanceAction(20,action20,1);
	addInstanceAction(20,action20,6);
	addInstanceAction(20,action20,7);
	addInstanceAction(20,action20,8);
	//instance 21
	struct Action action21 = { STEP_REDUCE, 10};
	addInstanceAction(21,action21,2);
	addInstanceAction(21,action21,3);
	addInstanceAction(21,action21,5);
	//instance 22
	struct Action action22 = { STEP_REDUCE, 13};
	addInstanceAction(22,action22,2);
	addInstanceAction(22,action22,3);
	addInstanceAction(22,action22,5);
	//instance 23
	struct Action action23 = { STEP_REDUCE, 14};
	addInstanceAction(23,action23,2);
	addInstanceAction(23,action23,3);
	addInstanceAction(23,action23,5);
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
	printf("----------GRAMMAR-----------\n");
	for (int i = 0; i < NUM_RULES; i++) {
		printRule(rules[i]);
	}
}

void shift(Token *token, Token *instance) {
	push(token);
	push(instance);
	tokenIndex++;
}

void reduce(int rule) {
	for (int i = 0; i < rules[rule].length * 2; i++) {
		pop();
	}
	push(varTokens[rule-(NUM_INSTANCES+TOTAL_TOKENS)]);
}

int parse() {
	push(instanceTokens[0]);
	while(1) {
		//ignoring whitespace
		if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			tokenIndex++;
			continue;
		}
		//reading shift
		Step step = instances[getTopOfStack()->tokenType].actions[tokens[tokenIndex]->tokenType-NUM_INSTANCES].step;
		if (step == STEP_ACCEPT) {
			return 1;
		}
		else if (step == STEP_SHIFT) {
			shift(tokens[tokenIndex],instanceTokens[instances[getTopOfStack()->tokenType].actions[tokens[tokenIndex]->tokenType-NUM_INSTANCES].instance]);
		}
		else if (step == STEP_REDUCE) {
			reduce(instances[getTopOfStack()->tokenType].actions[tokens[tokenIndex]->tokenType-NUM_INSTANCES].instance);
		}
		else {
			printf("Token not found\n");
			return 0;
		}
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
	//printRules();
	initializeInstances();

	parse();



	freeRules();
	freeInstanceAndVarTokens();
	//freeTokens();seg fault
	freeStack();
}
