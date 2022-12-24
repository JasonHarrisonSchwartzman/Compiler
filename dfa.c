#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "dfa.h"
#define NUM_STATES 20

struct Transition {
	int state;
	char letter;
	int delimeter;//a delimeter is defined as a letter that separates a token from another token
	/*EXAMPLES:
	 * int x = 5; 
	 * the space between int and x sepeartes the three tokens. 'int' '[space]' 'x' are all tokens
	 * the transition will looks as follows
	 *  KEY         l: ' ', d: '1'   SPACE      l: 'x', d: '1'   IDENT
	 * STATE[0]-------------------> STATE[1] -----------------> STATE[2]
	 * where STATE[0] is a TOKEN_KEYWORD occurs after reading 'i' then 'n' then 't' (reading 'int')
	 * reading a space will move to STATE[1] is a TOKEN_SPACE 
	 * reading an x will move to STATE[2] is a TOKEN_IDENT
	 * int and [space] is classified as a tokens because a delimeter was read after them
	 * x is not yet a token because a delimeter has not been read but will be because the next character is [space]
	 */
};
typedef struct State {
	token_t token;
	int state;
	struct Transition *transitions;
	int numTransitions;
} State;
struct State states[NUM_STATES];//DFA
int numStates = 0;
/*
 * adds a transition to the dfa
 */
void addTransition(int startState, char letter, int endState, int delimeter) {
	int numTrans = states[startState].numTransitions;
	states[startState].transitions = realloc(states[startState].transitions,sizeof(struct Transition)*(numTrans+1));
	states[startState].transitions[numTrans].letter = letter;
	states[startState].transitions[numTrans].state = endState;
	states[startState].transitions[numTrans].delimeter = delimeter;
	states[startState].numTransitions++;
}

/*
 * returns if transition taken is a delimeter (1 if it is) (0 if it is not) 
 * if transition DNE return -1 (it shouldn't happen in theory)
 */
int takeTransition(int startState, char letter, int *endState) {
	for (int i = 0; i < states[startState].numTransitions; i++) {
		if (states[startState].transitions[i].letter == letter) {
			*endState = states[startState].transitions[i].state;
			return states[startState].transitions[i].delimeter;
		}
	}
	printf("Error!!! This character caused it %c\n",letter);
	return -1;
}
/*
 * builds dfa
 */
void initialize() {
	/*states[0].token = TOKEN_EOF; //Start state
	states[2].token = TOKEN_KEY; //keywords
	states[6].token = TOKEN_NUM;
	states[9].token = TOKEN_ID;
	states[13].token = TOKEN_SPACE;
	states[14].token = TOKEN_COMMENT;
	states[15].token = TOKEN_OPERATOR;
	states[16].token = TOKEN_ASSIGNMENT;
	states[17].token = TOKEN_COMPARATOR;
	states[18].token = TOKEN_STR;*/
	//if
	addTransition(0,'i',1,0);
	addTransition(1,'f',2,0);
	//else
	addTransition(0,'e',3,0);
	addTransition(3,'l',4,0);
	addTransition(4,'s',5,0);
	addTransition(5,'e',2,0);
	//for
	addTransition(0,'f',7,0);
	addTransition(7,'o',8,0);
	addTransition(8,'r',2,0);
	//int 
	addTransition(1,'n',10,0);
	addTransition(10,'t',2,0);
	
	addTransition(2,';',11,1); //testing	
	addTransition(11,'\n',12,1); //still testing
	//numbers
	for (int i = 48; i <= 57; i++) {
		addTransition(0,i,6,0);
		addTransition(6,i,6,0);
	}
}

