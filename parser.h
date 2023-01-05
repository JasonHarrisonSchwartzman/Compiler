#ifndef PARSER_H
#define PARSER_H

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
