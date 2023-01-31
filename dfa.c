#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#define NUM_STATES 99

struct Transition {
	int state;
	char letter;
	int delimeter;//a delimeter is defined as a letter that separates a token from another token
	/*EXAMPLES:
	 * int x = 5; 
	 * the space between int and x sepeartes the three tokens. 'int' '[space]' 'x' are all tokens
	 * the transition will looks as follows
	 *  KEY         l: 'q ', d: '1'   SPACE      l: 'x', d: '1'   IDENT
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
	printf("Error!!! This character caused it %c ascii: %d\n",letter,(int)letter);
	return -1;
}

void createIDTransitions(int startState, char blacklist[]) {
	for (int i = 0; i < 127; i++) {
		// handles 0-9
		if (startState != 0 && startState != 82) {
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
				if (startState == 91 || startState == 93) {
					addTransition(startState, i, 80, 1);
				} else {
					addTransition(startState, i, 80, 0);
				}
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
token_t tokenValues[] = { TOKEN_EOF, TOKEN_ID, TOKEN_NUM, TOKEN_DOUBLE, TOKEN_CHARCONST, TOKEN_STRINGCONST};
int numKeywords = 15;
int numDelimeters = 3;
char *keywords[] = { "if", "else", "elseif", "while", "for", "int", "long", "double", "short", "char", "return", "break", "continue", "signed", "unsigned" };
char *delimiters[] = { " ", "\t", "\n" };
int numSpecialChars = 23;
char specialChars[] = { '~','!','@','%','^','&','*','(',')','-','+','=','{','[','}',']','/','|',';','<','>',',','?' };
unsigned long specialCharStates[] = { };
int numAlphabet = 92;
char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','~','!','@','%','^','&','*','(',')','-','_','+','=','{','[','}',']','/','\"','\'','|',';','<','>',',','.','?',' ','\t','\n' };

void createDFA() {
	for (int i = 0; i < numKeywords; i++) {
		
	}
}

void printNumTransitions() {
	for (int i = 0; i < NUM_STATES; i++) {
		printf("State %d: %d\n",i,states[i].numTransitions);
	}
	for (int i = 0; i< states[0].numTransitions; i++) {
		printf("%c ",states[0].transitions[i].letter);
	}
	printf("\n");
	exit(1);
}

/*
 * builds dfa
 */
void initialize() {
	states[0].token = TOKEN_EOF;

	states[1].token = TOKEN_ID;

	states[2].token = TOKEN_IF;

	states[3].token = TOKEN_ID;
	states[4].token = TOKEN_ID;
	states[5].token = TOKEN_ID;

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

	states[67].token = TOKEN_ID;

	states[68].token = TOKEN_ELSEIF;

	states[69].token = TOKEN_EQUAL;
	states[70].token = TOKEN_LESS;
	states[71].token = TOKEN_LESSEQUAL;
	states[72].token = TOKEN_GREAT;
	states[73].token = TOKEN_GREATEQUAL;
	states[75].token = TOKEN_NOT;
	states[76].token = TOKEN_BITWISEAND;
	states[77].token = TOKEN_AND;
	states[78].token = TOKEN_BITWISEXOR;

	states[79].token = TOKEN_NUM;
	states[80].token = TOKEN_ID;
	states[81].token = TOKEN_ASSIGN;
	states[82].token = TOKEN_WHITESPACE;
	states[83].token = TOKEN_PLUS;
	states[84].token = TOKEN_MINUS;
	states[85].token = TOKEN_SEMICOLON;
	states[86].token = TOKEN_MULT;
	states[87].token = TOKEN_DIV;
	states[88].token = TOKEN_MOD;

	states[89].token = TOKEN_BITWISEOR;
	states[90].token = TOKEN_OR;

	states[91].token = TOKEN_LEFTPAREN;
	states[92].token = TOKEN_RIGHTPAREN;

	states[93].token = TOKEN_LEFTCURLY;
	states[94].token = TOKEN_RIGHTCURLY;

	states[97].token = TOKEN_CHARCONST;

	addTransition(0, 'i', 1, 0);
	addTransition(0, 'e', 3, 0);
	addTransition(0, 'f', 7, 0);
	addTransition(0, 'd', 17, 0);
	addTransition(0, 'c', 23, 0);
	addTransition(0, 'w', 10, 0);
	addTransition(0, 'l', 27, 0);
	addTransition(0, 'b', 55, 0);
	addTransition(0, 's', 31, 0);
	addTransition(0, 'u', 41, 0);
	addTransition(0, 'r', 49, 0);
	createSpaceTransitions();
	addTransition(0, ' ', 82, 1);
	addTransition(0, '\n', 82, 1);
	addTransition(0, '\t', 82, 1);
	createIDTransitions(0, "iefwdclsuvrb");
	
	
	addTransition(1, 'f', 2, 0);

	addTransition(81, 'i', 1, 1);
	addTransition(82, 'i', 1, 1);
	addTransition(85, 'i', 1, 1);

	addTransition(1, ' ', 82, 1);
	addTransition(1, '\n', 82, 1);
	addTransition(1, '\t', 82, 1);
	addTransition(1, '\r', 82, 1);

	addTransition(2, ' ', 82, 1);
	addTransition(2, '\n', 82, 1);
	addTransition(2, '\t', 82, 1);
	addTransition(2, '\r', 82, 1);



	
	addTransition(3, 'l', 4, 0);
	addTransition(4, 's', 5, 0);
	addTransition(5, 'e', 6, 0);

	addTransition(81, 'e', 3, 1);
	addTransition(82, 'e', 3, 1);
	addTransition(85, 'e', 3, 1);

	addTransition(3, ' ', 82, 1);
	addTransition(3, '\n', 82, 1);
	addTransition(3, '\t', 82, 1);
	addTransition(3, '\r', 82, 1);

	addTransition(4, ' ', 82, 1);
	addTransition(4, '\n', 82, 1);
	addTransition(4, '\t', 82, 1);
	addTransition(4, '\r', 82, 1);

	addTransition(5, ' ', 82, 1);
	addTransition(5, '\n', 82, 1);
	addTransition(5, '\t', 82, 1);
	addTransition(5, '\r', 82, 1);

	addTransition(6, ' ', 82, 1);
	addTransition(6, '\n', 82, 1);
	addTransition(6, '\t', 82, 1);
	addTransition(6, '\r', 82, 1);



	
	addTransition(7, 'o', 8, 0);
	addTransition(8, 'r', 9, 0);

	addTransition(81, 'f', 7, 1);
	addTransition(82, 'f', 7, 1);
	addTransition(85, 'f', 7, 1);

	addTransition(9, ' ', 82, 1);
	addTransition(9, '\n', 82, 1);
	addTransition(9, '\t', 82, 1);
	addTransition(9, '\r', 82, 1);

	
	addTransition(10, 'h', 11, 0);
	addTransition(11, 'i', 12, 0);
	addTransition(12, 'l', 13, 0);
	addTransition(13, 'e', 14, 0);

	addTransition(81, 'w', 10, 1);
	addTransition(82, 'w', 10, 1);
	addTransition(85, 'w', 10, 1);


	// int
	addTransition(1, 'n', 15, 0);
	addTransition(15, 't', 16, 0);

	addTransition(16, ' ', 82, 1);
	addTransition(16, '\n', 82, 1);
	addTransition(16, '\t', 82, 1);
	addTransition(16, '\r', 82, 1);


	// double
	addTransition(17, 'o', 18, 0);
	addTransition(18, 'u', 19, 0);
	addTransition(19, 'b', 20, 0);
	addTransition(20, 'l', 21, 0);
	addTransition(21, 'e', 22, 0);

	addTransition(81, 'd', 17, 1);
	addTransition(82, 'd', 17, 1);
	addTransition(85, 'd', 17, 1);
	addTransition(22, ' ', 82, 1);
	addTransition(22, '\n', 82, 1);
	addTransition(22, '\t', 82, 1);
	addTransition(22, '\r', 82, 1);


	addTransition(23, 'h', 24, 0);
	addTransition(24, 'a', 25, 0);
	addTransition(25, 'r', 26, 0);

	addTransition(81, 'c', 23, 1);
	addTransition(82, 'c', 23, 1);
	addTransition(85, 'c', 23, 1);

	addTransition(26, ' ', 82, 1);
	addTransition(26, '\n', 82, 1);
	addTransition(26, '\t', 82, 1);
	addTransition(26, '\r', 82, 1);


	addTransition(27, 'o', 28, 0);
	addTransition(28, 'n', 29, 0);
	addTransition(29, 'g', 30, 0);

	addTransition(81, 'l', 27, 1);
	addTransition(82, 'l', 27, 1);
	addTransition(85, 'l', 27, 1);

	addTransition(30, ' ', 82, 1);
	addTransition(30, '\n', 82, 1);
	addTransition(30, '\t', 82, 1);
	addTransition(30, '\r', 82, 1);


	addTransition(31, 'h', 32, 0);
	addTransition(32, 'o', 33, 0);
	addTransition(33, 'r', 34, 0);
	addTransition(34, 't', 35, 0);

	addTransition(81, 's', 31, 1);
	addTransition(82, 's', 31, 1);
	addTransition(85, 's', 31, 1);

	addTransition(35, ' ', 82, 1);
	addTransition(35, '\n', 82, 1);
	addTransition(35, '\t', 82, 1);
	addTransition(35, '\r', 82, 1);


	// signed
	addTransition(31, 'i', 36, 0);
	addTransition(36, 'g', 37, 0);
	addTransition(37, 'n', 38, 0);
	addTransition(38, 'e', 39, 0);
	addTransition(39, 'd', 40, 0);

	addTransition(40, ' ', 82, 1);
	addTransition(40, '\n', 82, 1);
	addTransition(40, '\t', 82, 1);
	addTransition(40, '\r', 82, 1);

	addTransition(41, 'n', 42, 0);
	addTransition(42, 's', 43, 0);
	addTransition(43, 'i', 44, 0);
	addTransition(44, 'g', 45, 0);
	addTransition(45, 'n', 46, 0);
	addTransition(46, 'e', 47, 0);
	addTransition(47, 'd', 48, 0);

	addTransition(81, 'u', 41, 1);
	addTransition(82, 'u', 41, 1);
	addTransition(85, 'u', 41, 1);

	addTransition(48, ' ', 82, 1);
	addTransition(48, '\n', 82, 1);
	addTransition(48, '\t', 82, 1);
	addTransition(48, '\r', 82, 1);

	addTransition(49, 'e', 50, 0);
	addTransition(50, 't', 51, 0);
	addTransition(51, 'u', 52, 0);
	addTransition(52, 'r', 53, 0);
	addTransition(53, 'n', 54, 0);

	addTransition(81, 'r', 49, 1);
	addTransition(82, 'r', 49, 1);
	addTransition(85, 'r', 49, 1);

	addTransition(54, ' ', 82, 1);
	addTransition(54, '\n', 82, 1);
	addTransition(54, '\t', 82, 1);
	addTransition(54, '\r', 82, 1);
	addTransition(54, ';', 85, 1);
	
	addTransition(55, 'r', 56, 0);
	addTransition(56, 'e', 57, 0);
	addTransition(57, 'a', 58, 0);
	addTransition(58, 'k', 59, 0);

	addTransition(81, 'b', 55, 1);
	addTransition(82, 'b', 55, 1);
	addTransition(85, 'b', 55, 1);

	addTransition(59, ' ', 82, 1);
	addTransition(59, '\n', 82, 1);
	addTransition(59, '\t', 82, 1);
	addTransition(59, '\r', 82, 1);
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
	addTransition(66, '\n', 82, 1);
	addTransition(66, '\t', 82, 1);
	addTransition(66, '\r', 82, 1);
	addTransition(66, ';', 85, 1);


	// elseif
	addTransition(6, 'i', 67, 0);
	addTransition(67, 'f', 68, 0);

	addTransition(67, ' ', 82, 1);
	addTransition(67, '\n', 82, 1);
	addTransition(67, '\t', 82, 1);
	addTransition(67, '\r', 82, 1);

	addTransition(68, ' ', 82, 1);
	addTransition(68, '\n', 82, 1);
	addTransition(68, '\t', 82, 1);
	addTransition(68, '\r', 82, 1);


	// assign
	addTransition(81, ' ', 82, 1);
	addTransition(81, '\n', 82, 1);
	addTransition(81, '\t', 82, 1);
	addTransition(81, '\r', 82, 1);
	addTransition(82, '=', 81, 1);


	// id
	addTransition(80, ' ', 82, 1);
	addTransition(80, '\n', 82, 1);
	addTransition(80, '\t', 82, 1);
	addTransition(80, '\r', 82, 1);
	addTransition(80, '=', 81, 1);
	addTransition(80, ';', 85, 1);


	// numbers
	for (int i = 48; i < 58; i++) {
		addTransition(0, i, 79, 0);
		addTransition(79, i, 79, 0);
		addTransition(82, i, 79, 1);

		// plus
		addTransition(83, i, 79, 1);

		// minus
		addTransition(84, i, 79, 1);

		// mult
		addTransition(86, i, 79, 1);

		// div
		addTransition(87, i, 79, 1);

		// mod
		addTransition(88, i, 79, 1);

		// equal
		addTransition(69, i, 79, 1);

		// less
		addTransition(70, i, 79, 1);

		// less equal
		addTransition(71, i, 79, 1);

		// greater
		addTransition(72, i, 79, 1);

		// greater equal
		addTransition(73, i, 79, 1);

		// not equal
		addTransition(75, i, 79, 1);

		// bitwise and
		addTransition(76, i, 79, 1);

		// and
		addTransition(77, i, 79, 1);

		// bitwise xor
		addTransition(78, i, 79, 1);

		// bitwise or
		addTransition(89, i, 79, 1);

		// or
		addTransition(90, i, 79, 1);

		// assign
		addTransition(81, i, 79, 1);

		// left paren
		addTransition(91, i, 79, 1);
	}
	
	// numbers
	addTransition(79, ')', 92, 1);
	addTransition(79, ';', 85, 1);
	addTransition(79, ' ', 82, 1);
	addTransition(79, '\n', 82, 1);
	addTransition(79, '\t', 82, 1);
	addTransition(79, '\r', 82, 1);

	// assign
	addTransition(79, '=', 81, 1);

	// semicolon
	addTransition(85, ' ', 82, 1);
	addTransition(85, '\n', 82, 1);
	addTransition(85, '\t', 82, 1);
	addTransition(85, '\r', 82, 1);
	addTransition(82, ';', 85, 1);

	// plus
	addTransition(82, '+', 83, 1);
	addTransition(79, '+', 83, 1);
	addTransition(83, ' ', 82, 1);
	addTransition(83, '\n', 82, 1);
	addTransition(83, '\t', 82, 1);
	addTransition(83, '\r', 82, 1);

	// minus 
	addTransition(82, '-', 84, 1);
	addTransition(79, '-', 84, 1);
	addTransition(84, ' ', 82, 1);
	addTransition(84, '\n', 82, 1);
	addTransition(84, '\t', 82, 1);
	addTransition(84, '\r', 82, 1);

	// mult
	addTransition(82, '*', 86, 1);
	addTransition(79, '*', 86, 1);
	addTransition(86, ' ', 82, 1);
	addTransition(86, '\n', 82, 1);
	addTransition(86, '\t', 82, 1);
	addTransition(86, '\r', 82, 1);

	// div
	addTransition(82, '/', 87, 1);
	addTransition(79, '/', 87, 1);
	addTransition(87, ' ', 82, 1);
	addTransition(87, '\n', 82, 1);
	addTransition(87, '\t', 82, 1);
	addTransition(87, '\r', 82, 1);

	// mod
	addTransition(82, '%', 88, 1);
	addTransition(79, '%', 88, 1);
	addTransition(88, ' ', 82, 1);
	addTransition(88, '\n', 82, 1);
	addTransition(88, '\t', 82, 1);
	addTransition(88, '\r', 82, 1);

	// equal
	addTransition(81, '=', 69, 0);
	addTransition(69, ' ', 82, 1);
	addTransition(69, '\n', 82, 1);
	addTransition(69, '\t', 82, 1);
	addTransition(69, '\r', 82, 1);

	// less
	addTransition(82, '<', 70, 1);
	addTransition(70, ' ', 82, 1);
	addTransition(70, '\n', 82, 1);
	addTransition(70, '\t', 82, 1);
	addTransition(70, '\r', 82, 1);

	// less equal
	addTransition(70, '=', 71, 0);
	addTransition(71, ' ', 82, 1);
	addTransition(71, '\n', 82, 1);
	addTransition(71, '\t', 82, 1);
	addTransition(71, '\r', 82, 1);

	// greater 
	addTransition(82, '>', 72, 1);
	addTransition(72, ' ', 82, 1);
	addTransition(72, '\n', 82, 1);
	addTransition(72, '\t', 82, 1);
	addTransition(72, '\r', 82, 1);

	// greater equal
	addTransition(72, '=', 73, 0);
	addTransition(73, ' ', 82, 1);
	addTransition(73, '\n', 82, 1);
	addTransition(73, '\t', 82, 1);
	addTransition(73, '\r', 82, 1);

	// not equal
	addTransition(82, '!', 74, 1);
	addTransition(74, '=', 75, 0);
	addTransition(75, ' ', 82, 1);
	addTransition(75, '\n', 82, 1);
	addTransition(75, '\t', 82, 1);
	addTransition(75, '\r', 82, 1);

	// bitwise and
	addTransition(79, '&', 76, 1);
	addTransition(76, ' ', 82, 1);
	addTransition(76, '\n', 82, 1);
	addTransition(76, '\t', 82, 1);
	addTransition(76, '\r', 82, 1);

	// and
	addTransition(76, '&', 77, 0);
	addTransition(77, ' ', 82, 1);
	addTransition(77, '\n', 82, 1);
	addTransition(77, '\t', 82, 1);
	addTransition(77, '\r', 82, 1);

	// bitwise xor
	addTransition(82, '^', 78, 1);
	addTransition(78, ' ', 82, 1);
	addTransition(78, '\n', 82, 1);
	addTransition(78, '\t', 82, 1);
	addTransition(78, '\r', 82, 1);

	// bitwise or
	addTransition(82, '|', 89, 1);
	addTransition(89, ' ', 82, 1);
	addTransition(89, '\n', 82, 1);
	addTransition(89, '\t', 82, 1);
	addTransition(89, '\r', 82, 1);

	// or
	addTransition(89, '|', 90, 0);
	addTransition(90, ' ', 82, 1);
	addTransition(90, '\n', 82, 1);
	addTransition(90, '\t', 82, 1);
	addTransition(90, '\r', 82, 1);

	// left paren
	addTransition(82, '(', 91, 1);
	addTransition(80, '(', 91, 1);
	addTransition(91, ')', 92, 1);
	addTransition(91, ' ', 82, 1);
	addTransition(91, '\n', 82, 1);
	addTransition(91, '\t', 82, 1);
	addTransition(91, '\r', 82, 1);

	// right paren
	addTransition(82, ')', 92, 1);
	addTransition(92, ' ', 82, 1);
	addTransition(92, '\n', 82, 1);
	addTransition(92, '\t', 82, 1);
	addTransition(92, '\r', 82, 1);
	addTransition(92, ';', 85, 1);

	// left curly
	addTransition(92, '{', 93, 1); // right paren to left curly
	addTransition(82, '{', 93, 1); 
	addTransition(93, ' ', 82, 1);
	addTransition(93, '\n', 82, 1);
	addTransition(93, '\t', 82, 1);
	addTransition(93, '\r', 82, 1);

	// right curly
	addTransition(85, '}', 94, 1); // semicolon to right curly
	addTransition(82, '}', 94, 1);
	addTransition(94, ' ', 82, 1);
	addTransition(94, '\n', 82, 1);
	addTransition(94, '\t', 82, 1);
	addTransition(94, '\r', 82, 1);


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
	
	// if
	addTransition(2, '(', 91, 1);

	// elseif
	addTransition(68, '(', 91, 1);

	// else
	addTransition(6, '{', 93, 1);

	// char const
	addTransition(69, '\'', 95, 1);
	addTransition(70, '\'', 95, 1);
	addTransition(71, '\'', 95, 1);
	addTransition(72, '\'', 95, 1);
	addTransition(73, '\'', 95, 1);
	addTransition(74, '\'', 95, 1);
	addTransition(75, '\'', 95, 1);
	addTransition(82, '\'', 95, 1);
	addTransition(91, '\'', 95, 1);
	for (int i = 0; i < 126; i++) {
		if (i == 9 || i == 10 || i == 13 || (i >= 32 && i <= 126)) {
			addTransition(95, i, 96, 0);
		}
	}
	addTransition(96, '\'', 97, 0);
	addTransition(97, ')', 92, 1);
	addTransition(97, ';', 85, 1);
	addTransition(97, '=', 81, 1);
	addTransition(97, '<', 70, 1);
	addTransition(97, '>', 72, 1);
	addTransition(97, '!', 74, 1);
	addTransition(97, ' ', 82, 1);
	addTransition(97, '\n', 82, 1);
	addTransition(97, '\t', 82, 1);
	addTransition(97, '\r', 82, 1);

	// string
	addTransition(69, '\"', 98, 1);
	addTransition(70, '\"', 98, 1);
	addTransition(71, '\"', 98, 1);
	addTransition(72, '\"', 98, 1);
	addTransition(73, '\"', 98, 1);
	addTransition(74, '\"', 98, 1);
	addTransition(75, '\"', 98, 1);
	addTransition(82, '\"', 98, 1);
	addTransition(91, '\'', 98, 1);
	for (int i = 0; i < 126; i++) {
		if (i == 9 || i == 10 || i == 13 || (i >= 32 && i <= 126)) {
			addTransition(98, i, 99, 0);
		}
	}
	addTransition(99, '\"', 100, 0);
	addTransition(100, ')', 92, 1);
	addTransition(100, '=', 81, 1);
	addTransition(100, ';', 85, 1);
	addTransition(100, '<', 70, 1);
	addTransition(100, '>', 72, 1);
	addTransition(100, '!', 74, 1);
	addTransition(100, ' ', 82, 1);
	addTransition(100, '\n', 82, 1);
	addTransition(100, '\t', 82, 1);
	addTransition(100, '\r', 82, 1);


	createIDTransitions(1, "f");
	createIDTransitions(2, "");
	createIDTransitions(3, "l");
	createIDTransitions(4, "s");
	createIDTransitions(5, "e");
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
	createIDTransitions(17, "o");
	createIDTransitions(18, "u");
	createIDTransitions(19, "b");
	createIDTransitions(20, "l");
	createIDTransitions(21, "e");
	createIDTransitions(22, "");
	createIDTransitions(23, "ho");
	createIDTransitions(24, "a");
	createIDTransitions(25, "r");
	createIDTransitions(26, "");
	createIDTransitions(27, "o");
	createIDTransitions(28, "n");
	createIDTransitions(29, "g");
	createIDTransitions(30, "");
	createIDTransitions(31, "hi");
	createIDTransitions(32, "o");
	createIDTransitions(33, "r");
	createIDTransitions(34, "t");
	createIDTransitions(35, "");
	createIDTransitions(36, "g");
	createIDTransitions(37, "n");
	createIDTransitions(38, "e");
	createIDTransitions(39, "d");
	createIDTransitions(40, "");
	createIDTransitions(41, "n");
	createIDTransitions(42, "s");
	createIDTransitions(43, "i");
	createIDTransitions(44, "g");
	createIDTransitions(45, "n");
	createIDTransitions(46, "e");
	createIDTransitions(47, "d");
	createIDTransitions(48, "");
	createIDTransitions(49, "e");
	createIDTransitions(50, "t");
	createIDTransitions(51, "u");
	createIDTransitions(52, "r");
	createIDTransitions(53, "n");
	createIDTransitions(54, "");
	createIDTransitions(55, "r");
	createIDTransitions(56, "e");
	createIDTransitions(57, "a");
	createIDTransitions(58, "k");
	createIDTransitions(59, "");
	createIDTransitions(60, "n");
	createIDTransitions(61, "t");
	createIDTransitions(62, "i");
	createIDTransitions(63, "n");
	createIDTransitions(64, "u");
	createIDTransitions(65, "e");
	createIDTransitions(66, "");
	createIDTransitions(67, "f");
	createIDTransitions(68, "");
	createIDTransitions(69, "");
	createIDTransitions(70, "");
	createIDTransitions(71, "");
	createIDTransitions(72, "");
	createIDTransitions(73, "");
	// skip 74 because 74 is !
	createIDTransitions(75, "");
	createIDTransitions(76, "");
	createIDTransitions(77, "");
	createIDTransitions(78, "");
	// skip 79 because 79 is NUM
	createIDTransitions(80, "");
	createIDTransitions(81, "iefwdclsuvrb");
	createIDTransitions(82, "");
	createIDTransitions(83, "");
	createIDTransitions(84, "");
	createIDTransitions(85, "");
	createIDTransitions(86, "");
	createIDTransitions(87, "");
	createIDTransitions(88, "");
	createIDTransitions(89, "");
	createIDTransitions(90, "");
	createIDTransitions(91, "");
	// skip 92 because 92 is right paren
	createIDTransitions(93, "");
	createIDTransitions(94, "");
	printNumTransitions();
}
