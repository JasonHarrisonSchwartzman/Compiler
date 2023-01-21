#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "dfa.h"
#include "dfa.c"

int lineNum = 1;
extern struct State states[];//defined in dfa.c

/*
 * adds token with its given token type into the token array
 */
void addToken(token_t tokenType, char *token, int lineNum) {
	tokens = realloc(tokens,sizeof(Token)*(numTokens+1));
	Token *t = malloc(sizeof(Token));
	t->token = token;
	t->tokenType = tokenType;
	t->line = lineNum;
	tokens[numTokens] = t;
	numTokens++;
		
}
/*
 * SCANNER
 * given the current string, length of string, new letter, and current state
 * 1. if the letter transition to the new state is a delimeter that means that current string is a token
 *    get the token (which is in the state of the dfa) and add this token type and string to the token array
 * 2. if the letter transition to the new state is NOT a delimeter add it to the current string
 */
char* scan(char *curString, int *curStringLength, char c, int *state) {
	if (c == '\n') lineNum++;
	//printf("Start state: %d\n",*state);
	int currentState = *state;
	if (!takeTransition(*state,c,state)) {
		//printf("New state: %d\n",*state);
		curString = realloc(curString, sizeof(char) * (*curStringLength + 1));
		curString[(*curStringLength)++] = c;	
		return curString;
	}
	else {
		curString = realloc(curString, sizeof(char) * (*curStringLength + 1));
		curString[(*curStringLength)++] = '\0';
		char *token = malloc(sizeof(char) * *curStringLength);
		strcpy(token,curString);
		//printf("Generated token: %s\n",curString);
		addToken(states[currentState].token, token,lineNum);
		curString = realloc(curString, sizeof(char));
		curString[0] = c;
		*curStringLength = 1;
		//printf("Did i do this right: %s\n",tokens[0]->token);
		return curString;
	}
}
/*
 * No mem leeks
 */
void freeTokens() {
	for (int i = 0; i < numTokens; i++) {
		free(tokens[i]->token);
		free(tokens[i]);
	}
	free(tokens);
}
/*
 * Prints tokens array (For debugging)
 */
void printTokens() {
	printf("NUM TOKENS: %d\n",numTokens);
	for (int i = 0; i < numTokens; i++) {
		if (tokens[i]->tokenType == TOKEN_WHITESPACE) continue;
		printf("%15s | ",tokens[i]->token);
		printToken(tokens[i]->tokenType);
		printf("\n");
	}
}

int scanner(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: ./scanner [file]\n");
		return 0;
	}
	FILE *file;
	file = fopen(argv[1],"r");
	char c;
	int state = 0;
	char *string = malloc(sizeof(char));
	int stringLength = 0;
	initialize();//creates dfa

	/*for (int i = 0; i < states[0].numTransitions; i++) {
		printf("%c\n",states[0].transitions[i].letter);
	}*/

	//use scanner to scan every character in input file
	while (1) {
		c = fgetc(file);
		if (feof(file)) {
			addToken(states[state].token, string, lineNum);
			break;
		}
		string = scan(string, &stringLength, c, &state);
		//printf("%s\n",string);
	}
	addToken(TOKEN_DOLLAR, "$", -1);
	return 0;
}
