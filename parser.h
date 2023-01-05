#ifndef PARSER_H
#define PARSER_H
#define NUM_RULES 87
#define NUM_ACTIONS 48
#define NUM_GOTO 32
#define NUM_INSTANCES 300
#define TOTAL_TOKENS 50

struct Rule {
	token_t var;
	token_t *symbols;//tokens and variables
	int length;
} Rule;

struct Instance {
	struct Action actions[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

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


#endif
