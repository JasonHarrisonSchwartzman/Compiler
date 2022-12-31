#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "scanner.c"
#define NUM_RULES 87
#define NUM_ACTIONS 9
#define NUM_GOTO 7
#define NUM_INSTANCES 24
#define TOTAL_TOKENS 26

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

struct Rule {
	token_t var;
	token_t *symbols;//tokens and variables
	int length;
} Rule;

struct Instance {
	struct Action actions[NUM_ACTIONS];
	int gotoAction[NUM_GOTO];
};

struct Instance instances[NUM_INSTANCES];

struct Token *instanceTokens[NUM_INSTANCES];//instance tokens (for stack)
struct Token *varTokens[NUM_GOTO];//variable (nonterminal) tokens (for stack)
extern struct Token **tokens;//input of tokens
extern int numTokens;
int tokenIndex = 0;//for reading input

struct Rule rules[NUM_RULES];//grammar

struct Token **stack;
int stackCapacity = 0;
int stackTopPointer = -1;//points to top of the stack

void addInstanceAction(int instanceNum, struct Action action, int actionNum) {
	instances[instanceNum].actions[actionNum].step = action.step;
	instances[instanceNum].actions[actionNum].instance = action.instance;
}
void addInstanceGoto(int instanceNum, int gotoNum, int gotoInstance) {
	instances[instanceNum].gotoAction[gotoNum] = gotoInstance;
}

void initializeInstances() {
	//EXAMPLE
	/*struct Action action0id = { STEP_SHIFT, 5 };
	struct Action action0int = { STEP_SHIFT, 6 };
	struct Action action0long = { STEP_SHIFT, 7 };
	addInstanceAction(0,action0id,1);
	addInstanceAction(0,action0int,6);
	addInstanceAction(0,action0long,7);
	addInstanceGoto(0,0,1);
	addInstanceGoto(0,1,2);
	addInstanceGoto(0,2,3);
	addInstanceGoto(0,3,4);
	*/


}

void push(Token *token) {
	stack = realloc(stack,sizeof(Token) * (stackTopPointer + 2));
	stack[++stackTopPointer] = token;
}
void pop() {
	stack = realloc(stack,sizeof(Token) * (stackTopPointer--));
}

Token *getTopOfStack() {
	return stack[stackTopPointer];
}

Token *getSecondTopOfStack() {
	return stack[stackTopPointer-1];
}

void freeStack() {
	free(stack);
}

void printToken(token_t token);

void createInstanceAndVarTokens() {
	for (int i = 0; i < NUM_INSTANCES; i++) {
		instanceTokens[i] = malloc(sizeof(Token));
		instanceTokens[i]->tokenType = i;
	}
	for (int i = 0; i < NUM_GOTO; i++) {
		varTokens[i] = malloc(sizeof(Token));
		varTokens[i]->tokenType = NUM_INSTANCES + TOTAL_TOKENS + i; 
	}
}

void freeInstanceAndVarTokens() {
	for (int i = 0; i < NUM_INSTANCES; i++) {
		free(instanceTokens[i]);
	}
	for (int i = 0; i < NUM_GOTO; i++) {
		free(varTokens[i]);
	}
}
void printToken(token_t token) {
	char *str;
	switch(token) {
		case TOKEN_NUM:
			str = "num";
			break;
		case TOKEN_ID:
			str = "id";
			break;
		case TOKEN_PLUS:
			str = "+";
			break;
		case TOKEN_MINUS:
			str = "-";
			break;
		case TOKEN_ASSIGN:
			str = "=";
			break;
		case TOKEN_SEMICOLON:
			str = ";";
			break;
		case TOKEN_INT:
			str = "int";
			break;
		case TOKEN_LONG:
			str = "long";
			break;
		case TOKEN_DOLLAR:
			str = "$";
			break;
		case TOKEN_EOF:
			str = "eof";
			break;
		case TOKEN_WHITESPACE:
			str = "[SPACE]";
			break;
		case TOKEN_SIGNED:
			str = "signed";
			break;
		default:
			str = "ERROR VAR NOT FOUND";
			break;
	}
	printf("%s",str);
}

void addRule(int item, token_t var, int length, token_t symbol[length]) {
	rules[item].var = var;
	rules[item].length = length;
	rules[item].symbols = malloc(sizeof(token_t) * length);
	for (int i = 0; i < length; i++) {
		rules[item].symbols[i] = symbol[i];
	}
}

void printRule(struct Rule rule) {
	printToken(rule.var);
	printf("->");
	for (int i = 0; i < rule.length; i++) {
		printf(" ");
		printToken(rule.symbols[i]);
	}
	printf("\n");
}

void printRules() {
	printf("----------GRAMMAR-----------\n");
	for (int i = 0; i < NUM_RULES; i++) {
		printRule(rules[i]);
	}
}

void shift(Token *token, Token *instance) {
	printf("Shift \n");
	push(token);
	push(instance);
	tokenIndex++;
}
void printStack();

void reduce(int rule) {
	printf("Reduce by %d\n",rule);
	for (int i = 0; i < rules[rule].length * 2; i++) {
		pop();
	}
	push(varTokens[rules[rule].var-(TOTAL_TOKENS+NUM_INSTANCES)]);
	push(instanceTokens[instances[getSecondTopOfStack()->tokenType].gotoAction[getTopOfStack()->tokenType-(TOTAL_TOKENS+NUM_INSTANCES)]]);
}

void printStack() {
	for (int i = 0; i <= stackTopPointer; i++) {
		printf("%d ", stack[i]->tokenType);
	}
	printf("\n");
}

int parse() {
	push(instanceTokens[0]);
	printf("Beggining parser...\n");
	while(1) {
		//ignoring whitespace
		printf("Reading token %d: ",tokenIndex);
		printToken(tokens[tokenIndex]->tokenType);
		printf("\n");
		if (tokens[tokenIndex]->tokenType == TOKEN_WHITESPACE) {
			printf("Skipping whitespace!\n");
			tokenIndex++;
			continue;
		}
		//reading shift
		token_t token = getTopOfStack()->tokenType;
		int actionIndex = tokens[tokenIndex]->tokenType-NUM_INSTANCES;
		Step step = instances[token].actions[actionIndex].step;
		//printf("Instance: %d | ActionIndex: %d | step: %d\n",token,actionIndex,step);
		if (step == STEP_ACCEPT) {
			return 1;
		}
		else if (step == STEP_SHIFT) {
			shift(tokens[tokenIndex],instanceTokens[instances[token].actions[actionIndex].instance]);
		}
		else if (step == STEP_REDUCE) {
			reduce(instances[token].actions[actionIndex].instance);
		}
		else {
			printf("Token not found\n");
			return 0;
		}
		printStack();
	}
	return -1;
}

void freeRules() {
	for (int i = 0; i < NUM_RULES; i++) {
		free(rules[i].symbols);
	}
}
/*
 * initializing grammar
 */
void initializeRules() {
	/*EXAMPLE
	token_t rule0[1] = { VAR_L };
	addRule(0, VAR_P, 1, rule0);*/
	token_t rule0[1] = { VAR_B1 }; 
	addRule(0, VAR_A1, 1, rule0);
	token_t rule1[2] = { VAR_C1, VAR_B1 };
	addRule(1, VAR_B1, 2, rule1);
	token_t rule2[1] = { VAR_C1 } ;
	addRule(2, VAR_B1, 1, rule2);
	token_t rule3[1] = { VAR_M1 };
	addRule(3, VAR_C1, 1, rule3);
	token_t rule4[1] = { VAR_D1 };
	addRule(4, VAR_C1, 1, rule4);
	token_t rule5[3] = { VAR_E1, VAR_H1, TOKEN_SEMICOLON };
	addRule(5, VAR_D1, 3, rule5);
	token_t rule6[2] = { VAR_F1, VAR_G1 };
	addRule(6, VAR_E1, 2, rule6);
	token_t rule7[1] = { VAR_G1 };
	addRule(7, VAR_E1, 1, rule7);
	token_t rule8[1] = { TOKEN_SIGNED };
	addRule(8, VAR_F1, 1, rule8);
	token_t rule9[1] = { TOKEN_UNSIGNED };
	addRule(9, VAR_F1, 1, rule9);
	token_t rule10[1] = { TOKEN_INT };
	addRule(10, VAR_G1, 1, rule10);
	token_t rule11[1] = { TOKEN_LONG };
	addRule(11, VAR_G1, 1, rule11);
	token_t rule12[1] = { TOKEN_SHORT };
	addRule(12, VAR_G1, 1, rule12);
	token_t rule13[1] = { TOKEN_CHAR };
	addRule(13, VAR_G1, 1, rule13);
	token_t rule14[1] = { TOKEN_DOUBLE };
	addRule(14, VAR_G1, 1, rule14);
	token_t rule15[1] = { VAR_I1 };
	addRule(15, VAR_H1, 1, rule15);
	token_t rule16[3] = { VAR_I1, TOKEN_ASSIGN, VAR_J1 };
	addRule(16, VAR_H1, 3, rule16);
	token_t rule17[4] = { TOKEN_ID, TOKEN_LEFTBRACKET, TOKEN_NUM, TOKEN_RIGHTBRACKET };
	addRule(17, VAR_I1, 4, rule17);
	token_t rule18[1] = { TOKEN_ID};
	addRule(18, VAR_I1, 1, rule18);
	token_t rule19[2] = { TOKEN_TILDE, TOKEN_ID };
	addRule(19, VAR_I1, 2, rule19);
	token_t rule20[5] = { TOKEN_TILDE, TOKEN_ID, TOKEN_LEFTBRACKET, TOKEN_NUM, TOKEN_RIGHTBRACKET };
	addRule(20, VAR_I1, 5, rule20);
	token_t rule21[1] = { TOKEN_NUM };
	addRule(21, VAR_J1, 1, rule21);
	token_t rule22[1] = { TOKEN_CHARCONST };
	addRule(22, VAR_J1, 1, rule22);
	token_t rule23[1] = { TOKEN_STRINGCONST };
	addRule(23, VAR_J1, 1, rule23);
	token_t rule24[8] = { VAR_E1, TOKEN_ID, TOKEN_LEFTPAREN, VAR_L1, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(24, VAR_K1, 8, rule24);
	token_t rule25[7] = { VAR_E1, TOKEN_ID, TOKEN_LEFTPAREN, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(25, VAR_K1, 7, rule25);
	token_t rule26[4] = { VAR_E1, VAR_I1, TOKEN_COMMA, VAR_L1 };
	addRule(26, VAR_L1, 4, rule26);
	token_t rule27[2] = { VAR_E1, VAR_I1 };
	addRule(27, VAR_L1, 2, rule27);
	token_t rule28[6] = { VAR_E1, TOKEN_ID, TOKEN_LEFTPAREN, VAR_L1, TOKEN_RIGHTPAREN, TOKEN_SEMICOLON };
	addRule(28, VAR_M1, 6, rule28);
	token_t rule29[5] = { VAR_E1, TOKEN_ID, TOKEN_LEFTPAREN, TOKEN_RIGHTPAREN, TOKEN_SEMICOLON };
	addRule(29, VAR_M1, 5, rule29);
	token_t rule30[1] = { VAR_K1 };
	addRule(30, VAR_M1, 1, rule30);
	token_t rule31[2] = { VAR_O1, VAR_N1 };
	addRule(31, VAR_N1, 2, rule31);
	token_t rule32[1] = { VAR_O1 };
	addRule(32, VAR_N1, 1, rule32);
	token_t rule33[1] = { VAR_D2 };
	addRule(33, VAR_O1, 1, rule33);
	token_t rule34[1] = { VAR_X1 };
	addRule(34, VAR_O1, 1, rule34);
	token_t rule35[1] = { VAR_H2 };
	addRule(35, VAR_O1, 1, rule35);
	token_t rule36[1] = { VAR_P1 };
	addRule(36, VAR_O1, 1, rule36);
	token_t rule85[2] = { TOKEN_BREAK, TOKEN_SEMICOLON };
	addRule(85, VAR_O1, 2, rule85);
	token_t rule86[2] = { TOKEN_CONTINUE, TOKEN_SEMICOLON };
	addRule(86, VAR_O1, 2, rule86);
	token_t rule37[2] = { VAR_V1, TOKEN_SEMICOLON };
	addRule(37, VAR_P1, 2, rule37);
	token_t rule38[3] = { VAR_E1, VAR_Q1, TOKEN_SEMICOLON };
	addRule(38, VAR_P1, 3, rule38);
	token_t rule39[1] = { VAR_I1 };
	addRule(39, VAR_Q1, 1, rule39);
	token_t rule40[3] = { VAR_I1, TOKEN_ASSIGN, VAR_R1 };
	addRule(40, VAR_Q1, 3, rule40);
	token_t rule41[1] = { VAR_U1 };
	addRule(41, VAR_R1, 1, rule41);
	token_t rule42[3] = { VAR_R1, VAR_S1, VAR_U1 };
	addRule(42, VAR_R1, 3, rule42);
	token_t rule43[3] = { VAR_R1, VAR_T1, VAR_U1 };
	addRule(43, VAR_R1, 3, rule43);
	token_t rule44[1] = { TOKEN_PLUS };
	addRule(44, VAR_S1, 1, rule44);
	token_t rule45[1] = { TOKEN_MULT };
	addRule(45, VAR_S1, 1, rule45);
	token_t rule46[1] = { TOKEN_MOD };
	addRule(46, VAR_S1, 1, rule46);
	token_t rule47[1] = { TOKEN_DIV };
	addRule(47, VAR_S1, 1, rule47);
	token_t rule48[1] = { TOKEN_MINUS };
	addRule(48, VAR_S1, 1, rule48);
	token_t rule49[1] = { TOKEN_BITWISEAND };
	addRule(49, VAR_S1, 1, rule49);
	token_t rule50[1] = { TOKEN_BITWISEOR };
	addRule(50, VAR_S1, 1, rule50);
	token_t rule51[1] = { TOKEN_BITWISEXOR };
	addRule(51, VAR_S1, 1, rule51);
	token_t rule52[1] = { TOKEN_EQUAL };
	addRule(52, VAR_T1, 1, rule52);
	token_t rule53[1] = { TOKEN_LESSEQUAL };
	addRule(53, VAR_T1, 1, rule53);
	token_t rule54[1] = { TOKEN_GREATEQUAL };
	addRule(54, VAR_T1, 1, rule54);
	token_t rule55[1] = { TOKEN_GREAT };
	addRule(55, VAR_T1, 1, rule55);
	token_t rule56[1] = { TOKEN_LESS };
	addRule(56, VAR_T1, 1, rule56);
	token_t rule57[1] = { TOKEN_AND };
	addRule(57, VAR_T1, 1, rule57);
	token_t rule58[1] = { TOKEN_NOT };
	addRule(58, VAR_T1, 1, rule58);
	token_t rule59[1] = { TOKEN_OR };
	addRule(59, VAR_T1, 1, rule59);
	token_t rule60[1] = { VAR_J1 };
	addRule(60, VAR_U1, 1, rule60);
	token_t rule61[2] = { TOKEN_AT, TOKEN_ID };
	addRule(61, VAR_U1, 2, rule61);
	token_t rule62[2] = { TOKEN_QUESTIONMARK, TOKEN_ID };
	addRule(62, VAR_U1, 2, rule62);
	token_t rule63[1] = { TOKEN_ID};
	addRule(63, VAR_U1, 1, rule63);
	token_t rule64[4] = { TOKEN_ID, TOKEN_LEFTBRACKET, VAR_R1, TOKEN_RIGHTBRACKET };
	addRule(64, VAR_U1, 4, rule64);
	token_t rule65[1] = { VAR_V1 };
	addRule(65, VAR_U1, 1, rule65);
	token_t rule66[4] = { TOKEN_ID, TOKEN_LEFTPAREN, VAR_W1, TOKEN_RIGHTPAREN };
	addRule(66, VAR_V1, 4, rule66);
	token_t rule67[3] = { TOKEN_ID, TOKEN_LEFTPAREN, TOKEN_RIGHTPAREN };
	addRule(67, VAR_V1, 3, rule67);
	token_t rule68[3] = { VAR_R1, TOKEN_COMMA, VAR_W1 };
	addRule(68, VAR_W1, 3, rule68);
	token_t rule69[1] = { VAR_R1 };
	addRule(69, VAR_W1, 1, rule69);
	token_t rule70[1] = { VAR_Y1 };
	addRule(70, VAR_X1, 1, rule70);
	token_t rule71[1] = { VAR_A2 };
	addRule(71, VAR_X1, 1, rule71);
	token_t rule72[7] = { TOKEN_WHILE, TOKEN_LEFTPAREN, VAR_R1, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(72, VAR_Y1, 7, rule72);
	token_t rule73[12] = { TOKEN_FOR, TOKEN_LEFTPAREN, VAR_E1, VAR_Q1, TOKEN_SEMICOLON, VAR_B2, TOKEN_SEMICOLON, VAR_C2, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(73, VAR_A2, 12, rule73);
	token_t rule74[3] = { TOKEN_ID, VAR_T1, VAR_R1 };
	addRule(74, VAR_B2, 3, rule74);
	token_t rule75[3] = { TOKEN_ID, TOKEN_ASSIGN, VAR_R1 };
	addRule(75, VAR_C2, 3, rule75);
	token_t rule76[3] = { VAR_E2, VAR_F2, VAR_G2 };
	addRule(76, VAR_D2, 3, rule76);
	token_t rule77[2] = { VAR_E2, VAR_F2 };
	addRule(77, VAR_D2, 2, rule77);
	token_t rule78[2] = { VAR_E2, VAR_G2 };
	addRule(78, VAR_D2, 2, rule78);
	token_t rule79[1] = { VAR_E2 };
	addRule(79, VAR_D2, 1, rule79);
	token_t rule80[7] = { TOKEN_IF, TOKEN_LEFTPAREN, VAR_R1, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(80, VAR_E2, 7, rule80);
	token_t rule81[7] = { TOKEN_ELSEIF, TOKEN_LEFTPAREN, VAR_R1, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(81, VAR_F2, 7, rule81);
	token_t rule82[8] = { TOKEN_ELSEIF, TOKEN_LEFTPAREN, VAR_R1, TOKEN_RIGHTPAREN, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY, VAR_F2 };
	addRule(82, VAR_F2, 8, rule82);
	token_t rule83[4] = { TOKEN_ELSE, TOKEN_LEFTCURLY, VAR_N1, TOKEN_RIGHTCURLY };
	addRule(83, VAR_G2, 4, rule83);
	token_t rule84[3] = { TOKEN_RETURN, VAR_R1, TOKEN_SEMICOLON };
	addRule(84, VAR_H2, 3, rule84);

}
struct Test {
	int x;
};

struct Test test;

int main(int argc, char *argv[]) {
	scanner(argc, argv);
	test.x = 5;
	initializeRules();
	createInstanceAndVarTokens();
	//printRules();

	initializeInstances();
	printTokens();
	int result = parse();
	if (result == 1) {
		printf("SUCCESS!\n");
	}
	else if (result == 0) {
		printf("FAILURE\n");
	}
	else {
		printf("ODD FAILURE\n");
	}


	freeRules();
	freeInstanceAndVarTokens();
	//freeTokens();seg fault
	freeStack();
}
