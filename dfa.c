#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#define NUM_STATES 104

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

//adding transitions for alpha characters (and _ and `) given a blacklist of what to not include and whether these transitions are delimeters
void addIDTransitions(int startState, char blacklist[], int delimeter) {
	for (char c = '_'; c <= 'z'; c++) {
		char *ptr = strchr(blacklist, c);
		if (!ptr) {
			addTransition(startState, c, 69, delimeter);
		}
	}
	for (char c = 'A'; c <= 'Z'; c++) {
		addTransition(startState, c, 69, delimeter);
	}
}

void addSpecialCharacterTransitions(int s) {
	addTransition(s, '+', 70, 1);
	addTransition(s, '-', 71, 1);
	addTransition(s, '=', 72, 1);
	addTransition(s, ';', 73, 1);
	addTransition(s, '[', 74, 1);
	addTransition(s, ']', 75, 1);
	addTransition(s, '(', 76, 1);
	addTransition(s, ')', 77, 1);
	addTransition(s, '{', 78, 1);
	addTransition(s, '}', 79, 1);
	addTransition(s, '>', 80, 1);
	addTransition(s, '<', 81, 1);
	addTransition(s, '*', 82, 1);
	addTransition(s, '/', 83, 1);
	addTransition(s, '%', 84, 1);
	addTransition(s, '~', 85, 1);
	addTransition(s, '@', 86, 1);
	addTransition(s, ',', 87, 1);
	addTransition(s, '&', 88, 1);
	addTransition(s, '|', 89, 1);
	addTransition(s, '^', 90, 1);
	addTransition(s, '?', 91, 1);
	addTransition(s, '!', 97, 1);
}

//variables are not used but help visualize the components of the DFA
int numKeywords = 15;
int numDelimeters = 4;
char *keywords[] = { "if", "else", "elseif", "while", "for", "int", "long", "double", "short", "char", "return", "break", "continue", "signed", "unsigned" };
char *delimiters[] = { " ", "\t", "\n", "\r" };
int numSpecialChars = 23;
char specialChars[] = { '~','!','@','%','^','&','*','(',')','-','+','=','{','[','}',']','/','|',';','<','>',',','?' };
int numAlphabet = 94;
char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','~','!','@','%','^','&','*','(',')','-','_','`','+','=','{','[','}',']','/','\"','\'','|',';','<','>',',','.','?',' ','\t','\n','\r'};

// for debugging
void printNumTransitions() {
	for (int i = 0; i < NUM_STATES; i++) {
		printf("State %d: %d\n",i,states[i].numTransitions);
	}
	for (int i = 0; i< states[98].numTransitions; i++) {
		printf("i: %d c: %c\n",i,states[98].transitions[i].letter);
	}
	printf("\n");
}

/*
 * builds dfa
 */
void initialize() {
	states[0].token = TOKEN_EOF;
	
	for (int i = 1; i <= 53; i++) {
		states[i].token = TOKEN_ID;
	}
	states[54].token = TOKEN_IF;
	states[55].token = TOKEN_ELSE;
	states[56].token = TOKEN_ELSEIF;
	states[57].token = TOKEN_WHILE;
	states[58].token = TOKEN_FOR;
	states[59].token = TOKEN_INT;
	states[60].token = TOKEN_LONG;
	states[61].token = TOKEN_DOUBLE;
	states[62].token = TOKEN_SHORT;
	states[63].token = TOKEN_CHAR;
	states[64].token = TOKEN_RETURN;
	states[65].token = TOKEN_BREAK;
	states[66].token = TOKEN_CONTINUE;
	states[67].token = TOKEN_SIGNED;
	states[68].token = TOKEN_UNSIGNED;
	states[69].token = TOKEN_ID;
	states[70].token = TOKEN_PLUS;
	states[71].token = TOKEN_MINUS;
	states[72].token = TOKEN_ASSIGN;
	states[73].token = TOKEN_SEMICOLON;
	states[74].token = TOKEN_LEFTBRACKET;
	states[75].token = TOKEN_RIGHTBRACKET;
	states[76].token = TOKEN_LEFTPAREN;
	states[77].token = TOKEN_RIGHTPAREN;
	states[78].token = TOKEN_LEFTCURLY;
	states[79].token = TOKEN_RIGHTCURLY;
	states[80].token = TOKEN_GREAT;
	states[81].token = TOKEN_LESS;
	states[82].token = TOKEN_MULT;
	states[83].token = TOKEN_DIV;
	states[84].token = TOKEN_MOD;
	states[85].token = TOKEN_TILDE;
	states[86].token = TOKEN_AT;
	states[87].token = TOKEN_COMMA;
	states[88].token = TOKEN_BITWISEAND;
	states[89].token = TOKEN_BITWISEOR;
	states[90].token = TOKEN_BITWISEXOR;
	states[91].token = TOKEN_QUESTIONMARK;
	states[92].token = TOKEN_EQUAL;
	states[93].token = TOKEN_LESSEQUAL;
	states[94].token = TOKEN_GREATEQUAL;
	states[95].token = TOKEN_AND;
	states[96].token = TOKEN_OR;
	states[97].token = TOKEN_NOT;
	
	states[98].token = TOKEN_CHARCONST;
	states[99].token = TOKEN_STRINGCONST;
	
	states[100].token = TOKEN_NUM;
	states[101].token = TOKEN_NUM; //decimals
	
	states[102].token = TOKEN_WHITESPACE;
	
	states[103].token = TOKEN_WHITESPACE; //comments
	
	for (int i = 0; i < NUM_STATES - 3; i++) {
		addTransition(i, ' ', 102, 1);
		addTransition(i, '\n',102, 1);
		addTransition(i, '\t',102, 1);
		addTransition(i, '\r',102, 1);
	}

	addTransition(102, ' ', 102, 0);
	addTransition(102, '\n', 102, 1);
	addTransition(102, '\r', 102, 0);
	addTransition(102, '\t', 102, 0);

	
	for (int i = 0; i <= 69; i++) {
		addSpecialCharacterTransitions(i);
	}

	for (int i = 0; i < 94; i++) {
		addTransition(98, alphabet[i], 98, 0);
		addTransition(99, alphabet[i], 99, 0);
	}
	states[98].transitions[86].state = 102;
	states[98].transitions[86].delimeter = 1;
	states[99].transitions[85].state = 102;
	states[99].transitions[85].delimeter = 1;

	addSpecialCharacterTransitions(100);
	addSpecialCharacterTransitions(101);
	

	for (int i = 0; i < NUM_STATES - 3; i++) {
		if ((i == 98) || (i == 99) || (i == 103)) continue;
		addTransition(i, '\'', 98, 1);
		addTransition(i, '\"', 99, 1);
	} 

	for (int i = 54; i <= 69; i++) {
		if (i == 55) {
			addTransition(55, 'i', 5, 0);
			addIDTransitions(i, "i", 0);
		}
		else addIDTransitions(i, "", 0);
	}

	for (int i = 70; i <= 97; i++) {
		addIDTransitions(i, "iewfldscrbu",1);
	}
	for (int i = 70; i <= 91; i++) {
		if (i == 83) {
			addTransition(83,'/',103,0);//comments
		}
		addSpecialCharacterTransitions(i);
	}

	//comments
	for (int i = 0; i < numAlphabet - 2;i++) {
		addTransition(103,alphabet[i],103,0);
	}
	addTransition(103,'\n',102,1);
	addTransition(103,'\r',102,1);

	states[72].transitions[51].state = 92;
	states[72].transitions[51].delimeter = 0;
	
	states[81].transitions[51].state = 93;
	states[81].transitions[51].delimeter = 0;

	states[80].transitions[51].state = 94;
	states[80].transitions[51].delimeter = 0;

	states[88].transitions[67].state = 95;
	states[88].transitions[67].delimeter = 0;
	
	states[89].transitions[68].state = 96;
	states[89].transitions[68].delimeter = 0;


	
	addTransition(0, 'i', 1, 0);
	addTransition(0, 'e', 2, 0);
	addTransition(0, 'w', 6, 0);
	addTransition(0, 'f', 10, 0);
	addTransition(0, 'l', 13, 0);
	addTransition(0, 'd', 16, 0);
	addTransition(0, 's', 21, 0);
	addTransition(0, 'c', 25, 0);
	addTransition(0, 'r', 28, 0);
	addTransition(0, 'b', 33, 0);
	addTransition(0, 'u', 47, 0);
	addIDTransitions(0, "iewfldscrbu", 0);
	for (char i = '0'; i <= '9'; i++) {
		addTransition(0, i, 100, 0);
	}

	for (int i = 1; i <= 69; i++) {
		for (char c = '0'; c <= '9'; c++) {
			addTransition(i, c, 69, 0);
		}
	}

	for (char c = '0'; c <= '9'; c++) {
		addTransition(100, c, 100, 0);
		addTransition(101, c, 101, 0);
	}
	addTransition(100, '.', 101, 0);

	for (int i = 70; i <= 97; i++) {
		for (char c = '0'; c <= '9'; c++) {
			if (i == 71) {
				addTransition(71, c, 100, 0);
			}
			else {
				addTransition(i, c, 100, 1);
			}
		}
	}

	addTransition(97, '=', 97, 0);


	addTransition(1, 'n', 12, 0);
	addTransition(1, 'f', 54, 0);
	addIDTransitions(1, "nf", 0);

	addTransition(2, 'l', 3, 0);
	addIDTransitions(2, "l", 0);

	addTransition(3, 's', 4, 0);
	addIDTransitions(3, "s", 0);

	addTransition(4, 'e', 55, 0);
	addIDTransitions(4, "e", 0 );

	addTransition(5, 'f', 56, 0);
	addIDTransitions(5, "f", 0);

	addTransition(6, 'h', 7, 0);
	addIDTransitions(6, "h",0);

	addTransition(7, 'i', 8, 0);
	addIDTransitions(7, "i",0);

	addTransition(8, 'l', 9, 0);
	addIDTransitions(8, "l",0);

	addTransition(9, 'e', 57, 0);
	addIDTransitions(9, "e",0);

	addTransition(10, 'o', 11, 0);
	addIDTransitions(10, "o",0);

	addTransition(11, 'r', 58, 0);
	addIDTransitions(11, "r",0);

	addTransition(12, 't', 59, 0);
	addIDTransitions(12, "t",0);

	addTransition(13, 'o', 14, 0);
	addIDTransitions(13, "o",0);

	addTransition(14, 'n', 15, 0);
	addIDTransitions(14, "n",0);

	addTransition(15, 'g', 60, 0);
	addIDTransitions(15, "g",0);

	addTransition(16, 'o', 17, 0);
	addIDTransitions(16, "o",0);

	addTransition(17, 'u', 18, 0);
	addIDTransitions(17, "u",0);

	addTransition(18, 'b', 19, 0);
	addIDTransitions(18, "b",0);

	addTransition(19, 'l', 20, 0);
	addIDTransitions(19, "l",0);

	addTransition(20, 'e', 61, 0);
	addIDTransitions(20, "e",0);

	addTransition(21, 'h', 22, 0);
	addTransition(21, 'i', 43, 0);
	addIDTransitions(21, "hi",0); 

	addTransition(22, 'o', 23, 0);
	addIDTransitions(22, "o",0);

	addTransition(23, 'r', 24, 0);
	addIDTransitions(23, "r",0);

	addTransition(24, 't', 62, 0);
	addIDTransitions(24, "t",0);

	addTransition(25, 'h', 26, 0);
	addTransition(25, 'o', 37, 0);
	addIDTransitions(25, "ho",0);

	addTransition(26, 'a', 27, 0);
	addIDTransitions(26, "a",0);

	addTransition(27, 'r', 63, 0);
	addIDTransitions(27, "r",0);

	addTransition(28, 'e', 29, 0);
	addIDTransitions(28, "e",0);

	addTransition(29, 't', 30, 0);
	addIDTransitions(29, "t",0);

	addTransition(30, 'u', 31, 0);
	addIDTransitions(30, "u",0);
	
	addTransition(31, 'r', 32, 0);
	addIDTransitions(31, "r",0);

	addTransition(32, 'n', 64, 0);
	addIDTransitions(32, "n",0);

	addTransition(33, 'r', 34, 0);
	addIDTransitions(33, "r",0);
	
	addTransition(34, 'e', 35, 0);
	addIDTransitions(34, "e",0);

	addTransition(35, 'a', 36, 0);
	addIDTransitions(35, "a",0);

	addTransition(36, 'k', 65, 0);
	addIDTransitions(36, "k",0);

	addTransition(37, 'n', 38, 0);
	addIDTransitions(37, "n",0);

	addTransition(38, 't', 39, 0);
	addIDTransitions(38, "t",0);

	addTransition(39, 'i', 40, 0);
	addIDTransitions(39, "i",0);

	addTransition(40, 'n', 41, 0);
	addIDTransitions(40, "n",0);

	addTransition(41, 'u', 42, 0);
	addIDTransitions(41, "u",0);

	addTransition(42, 'e', 66, 0);
	addIDTransitions(42, "e",0);

	addTransition(43, 'g', 44, 0);
	addIDTransitions(43, "g",0);

	addTransition(44, 'n', 45, 0);
	addIDTransitions(44, "n",0);

	addTransition(45, 'e', 46, 0);
	addIDTransitions(45, "e",0);

	addTransition(46, 'd', 67, 0);
	addIDTransitions(46, "d",0);

	addTransition(47, 'n', 48, 0);
	addIDTransitions(47, "n",0);

	addTransition(48, 's', 49, 0);
	addIDTransitions(48, "s",0);

	addTransition(49, 'i', 50, 0);
	addIDTransitions(49, "i",0);

	addTransition(50, 'g', 51, 0);
	addIDTransitions(50, "g",0);

	addTransition(51, 'n', 52, 0);
	addIDTransitions(51, "n",0);

	addTransition(52, 'e', 53, 0);
	addIDTransitions(52, "e",0);

	addTransition(53, 'd', 68, 0);
	addIDTransitions(53, "d",0);

	for (int i = 70; i <= 97; i++) {
		addTransition(i, 'i', 1, 1);
		addTransition(i, 'e', 2, 1);
		addTransition(i, 'w', 6, 1);
		addTransition(i, 'f', 10, 1);
		addTransition(i, 'l', 13, 1);
		addTransition(i, 'd', 16, 1);
		addTransition(i, 's', 21, 1);
		addTransition(i, 'c', 25, 1);
		addTransition(i, 'r', 28, 1);
		addTransition(i, 'b', 33, 1);
		addTransition(i, 'u', 47, 1);
	}
	for (int i = 0; i < states[0].numTransitions; i++) {
		addTransition(102, states[0].transitions[i].letter, states[0].transitions[i].state, 1);
	}

	/*
1.     i
2.     e
3.     el
4.      els
5.      elsei
6.      w
7.	wh
8.	whi
9.	whil
10.	f
11.	fo
12.	in
13.	l
14.	lo
15.	lon
16.	d
17.	do
18.	dou
19.	doub
20.	doubl
21.	s
22.	sh
23.	sho
24.	shor
25.	c
26.	ch
27.	cha
28.	r
29.	re
30.	ret
31.	retu
32.	retur
33.	b
34.	br
35.	bre
36.	brea
37.	co
38.	con
39.	cont
40.	conti
41.	contin
42.	continu
43.	si
44.	sig
45.	sign
46.	signe
47.	u
48.	un
49.	uns
50.	unsi
51.	unsig
52.	unsign
53.	unsigne */
}
