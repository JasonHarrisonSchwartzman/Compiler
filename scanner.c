#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "dfa.c"

/**
 * Utilizing the DFA created in dfa.c, the scanner will walk through the DFA taking transitions based on the input string (the program).
 * The scanner will accumulate a string based on an input until a delimeter is read. The delimeter is a field of the transition struct. 
 * Once a delimeter is read the current state's token is added to the token stream. 
 * When an error is found, meaning there is not a valid transition for the character being read, the compiler will print the line it occured on.
 * These invalid characters are ignored and the scanner continues to scan the input to try to find more errors.
 * However, the compiler will not proceed to the next phase (parsing).
 */

int scannerPass = 1;//if there are no errors this won't change else it'll be 0

unsigned long lineNum = 1;//current line number being read
unsigned long tokenNum = 0;//current token index within line

extern struct State states[];//defined in dfa.c

/*
 * adds token with its given token type into the token array
 */
void addToken(token_t tokenType, char *token, unsigned long lineNum, unsigned long tokenIndex) {
	tokens = realloc(tokens,sizeof(Token)*(numTokens+1));
	Token *t = calloc(1,sizeof(Token));
	t->token = token;
	t->tokenType = tokenType;
	t->line = lineNum;
	t->tokenIndex = tokenIndex;
	tokens[numTokens] = t;
	numTokens++;
		
}

//tabs, newlines, spaces, and carriage return
int isWhitespace(char *c) {
	return ( c[0] == ' ' || c[0] == '\n' || c[0] == '\r' || c[0] == '\t' || (strcmp(c, "") == 0) || (c == NULL) || (strcmp(c, "\t") == 0) || (strcmp(c, "\n") == 0) || (strcmp(c, "\r") == 0) || (strcmp(c, " ") == 0));
}

/*
 * Handles the errors caused by a missing transition from a state meaning that the language does not accept
 * the combinations of letters. However, it should be noted that invalid characters will be added to the current string, in other words they are ignored.
 */
void transitionError(int startState, char letter, char *curString, int curLength) {
	scannerPass = 0;

	char *c = calloc(1,sizeof(char)*curLength);
	strncpy(c, curString, curLength);

	//general error message
	fprintf(stderr,"Adding character '%c' to string '%s' will result in an invalid token\n",letter, (curLength == 0) || isWhitespace(c) ? "" : c);

	//more specific error message
	if (letter == '.') {
		if (startState == 101) fprintf(stderr,"Decimals can only have 1 '.' character\n");
		else fprintf(stderr,"Character '.' can only be used within a valid decimal token\n");
	}

	printLine(lineNum);
	for (int i = 0; i < lineNum/10 + 4; i++) fprintf(stderr," ");
	unsigned long tokenIndex;
	//find the current line number the error is on
	for (tokenIndex = 0; tokenIndex < numTokens; tokenIndex++) {
		if (tokens[tokenIndex]->line == lineNum) {
			break;
		}
	}
	//within the line number get the length of each token to print out spaces to line up with the invalid character
	for ( ; tokenIndex < numTokens; tokenIndex++) {
		printToken(tokens[tokenIndex]->tokenType);
		/*if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			printf(" ");
		}
		if (strcmp(tokens[tokenIndex]->token,"\t") == 0) {
			printf("\t");
			continue;
		}
		else {
			for (int i = 0; i < strlen(tokens[tokenIndex]->token); i++) {
				printf(" ");
			}
		}*/
	}
	fprintf(stderr,"|");
	//if (curString[0] == '\t') printf("\t");//if a tab character is in the token stream a tab character must be printed to cover the same spacing
	for (int i = 0; i < curLength -1 ; i++) {
		fprintf(stderr," ");
	}
	fprintf(stderr,"^");//this should point to the invalid character in the line above
	fprintf(stderr,"\n\n");
}

/*
 * returns if transition taken is a delimeter (1 if it is) (0 if it is not) 
 * if transition DNE return -1 and calls error handler
 */
int takeTransition(int startState, char letter, int *endState, char *curString, int curLength) {
	for (int i = 0; i < states[startState].numTransitions; i++) {
		if (states[startState].transitions[i].letter == letter) {
			*endState = states[startState].transitions[i].state;
			return states[startState].transitions[i].delimeter;
		}
	}
	transitionError(startState, letter, curString, curLength);
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
	if (!takeTransition(*state,c,state,curString,*curStringLength)) { //illegal transition
		//but add the character anyway to the token string 
		curString = realloc(curString, sizeof(char) * (*curStringLength + 1));
		curString[(*curStringLength)++] = c;	
		return curString;
	}
	else { //legal transition
		//adding a null terminating character
		curString = realloc(curString, sizeof(char) * (*curStringLength + 1));
		curString[(*curStringLength)++] = '\0';

		char *token = calloc(1,sizeof(char) * *curStringLength);
		strcpy(token,curString);

		//tokenifying the current string
		addToken(states[currentState].token, token,lineNum, tokenNum++);

		//resetting current string to only contain the new character being read
		curString = realloc(curString, sizeof(char));
		curString[0] = c;
		*curStringLength = 1;
		
		//resetting line num and token index
		if (c == '\n') {
			lineNum++;
			tokenNum = 0;
		}
		return curString;
	}
}

/*
 * Note to future self: You have not taken care of memory leaks
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
 * Prints tokens array in a somewhat organized manner for debugging
 */
void printTokens() {
	printf("NUM TOKENS: %d\n",numTokens);
	for (int i = 0; i < numTokens; i++) {
		//if (tokens[i]->tokenType == TOKEN_WHITESPACE) continue; commenting this out for debugging
		printf("%15s | ",tokens[i]->token);
		printToken(tokens[i]->tokenType);
		printf("%15lu | ",tokens[i]->line);
		printf("%15lu | ",tokens[i]->tokenIndex);
		printf("\n");
	}
}

/*
 * Scans input file generating tokens and adding them to token stream.
 */
int scanner(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr,"Missing run-time args. \nUsage: ./jcc [src] [bin]\n");
		exit(1);
	}
	FILE *file;
	file = fopen(argv[1],"r"); 
	if (!file) {
		fprintf(stderr,"Could not read file %s. Please try again.\n",argv[1]);
		exit(1);
	}
	char c;//character that will be readf
	int state = 0;//start state

	char *string = calloc(1,sizeof(char));//current characters being read
	int stringLength = 0;

	initialize();//creates dfa

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	//generating the lines array containing all lines from the program (used for printing the line an error occurred)
	while ((read = getline(&line, &len, file)) != -1) {
		lines = realloc(lines, sizeof(char*)* ++numLines);
		lines[numLines - 1] = strdup(line);
	}
	rewind(file);

	//use scanner to scan every character in input file
	while (1) {
		c = fgetc(file);
		if (feof(file)) {
			addToken(states[state].token, string, lineNum, tokenNum);//generate token for whatever is leftover at the EOF
			break;
		}
		string = scan(string, &stringLength, c, &state);
	}
	addToken(TOKEN_DOLLAR, "$", -1, -1);//adding empty token to token stream (used in parser)
	return 0;
}
