#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "syntaxtree.h"
#include "parserinit.c"
#include "parser.h"

/*
 * How the stack is set up for SLR Parsing:
 * Each element in the stack contains an instance followed by a nonterminal variable or token
 * [0 ID] [5 INT] [7 x] //reading id then reading int (x means empty in the stack struct)
 * [0 E]  [10 x]       //reducing to E (x means empty in the stack struct)
 * (this isn't a real part of the grammar its just an example
 */

/*
 * Prints stack with integers instead of meaningful characters I will probably change this eventually (also only used for debugging)
 */
void printStack() {
	for (int i = 0; i <= stackTopPointer; i++) {
		printf("%d ", stack[i]->instance);
		if (stack[i]->token != NULL) {
			printToken(stack[i]->token->tokenType);
			printf(" ");
			//printf("%d ",stack[i]->token->tokenType);
		}
		if (stack[i]->var != 0) {
			printToken(stack[i]->var);
			printf(" ");
			//printf("%d ",stack[i]->var);
		}
	}
	printf("\n");
}
/*
 * Pushes an instance, a token, and a var (
 * will only push two at a time since the second element of stack item struct is either a token or a var
 * The pointer is used for the syntax tree
 */
void push(token_t instance, Token *token, token_t var, void *ptr) {
	if (var != -1) {
		stack[stackTopPointer]->var = var;
	}
	if (token != NULL) {
		stack[stackTopPointer]->token = token;
	}
	if (stackTopPointer != -1) stack[stackTopPointer]->ptr = ptr;
	if (instance != -1) {
		stack = realloc(stack,sizeof(void*) * (stackTopPointer + 2));
		stack[++stackTopPointer] = calloc(1,sizeof(struct StackItem));
		stack[stackTopPointer]->instance = instance;
	}
}
/*
 * pops and return a pointer to a syntax tree struct if popping a token or var
 * pops and returns NULL if popping an instance
 * Remember instead of a stack containing an array of individual elements it contains structures containing two elements
 * i.e. [INSTANCE VAR/TOKEN] so this function pops the last non-NULL value
 * there are multiple structs within the tree and its void because you dont know which one
 */
void *pop() {
	if ((stack[stackTopPointer]->token == NULL) && (stack[stackTopPointer]->var == 0)) {
		stack = realloc(stack,sizeof(StackItem) * (stackTopPointer--));
		return NULL;
	}
	else {
		if (stack[stackTopPointer]->ptr == NULL) {
			printf("Variable contains a NULL pointer: ");
			printToken(stack[stackTopPointer]->var);
			printf("\n");
		}
		stack[stackTopPointer]->token = NULL;
		stack[stackTopPointer]->var = 0;
		return stack[stackTopPointer]->ptr;
	}
}

void parseError() {
	
}

/*
 * Simply pushes a stack item onto the stack
 */
void shift(token_t instance, Token *token, token_t var, void *ptr) {
	printf("Shift \n");
	push(instance, token, var, token);
	tokenIndex++;
}

/*
 * The semantic rules help build the syntax tree
 * the param array input contains the list of void pointers pointing to structs within syntax tree
 * rule is gotten from the SLR parsing table
 */
void *callSemanticRule(void *param[], int rule) {
	switch (rule) {
		case 1: 
			return addDeclarations(param[0],param[1]);
		case 2:
			return param[0];
		case 3:
			return addDeclaration(FUNC,param[0],NULL);
		case 4:
			return addDeclaration(VAR,NULL,param[0]);
		case 5: 
			return addVarDecl(param[0],param[1]);
		case 6:
			return addType(param[1],param[0]);
		case 7:
			return addType(param[0],NULL);
		case 8:
			return addSign(SIGNED);
		case 9:
			return addSign(UNSIGNED);
		case 10:
			return addDataType(INT);
		case 11:
			return addDataType(LONG);
		case 12:
			return addDataType(SHORT);
		case 13:
			return addDataType(CHAR);
		case 14:
			return addDataType(DOUBLE);
		case 15:
			return addDecl(param[0],NULL);
		case 16:
			return addDecl(param[0],addExpr(NULL,NULL,addEval(VALUE,param[2],NULL,NULL,-1,-1,NULL,((struct Value*)param[2])->line)));
		case 17:
			return addName(((struct Token*)param[0])->token,((struct Token*)param[2])->token,-1,NULL,((struct Token*)param[0])->line);
		case 18:
			return addName(((struct Token*)param[0])->token,NULL,-1,NULL,((struct Token*)param[0])->line);
		case 19:
			return addName(((struct Token*)param[1])->token,NULL,1,NULL,((struct Token*)param[1])->line);
		case 20:
			return addName(((struct Token*)param[1])->token,((struct Token*)param[3])->token,-1,NULL,((struct Token*)param[1])->line);
		case 21:
			return addValue(NUM,((struct Token*)param[0])->token,((struct Token*)param[0])->line); //numbers and decimals and what not
		case 22:
			return addValue(CHARCONST,((struct Token*)param[0])->token,((struct Token*)param[0])->line);
		case 23:
			return addValue(STRINGCONST,((struct Token*)param[0])->token,((struct Token*)param[0])->line);
		case 24:
			return addFuncDecl(param[6],param[0],param[3],((struct Token*)param[1])->token,((struct Token*)param[1])->line);
		case 25:
			return addFuncDecl(param[5],param[0],NULL,((struct Token*)param[1])->token,((struct Token*)param[1])->line);
		case 26:
			return addParam(param[3],param[0],param[1]);
		case 27:
			return addParam(NULL,param[0],param[1]);
		case 28:
			return addFuncDecl(NULL,param[0],param[3],((struct Token*)param[1])->token,((struct Token*)param[1])->line);
		case 29:
			return addFuncDecl(NULL,param[0],NULL,((struct Token*)param[1])->token,((struct Token*)param[1])->line);
		case 30:
			return param[0];
		case 31:
			return addStatements(param[0],param[1]);
		case 32:
			return param[0];
		case 33:
			return addStatement(IF,NULL,NULL,NULL,NULL,param[0]);
		case 34:
			return addStatement(((struct Loop*)param[0])->stmt, NULL, NULL, NULL, param[0],NULL);
		case 35:
			return addStatement(RETURN,NULL,NULL,param[0],NULL,NULL);
		case 36:
			return addStatement(((struct FunctionStatement*)param[0])->stmt,((struct FunctionStatement*)param[0])->vardecl,((struct FunctionStatement*)param[0])->funccall,NULL,NULL,NULL);
		case 37:
			return addFunctionStatement(FUNCCALL,param[0],NULL);
		case 38:
			return addFunctionStatement(DECLARATION,NULL,addVarDecl(param[0],param[1]));
		case 39:
			return addDecl(param[0],NULL);
		case 40:
			return addDecl(param[0],param[2]);
		case 41:
			return addExpr(NULL,NULL,param[0]);
		case 42:
			return addExpr(param[0],param[1],param[2]);
		case 43:
			return addExpr(param[0],param[1],param[2]);
		case 44:
			return addOperation(PLUS);
		case 45:
			return addOperation(MULT);
		case 46:
			return addOperation(MOD);
		case 47:
			return addOperation(DIV);
		case 48:
			return addOperation(MINUS);
		case 49:
			return addOperation(BITWISEAND);
		case 50:
			return addOperation(BITWISEOR);
		case 51:
			return addOperation(BITWISEXOR);
		case 52:
			return addOperation(EQUAL);
		case 53:
			return addOperation(LESSEQUAL);
		case 54:
			return addOperation(GREATEQUAL);
		case 55:
			return addOperation(GREAT);
		case 56:
			return addOperation(LESS);
		case 57:
			return addOperation(AND);
		case 58:
			return addOperation(NOT);
		case 59:
			return addOperation(OR);
		case 60:
			return addEval(VALUE,param[0],NULL,NULL,-1,-1,NULL,((struct Value*)param[0])->line);
		case 61:
			return addEval(DEREF,NULL,NULL,((struct Token*)param[1])->token,1,-1,NULL,((struct Token*)param[1])->line);
		case 62:
			return addEval(REF,NULL,NULL,((struct Token*)param[1])->token,-1,1,NULL,((struct Token*)param[1])->line);
		case 63:
			return addEval(ID,NULL,NULL,((struct Token*)param[0])->token,-1,-1,NULL,((struct Token*)param[0])->line);
		case 64:
			return addEval(ARRAYINDEX,NULL,param[2],((struct Token*)param[0])->token,-1,-1,NULL,((struct Token*)param[0])->line);
		case 65:
			return addEval(FUNCRETURN,NULL,NULL,NULL,-1,-1,param[0],((struct FunctionCall*)param[0])->line);
		case 66:
			return addFuncCall(((struct Token*)param[0])->token,param[2],((struct Token*)param[0])->line);
		case 67:
			return addFuncCall(((struct Token*)param[0])->token,NULL,((struct Token*)param[0])->line);
		case 68:
			return addFuncArgs(param[0],param[2]);
		case 69:
			return addFuncArgs(param[0],NULL);
		case 70:
			return param[0];
		case 71:
			return param[0];
		case 72:
			return addLoop(WHILE,NULL,NULL,param[2],param[5]);
		case 73:
			return addLoop(FOR,addVarDecl(param[2],param[3]),param[7],param[5],param[10]);
		case 74:
			return NULL;//Not a rule anymore
		case 75:
			return addVarDecl(NULL,addDecl(addName(((struct Token*)param[0])->token,NULL,-1,NULL,((struct Token*)param[0])->line),param[2]));
		case 76:
			return addCondStatements(param[0],param[1],param[2]);
		case 77:
			return addCondStatements(param[0],param[1],NULL);
		case 78:
			return addCondStatements(param[0],NULL,param[1]);
		case 79:
			return param[0];
		case 80:
			return addCondStatement(param[2],param[5],IF,NULL);
		case 81:
			return addCondStatement(param[2],param[5],ELSEIF,NULL);
		case 82:
			return addCondStatement(param[2],param[5],ELSEIF,param[7]);
		case 83:
			return addCondStatement(NULL,param[2],ELSE,NULL);
		case 84:
			return param[1];
		case 85:
			return addStatement(BREAK,NULL,NULL,NULL,NULL,NULL);
		case 86:
			return addStatement(CONTINUE,NULL,NULL,NULL,NULL,NULL);
		case 87:
			return addFunctionStatement(ASSIGNMENT,NULL,addVarDecl(NULL,addDecl((struct Name*)param[0],(struct Expression*)param[2])));
		case 88:
			return addName(((struct Token*)param[0])->token,NULL,-1,NULL,((struct Token*)param[0])->line);
		case 89:
			return addName(((struct Token*)param[0])->token,NULL,-1,param[2],((struct Token*)param[0])->line);
		default:
			printf("Rule not found ERROR\n");
			return NULL;
			break;
	}
	return NULL;
}

/*
 * Given a rule from the SLR Parsing Table take all the needed void pointer params and call the semantic rule function
 * pops the number of items that make up a rule in the grammar
 */
void reduce(int rule) {
	printf("Reduce by %d\n",rule);
	int ruleLength = rules[rule].length;
	printf("Rule length %d\n",ruleLength);
	void *pointers[ruleLength];
	int num = ruleLength - 1;
	void *p;
	for (int i = 0; i < ruleLength * 2; i++) {
		if ((p = pop()) != NULL) {
			pointers[num--] = p;
		}
	}
	if (num != -1) printf("Did not fill up pointer array ERROR remaining: %d\n",num+1);
	token_t var = varTokens[rules[rule].var-(TOTAL_TOKENS+NUM_INSTANCES)]->tokenType;
	token_t instance = instanceTokens[instances[stack[stackTopPointer]->instance].gotoAction[var-(TOTAL_TOKENS+NUM_INSTANCES)]]->tokenType;

	void *ptr = callSemanticRule(pointers,rule);
	push(instance,NULL,var,ptr);
}
/*
 * The main parsing function that utilizes the SLR Parsing technique and initializes the syntax tree
 */
int parse() {
	push(0,NULL,-1,NULL);
	printStack();
	while(1) {
		printf("Reading token %d: ",tokenIndex);
		printToken(tokens[tokenIndex]->tokenType);
		printf("\n");
		if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			printf("Skipping whitespace!\n");
			tokenIndex++;
			continue;
		}
		token_t state = stack[stackTopPointer]->instance;
		int actionIndex = tokens[tokenIndex]->tokenType-NUM_INSTANCES;
		Step step = instances[state].actions[actionIndex].step;
		if (step == STEP_ACCEPT) {
			syntaxTree = stack[0]->ptr;
			return 1;
		}
		else if (step == STEP_SHIFT) {
			shift(instanceTokens[instances[state].actions[actionIndex].instance]->tokenType, tokens[tokenIndex],-1,tokens[tokenIndex]);
		}
		else if (step == STEP_REDUCE) {
			reduce(instances[state].actions[actionIndex].instance);
		}
		else {
			//debug
			printLine(tokens[tokenIndex]->line);
			printf("Token not found %s NUM: %d Token type: ",tokens[tokenIndex]->token,tokens[tokenIndex]->tokenType);
			printToken(tokens[tokenIndex]->tokenType);
			printf("\n");
			return 0;
		}
		printStack();
	}
	return -1;
}
