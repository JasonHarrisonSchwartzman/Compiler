#ifndef PARSER_H
#define PARSER_H
#define NUM_RULES 90
#define NUM_ACTIONS 48
#define NUM_GOTO 33
#define NUM_INSTANCES 300
#define TOTAL_TOKENS 51
#include "token.h"

/**
 * A skeleton of what is needed for parser.c and parserinit.c consinsting of the sturcture being used and arrays being added to
 */


/*
 * StackItems for the SLR parsing method
 * The stack item contains an instance number and optionally a token OR a nonterminal variable for the grammar
 * The pointer is for the syntax tree and contains a structure depending on what variable is being stored
 */
struct StackItem {
	token_t instance;
			
	Token *token;
	token_t var;

	void *ptr;
} StackItem;

struct StackItem **stack;
int stackTopPointer = -1;


typedef enum Step{
	STEP_ERROR,
	STEP_REDUCE,
	STEP_SHIFT,
	STEP_ACCEPT
}Step;

//Rules for the grammar
struct Rule {
	token_t var;
	token_t *symbols;//tokens and variables
	int length;
} Rule;

//For the SLR parsing table
struct Action {
	Step step;
	int instance;//or rule in grammar
} Action;

//Instances contains actions and goto actions
//Actions consists of reading a token while gotoAction consists of reading a nonterminal variable
struct Instance {
	struct Action actions[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

struct Instance instances[NUM_INSTANCES];

struct Token *instanceTokens[NUM_INSTANCES];//instance tokens (for stack)
struct Token *varTokens[NUM_GOTO];//variable (nonterminal) tokens (for stack)

int tokenIndex = 0;//for reading input

struct Rule rules[NUM_RULES];//grammar


#endif
