#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "dfa.c"

int lineNum = 1;
unsigned long tokenNum = 0;
extern struct State states[];//defined in dfa.c

void addChar(char *line, char c, int n) {
	line = realloc(line,sizeof(char)*n);
	line[n-1] = c;
}

/*
 * adds token with its given token type into the token array
 */
void addToken(token_t tokenType, char *token, unsigned long lineNum, unsigned long tokenIndex) {
	tokens = realloc(tokens,sizeof(Token)*(numTokens+1));
	Token *t = malloc(sizeof(Token));
	t->token = token;
	t->tokenType = tokenType;
	t->line = lineNum;
	t->tokenIndex = tokenIndex;
	tokens[numTokens] = t;
	numTokens++;
		
}

/*
 * Handles the errors caused by a missing transition from a state meaning that the language does not accept
 * the combinations of letters.
 */
void transitionError(int startState, char letter) {
	if (letter == '.') {
		if (startState == 101) printf("Decimals can only have 1 '.' character\n");
		else printf("Character '.' can only be used within a valid decimal token\n");
	}
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
	//exit(1);
	transitionError(startState, letter);
	return -1;
}


/*
 * SCANNER
 * given the current string, length of string, new letter, and current state
 * 1. if the letter transition to the new state is a delimeter that means that current string is a token
 *    get the token (which is in the state of the dfa) and add this token type and string to the token array
 * 2. if the letter transition to the new state is NOT a delimeter add it to the current string
 */
char* scan(char *curString, int *curStringLength, char c, int *state) {
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
		printf("tokeNum: %lu\n",tokenNum);
		addToken(states[currentState].token, token,lineNum, tokenNum++);
		curString = realloc(curString, sizeof(char));
		curString[0] = c;
		*curStringLength = 1;
		if (c == '\n') {
			lineNum++;
			tokenNum = 0;
		}
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
		printf("%15lu | ",tokens[i]->line);
		printf("%15lu | ",tokens[i]->tokenIndex);
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

	//use scanner to scan every character in input file
	while (1) {
		c = fgetc(file);
		if (feof(file)) {
			addToken(states[state].token, string, lineNum, tokenNum);
			break;
		}
		string = scan(string, &stringLength, c, &state);
		//printf("%s\n",string);
	}
	addToken(TOKEN_DOLLAR, "$", -1, -1);
	rewind(file);
	
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while ((read = getline(&line, &len, file)) != -1) {
		lines = realloc(lines, sizeof(char*)* ++numLines);
		lines[numLines - 1] = strdup(line);
	}
	return 0;
}
