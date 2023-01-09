#ifndef PARSER_H
#define PARSER_H
#define NUM_RULES 90
#define NUM_ACTIONS 48
#define NUM_GOTO 33
#define NUM_INSTANCES 300
#define TOTAL_TOKENS 50


typedef enum Step{
	STEP_ERROR,
	STEP_REDUCE,
	STEP_SHIFT,
	STEP_ACCEPT
}Step;

struct Rule {
	token_t var;
	token_t *symbols;//tokens and variables
	int length;
} Rule;

struct Action {
	Step step;
	int instance;//or rule in grammar
} Action;

struct Instance {
	struct Action actions[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

struct Instance instances[NUM_INSTANCES];

struct Token *instanceTokens[NUM_INSTANCES];//instance tokens (for stack)
struct Token *varTokens[NUM_GOTO];//variable (nonterminal) tokens (for stack)

int tokenIndex = 0;//for reading input

struct Rule rules[NUM_RULES];//grammar

struct Token **stack;
int stackCapacity = 0;
int stackTopPointer = -1;//points to top of the stack



#endif
