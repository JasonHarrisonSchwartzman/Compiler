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
		if (i == 0 || i == 9 || i == 10 || (i >= 32 && i <= 126)) {
			char *ptr = strchr(blacklist, i);
			if (ptr) {
				addTransition(startState, i, 9, 0);
			}
		}
	}
}

/*
 * builds dfa
 */
void initialize() {
	states[0].token = TOKEN_EOF; //Start state
	states[2].token = TOKEN_IF;
	states[15].token = TOKEN_ELSE;
	states[16].token = TOKEN_FOR;
	states[66].token = TOKEN_WHILE;
	states[17].token = TOKEN_INT;
	states[17].token = TOKEN_DOUBLE;
	states[39].token = TOKEN_CHAR;
	states[56].token = TOKEN_LONG;
	states[61].token = TOKEN_SHORT;
	states[70].token = TOKEN_SIGNED;
	states[78].token = TOKEN_UNSIGNED;
	states[21].token = TOKEN_RETURN;
	states[44].token = TOKEN_BREAK;
	states[52].token = TOKEN_CONTINUE;
	states[24].token = TOKEN_FUNC;
	states[65].token = TOKEN_VOID;
	states[83].token = TOKEN_STRUCT;
	states[6].token = TOKEN_NUM;
	states[9].token = TOKEN_ID;

	// createIDTransitions(0, "iefwdclsuvrb");
	// createIDTransitions(1, "f");
	// createIDTransitions(2, "");
	// createIDTransitions(3, "l");
	// createIDTransitions(4, "s");
	// createIDTransitions(5, "e");
	// createIDTransitions(6, "");
	// createIDTransitions(7, "o");
	// createIDTransitions(8, "r");
	// // createIDTransitions(9, "");
	// createIDTransitions(10, "t");
	// createIDTransitions(11, "");
	// createIDTransitions(12, "");
	// createIDTransitions(13, "e");
	// createIDTransitions(14, "t");
	// // createIDTransitions(15, "e");
	// // createIDTransitions(16, "t");
	// createIDTransitions(17, "");

	//if
	addTransition(0,'i',1,0);
	addTransition(1,'f',2,0);
	//else
	addTransition(0,'e',3,0);
	addTransition(3,'l',4,0);
	addTransition(4,'s',5,0);
	addTransition(5,'e',6,0);
	//for
	addTransition(0,'f',7,0);
	addTransition(7,'o',8,0);
	addTransition(8,'r',16,0);
	//while
	addTransition(0,'w',62,0);
	addTransition(62,'h',63,0);
	addTransition(63,'i',64,0);
	addTransition(64,'l',65,0);
	addTransition(65,'e',66,0);
	//int 
	addTransition(1,'n',10,0);
	addTransition(10,'t',17,0);
	//double
	addTransition(0,'d',25,0);
	addTransition(25,'o',26,0);
	addTransition(26,'u',27,0);
	addTransition(27,'b',28,0);
	addTransition(28,'l',29,0);
	addTransition(29,'e',30,0);
	//char
	addTransition(0,'c',36,0);
	addTransition(36,'h',37,0);
	addTransition(37,'a',38,0);
	addTransition(38,'r',39,0);
	//long
	addTransition(0,'l',53,0);
	addTransition(53,'o',54,0);
	addTransition(54,'n',55,0);
	addTransition(55,'g',56,0);
	//short
	addTransition(0,'s',57,0);
	addTransition(57,'h',58,0);
	addTransition(58,'o',59,0);
	addTransition(59,'r',60,0);
	addTransition(60,'t',61,0);
	//signed
	addTransition(57,'i',66,0);
	addTransition(66,'g',67,0);
	addTransition(67,'n',68,0);
	addTransition(68,'e',69,0);
	addTransition(69,'d',70,0);
	//unsigned
	addTransition(0,'u',71,0);
	addTransition(71,'n',72,0);
	addTransition(72,'s',73,0);
	addTransition(73,'i',74,0);
	addTransition(74,'g',75,0);
	addTransition(75,'n',76,0);
	addTransition(76,'e',77,0);
	addTransition(77,'d',78,0);
	//return
	addTransition(0,'r',13,0);
	addTransition(13,'e',14,0);
	addTransition(14,'t',18,0);
	addTransition(18,'u',19,0);
	addTransition(19,'r',20,0);
	addTransition(20,'n',21,0);
	//break
	addTransition(0,'b',40,0);
	addTransition(40,'r',41,0);
	addTransition(41,'e',42,0);
	addTransition(42,'a',43,0);
	addTransition(43,'k',44,0);
	//continue
	addTransition(0,'c',45,0);
	addTransition(45,'o',46,0);
	addTransition(46,'n',47,0);
	addTransition(47,'t',48,0);
	addTransition(48,'i',49,0);
	addTransition(49,'n',50,0);
	addTransition(50,'u',51,0);
	addTransition(51,'e',52,0);
	//func
	addTransition(7,'u',22,0);
	addTransition(22,'n',23,0);
	addTransition(23,'c',24,0);
	addTransition(6,';',11,1); //testing	
	addTransition(11,'\n',12,1); //still testing
	//void
	addTransition(0,'v',62,0);
	addTransition(62,'o',63,0);
	addTransition(63,'i',64,0);
	addTransition(64,'d',65,1);
	//struct
	addTransition(57,'t',79,0);
	addTransition(79,'r',80,0);
	addTransition(80,'u',81,0);
	addTransition(81,'c',82,0);
	addTransition(82,'t',83,0);

	//numbers
	for (int i = 48; i <= 57; i++) {
		addTransition(0,i,84,0);
		addTransition(84,i,84,0);
	}	

}

