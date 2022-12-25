#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "dfa.h"
#define NUM_STATES 100

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
	printf("state %d\n", startState);
	printf("Error!!! This character caused it %c\n",letter);
	return -1;
}

void createIDTransitions(int startState, char blacklist[]) {
	for (int i = 0; i < 127; i++) {
		// handles 0-9 when startStart != 0
		if (startState != 0) {
			if (i >= 48 && i <= 57) {
				char *ptr = strchr(blacklist, i);
				if (!ptr) {
					addTransition(startState, i, 80, 0);
				}
			}
		}
		// handles a-z and A-Z
		if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122)) {
			char *ptr = strchr(blacklist, i);
			if (!ptr) {
				addTransition(startState, i, 80, 0);
			}
		}
	}
}

// Handles from space to next char
void createSpaceTransitions() {
	for (int i = 0; i < 127; i++) {
		// assign
		if (i == 61) {
			addTransition(82, i, 81, 1);
			continue;
		}
		// space
		if (i == 10 || i == 32) {
			addTransition(82, i, 82, 0);
			continue;
		}

		char *ptr = strchr("iefwdclsuvrb", i);
		if (!ptr) {
			// a-z and A-Z
			if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122)) {
				addTransition(82, i, 80, 1);
				addTransition(85, i, 80, 1);
			}
		}
	}
}

/*
 * builds dfa
 */
void initialize() {
	states[0].token = TOKEN_EOF;
	states[2].token = TOKEN_IF;
	states[6].token = TOKEN_ELSE;
	states[9].token = TOKEN_FOR;
	states[14].token = TOKEN_WHILE;
	states[16].token = TOKEN_INT;
	states[22].token = TOKEN_DOUBLE;
	states[26].token = TOKEN_CHAR;
	states[30].token = TOKEN_LONG;
	states[35].token = TOKEN_SHORT;
	states[40].token = TOKEN_SIGNED;
	states[48].token = TOKEN_UNSIGNED;
	states[54].token = TOKEN_RETURN;
	states[59].token = TOKEN_BREAK;
	states[66].token = TOKEN_CONTINUE;
	states[69].token = TOKEN_FUNC;
	states[73].token = TOKEN_VOID;
	states[78].token = TOKEN_STRUCT;
	states[79].token = TOKEN_NUM;
	states[80].token = TOKEN_ID;
	states[81].token = TOKEN_ASSIGN;
	states[82].token = TOKEN_WHITESPACE;
	states[83].token = TOKEN_PLUS;
	states[84].token = TOKEN_MINUS;
	states[85].token = TOKEN_SEMICOLON;

	// if
	addTransition(0, 'i', 1, 0);
	addTransition(1, 'f', 2, 0);
	addTransition(81, 'i', 1, 1);
	addTransition(85, 'i', 1, 1);

	// else
	addTransition(0, 'e', 3, 0);
	addTransition(3, 'l', 4, 0);
	addTransition(4, 's', 5, 0);
	addTransition(5, 'e', 6, 0);
	addTransition(81, 'e', 3, 1);
	addTransition(85, 'e', 3, 1);

	// for
	addTransition(0, 'f', 7, 0);
	addTransition(7, 'o', 8, 0);
	addTransition(8, 'r', 9, 0);
	addTransition(81, 'f', 7, 1);
	addTransition(85, 'f', 7, 1);

	// while
	addTransition(0, 'w', 10, 0);
	addTransition(10, 'h', 11, 0);
	addTransition(11, 'i', 12, 0);
	addTransition(12, 'l', 13, 0);
	addTransition(13, 'e', 14, 0);
	addTransition(81, 'w', 10, 1);
	addTransition(85, 'w', 10, 1);

	// int
	addTransition(1, 'n', 15, 0);
	addTransition(15, 't', 16, 0);
	addTransition(16, ' ', 82, 1);
	addTransition(16, '\n', 82, 1);

	// double
	addTransition(0, 'd', 17, 0);
	addTransition(17, 'o', 18, 0);
	addTransition(18, 'u', 19, 0);
	addTransition(19, 'b', 20, 0);
	addTransition(20, 'l', 21, 0);
	addTransition(21, 'e', 22, 0);
	addTransition(81, 'd', 17, 1);
	addTransition(85, 'd', 17, 1);
	addTransition(22, ' ', 82, 1);
	addTransition(22, '\n', 82, 1);

	// char
	addTransition(0, 'c', 23, 0);
	addTransition(23, 'h', 24, 0);
	addTransition(24, 'a', 25, 0);
	addTransition(25, 'r', 26, 0);
	addTransition(81, 'c', 23, 1);
	addTransition(85, 'c', 23, 1);
	addTransition(26, ' ', 82, 1);
	addTransition(26, '\n', 82, 1);

	// long
	addTransition(0, 'l', 27, 0);
	addTransition(27, 'o', 28, 0);
	addTransition(28, 'n', 29, 0);
	addTransition(29, 'g', 30, 0);
	addTransition(81, 'l', 27, 1);
	addTransition(85, 'l', 27, 1);
	addTransition(30, ' ', 82, 1);
	addTransition(30, '\n', 82, 1);

	// short
	addTransition(0, 's', 31, 0);
	addTransition(31, 'h', 32, 0);
	addTransition(32, 'o', 33, 0);
	addTransition(33, 'r', 34, 0);
	addTransition(34, 't', 35, 0);
	addTransition(81, 's', 31, 1);
	addTransition(85, 's', 31, 1);
	addTransition(35, ' ', 82, 1);
	addTransition(35, '\n', 82, 1);

	// signed
	addTransition(31, 'i', 36, 0);
	addTransition(36, 'g', 37, 0);
	addTransition(37, 'n', 38, 0);
	addTransition(38, 'e', 39, 0);
	addTransition(39, 'd', 40, 0);
	addTransition(40, ' ', 82, 1);
	addTransition(40, '\n', 82, 1);

	// unsigned
	addTransition(0, 'u', 41, 0);
	addTransition(41, 'n', 42, 0);
	addTransition(42, 's', 43, 0);
	addTransition(43, 'i', 44, 0);
	addTransition(44, 'g', 45, 0);
	addTransition(45, 'n', 46, 0);
	addTransition(46, 'e', 47, 0);
	addTransition(47, 'd', 48, 0);
	addTransition(81, 'u', 41, 1);
	addTransition(85, 'u', 41, 1);
	addTransition(48, ' ', 82, 1);
	addTransition(48, '\n', 82, 1);

	// return
	addTransition(0, 'r', 49, 0);
	addTransition(49, 'e', 50, 0);
	addTransition(50, 't', 51, 0);
	addTransition(51, 'u', 52, 0);
	addTransition(52, 'r', 53, 0);
	addTransition(53, 'n', 54, 0);
	addTransition(81, 'r', 49, 1);
	addTransition(85, 'r', 49, 1);
	addTransition(54, ' ', 82, 1);
	
	// break
	addTransition(0, 'b', 55, 0);
	addTransition(55, 'r', 56, 0);
	addTransition(56, 'e', 57, 0);
	addTransition(57, 'a', 58, 0);
	addTransition(58, 'k', 59, 0);
	addTransition(81, 'b', 55, 1);
	addTransition(85, 'b', 55, 1);
	addTransition(59, ' ', 82, 1);
	addTransition(59, ';', 85, 1);

	// continue
	addTransition(23, 'o', 60, 0);
	addTransition(60, 'n', 61, 0);
	addTransition(61, 't', 62, 0);
	addTransition(62, 'i', 63, 0);
	addTransition(63, 'n', 64, 0);
	addTransition(64, 'u', 65, 0);
	addTransition(65, 'e', 66, 0);
	addTransition(66, ' ', 82, 1);
	addTransition(66, ';', 85, 1);

	// func
	addTransition(7, 'u', 67, 0);
	addTransition(67, 'n', 68, 0);
	addTransition(68, 'c', 69, 0);

	// void
	addTransition(0, 'v', 70, 0);
	addTransition(70, 'o', 71, 0);
	addTransition(71, 'i', 72, 0);
	addTransition(72, 'd', 73, 0);
	addTransition(81, 'v', 70, 1);
	addTransition(85, 'v', 70, 1);

	// struct
	addTransition(25, 't', 74, 0);
	addTransition(74, 'r', 75, 0);
	addTransition(75, 'u', 76, 0);
	addTransition(76, 'c', 77, 0);
	addTransition(77, 't', 78, 0);

	// assign
	addTransition(81, ' ', 82, 1);
	addTransition(82, '=', 81, 1);

	// id
	addTransition(80, ' ', 82, 1);
	addTransition(80, '=', 81, 1);

	// numbers
	for (int i = 48; i < 58; i++) {
		addTransition(0, i, 79, 0);
		addTransition(79, i, 79, 0);
		addTransition(79, ';', 85, 1);
		addTransition(79, ' ', 82, 1);
		addTransition(79, '\n', 82, 1);
		addTransition(82, i, 79, 1);

		// plus
		addTransition(79, '+', 83, 1);
		addTransition(83, i, 79, 1);

		// minus
		addTransition(79, '-', 84, 1);
		addTransition(84, i, 79, 1);

		// assign
		addTransition(79, '=', 81, 1);
		addTransition(81, i, 79, 1);
	}
	
	// whitespaces
	createSpaceTransitions();
	addTransition(0, ' ', 82, 1);
	addTransition(0, '\n', 82, 1);

	// semicolon
	addTransition(85, ' ', 82, 1);
	addTransition(85, '\n', 82, 1);
	addTransition(82, ';', 85, 1);

	// plus
	addTransition(82, '+', 83, 1);
	addTransition(83, ' ', 82, 1);

	// minus 
	addTransition(82, '-', 84, 1);
	addTransition(84, ' ', 82, 1);
	

	// from assign to id
	for (int i = 0; i < 127; i++) {
		char *ptr = strchr("iefwdclsuvrb", i);
		if (!ptr) {
			// a-z and A-Z
			if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122)) {
				addTransition(81, i, 80, 1);
			}
		}
	}

	createIDTransitions(0, "iefwdclsuvrb");
	createIDTransitions(1, "f");
	createIDTransitions(2, "");
	createIDTransitions(3, "l");
	createIDTransitions(4, "s");
	createIDTransitions(5, "e");
	createIDTransitions(6, "");
	createIDTransitions(6, "");
	createIDTransitions(7, "o");
	createIDTransitions(8, "r");
	createIDTransitions(9, "");
	createIDTransitions(10, "h");
	createIDTransitions(11, "i");
	createIDTransitions(12, "l");
	createIDTransitions(13, "e");
	createIDTransitions(14, "");
	createIDTransitions(15, "t");
	createIDTransitions(16, "");
	createIDTransitions(17, "");
	createIDTransitions(80, "");

}



