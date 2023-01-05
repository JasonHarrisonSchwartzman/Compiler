#include "parser.h"
#include "token.h"

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

void addInstanceAction(int instanceNum, Step step, int num, token_t token) {
	instances[instanceNum].actions[token-NUM_INSTANCES].step = step;
	instances[instanceNum].actions[token-NUM_INSTANCES].instance = num;
}
void addInstanceGoto(int instanceNum, token_t gotoNum, int gotoInstance) {
	instances[instanceNum].gotoAction[gotoNum-(NUM_INSTANCES+TOTAL_TOKENS)] = gotoInstance;
}

void initializeInstances() {
	//instance 0
	addInstanceAction(0,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(0,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(0,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(0,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(0,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(0,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(0,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceGoto(0,VAR_B1,1);
	addInstanceGoto(0,VAR_C1,9);
	addInstanceGoto(0,VAR_D1,10);
	addInstanceGoto(0,VAR_E1,12);
	addInstanceGoto(0,VAR_F1,14);
	addInstanceGoto(0,VAR_G1,15);
	addInstanceGoto(0,VAR_K1,13);
	addInstanceGoto(0,VAR_M1,11);
	//instance 1
	addInstanceAction(1,STEP_ACCEPT,-1,TOKEN_DOLLAR);
	//instance 2
	addInstanceAction(2,STEP_REDUCE,9,TOKEN_INT);
	addInstanceAction(2,STEP_REDUCE,9,TOKEN_LONG);
	addInstanceAction(2,STEP_REDUCE,9,TOKEN_DOUBLE);
	addInstanceAction(2,STEP_REDUCE,9,TOKEN_CHAR);
	addInstanceAction(2,STEP_REDUCE,9,TOKEN_SHORT);
	//instance 3
	addInstanceAction(3,STEP_REDUCE,8,TOKEN_INT);
	addInstanceAction(3,STEP_REDUCE,8,TOKEN_LONG);
	addInstanceAction(3,STEP_REDUCE,8,TOKEN_DOUBLE);
	addInstanceAction(3,STEP_REDUCE,8,TOKEN_CHAR);
	addInstanceAction(3,STEP_REDUCE,8,TOKEN_SHORT);
	//instance 4
	addInstanceAction(4,STEP_REDUCE,10,TOKEN_ID);
	addInstanceAction(4,STEP_REDUCE,10,TOKEN_TILDE);
	//instance 5
	addInstanceAction(5,STEP_REDUCE,11,TOKEN_ID);
	addInstanceAction(5,STEP_REDUCE,11,TOKEN_TILDE);
	//instance 6
	addInstanceAction(6,STEP_REDUCE,12,TOKEN_ID);
	addInstanceAction(6,STEP_REDUCE,12,TOKEN_TILDE);
	//instance 7
	addInstanceAction(7,STEP_REDUCE,13,TOKEN_ID);
	addInstanceAction(7,STEP_REDUCE,13,TOKEN_TILDE);
	//instance 8
	addInstanceAction(8,STEP_REDUCE,14,TOKEN_ID);
	addInstanceAction(8,STEP_REDUCE,14,TOKEN_TILDE);
	//instance 9
	addInstanceAction(9,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(9,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(9,STEP_REDUCE,2,TOKEN_DOLLAR);
	addInstanceAction(9,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(9,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(9,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(9,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(9,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceGoto(9,VAR_B1,16);
	addInstanceGoto(9,VAR_C1,9);
	addInstanceGoto(9,VAR_D1,10);
	addInstanceGoto(9,VAR_E1,12);
	addInstanceGoto(9,VAR_F1,14);
	addInstanceGoto(9,VAR_G1,15);
	addInstanceGoto(9,VAR_K1,13);
	addInstanceGoto(9,VAR_M1,11);
	//instance 10
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_INT);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_LONG);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_DOLLAR);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_DOUBLE);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_CHAR);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_SHORT);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_SIGNED);
	addInstanceAction(10,STEP_REDUCE,4,TOKEN_UNSIGNED);
	//instance 11
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_INT);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_LONG);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_DOLLAR);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_DOUBLE);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_CHAR);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_SHORT);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_SIGNED);
	addInstanceAction(11,STEP_REDUCE,3,TOKEN_UNSIGNED);
	//instance 12
	addInstanceAction(12,STEP_SHIFT,18,TOKEN_ID);
	addInstanceAction(12,STEP_SHIFT,20,TOKEN_TILDE);
	addInstanceGoto(12,VAR_H1,17);
	addInstanceGoto(12,VAR_I1,19);
	//instance 13
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_INT);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_LONG);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_DOLLAR);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_DOUBLE);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_CHAR);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_SHORT);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_SIGNED);
	addInstanceAction(13,STEP_REDUCE,30,TOKEN_UNSIGNED);
	//instance 14
	addInstanceAction(14,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(14,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(14,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(14,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(14,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceGoto(14,VAR_G1,21);
	//instance 15
	addInstanceAction(15,STEP_REDUCE,7,TOKEN_ID);
	addInstanceAction(15,STEP_REDUCE,7,TOKEN_LEFTBRACKET);
	//instance 16
	addInstanceAction(16,STEP_REDUCE,1,TOKEN_DOLLAR);
	//instance 17
	addInstanceAction(17,STEP_SHIFT,22,TOKEN_SEMICOLON);
	//instance 18
	addInstanceAction(18,STEP_REDUCE,18,TOKEN_ASSIGN);
	addInstanceAction(18,STEP_REDUCE,18,TOKEN_SEMICOLON);
	addInstanceAction(18,STEP_SHIFT,24,TOKEN_LEFTBRACKET);
	addInstanceAction(18,STEP_SHIFT,23,TOKEN_LEFTPAREN);
	addInstanceAction(18,STEP_REDUCE,18,TOKEN_RIGHTPAREN);
	addInstanceAction(18,STEP_REDUCE,18,TOKEN_COMMA);
	//instance 19
	addInstanceAction(19,STEP_SHIFT,25,TOKEN_ASSIGN);
	addInstanceAction(19,STEP_REDUCE,15,TOKEN_SEMICOLON);
	//instance 20
	addInstanceAction(20,STEP_SHIFT,26,TOKEN_ID);
	//instance 21
	addInstanceAction(21,STEP_REDUCE,6,TOKEN_ID);
	addInstanceAction(21,STEP_REDUCE,6,TOKEN_TILDE);
	//instance 22
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_INT);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_LONG);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_DOLLAR);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_DOUBLE);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_CHAR);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_SHORT);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_SIGNED);
	addInstanceAction(22,STEP_REDUCE,5,TOKEN_UNSIGNED);
	//instance 23
	addInstanceAction(23,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(23,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(23,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(23,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(23,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(23,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(23,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(23,STEP_SHIFT,28,TOKEN_RIGHTPAREN);
	addInstanceGoto(23,VAR_E1,29);
	addInstanceGoto(23,VAR_F1,14);
	addInstanceGoto(23,VAR_G1,15);
	addInstanceGoto(23,VAR_L1,27);
	//instance 24
	addInstanceAction(24,STEP_SHIFT,30,TOKEN_NUM);
	//instance 25
	addInstanceAction(25,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(25,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(25,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceGoto(25,VAR_J1,31);
	//instance 26
	addInstanceAction(26,STEP_REDUCE,19,TOKEN_ASSIGN);
	addInstanceAction(26,STEP_REDUCE,19,TOKEN_SEMICOLON);
	addInstanceAction(26,STEP_SHIFT,35,TOKEN_LEFTBRACKET);
	addInstanceAction(26,STEP_REDUCE,19,TOKEN_RIGHTPAREN);
	addInstanceAction(26,STEP_REDUCE,19,TOKEN_COMMA);
	//instance 27
	addInstanceAction(27,STEP_SHIFT,36,TOKEN_RIGHTPAREN);
	//instance 28
	addInstanceAction(28,STEP_SHIFT,37,TOKEN_SEMICOLON);
	addInstanceAction(28,STEP_SHIFT,38,TOKEN_LEFTCURLY);
	//instance 29
	addInstanceAction(29,STEP_SHIFT,121,TOKEN_NUM);
	addInstanceAction(29,STEP_SHIFT,20,TOKEN_TILDE);
	addInstanceGoto(29,VAR_I1,39);
	//instance 30
	addInstanceAction(30,STEP_SHIFT,40,TOKEN_RIGHTBRACKET);
	//instancd 31
	addInstanceAction(31,STEP_REDUCE,16,TOKEN_SEMICOLON);
	//instance 32,33,34
	for (int i = 0; i < 3; i++) {
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_PLUS);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_MINUS);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_SEMICOLON);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_RIGHTBRACKET);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_RIGHTPAREN);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_EQUAL);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_GREATEQUAL);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_LESSEQUAL);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_GREAT);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_LESS);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_AND);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_OR);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_NOT);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_MULT);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_DIV);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_MOD);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_COMMA);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_BITWISEAND);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_BITWISEOR);
	addInstanceAction(i+32,STEP_REDUCE,i+21,TOKEN_BITWISEXOR);
	}

	//instance 35
	addInstanceAction(35,STEP_SHIFT,41,TOKEN_NUM);
	//instance 36
	addInstanceAction(36,STEP_SHIFT,42,TOKEN_SEMICOLON);
	addInstanceAction(36,STEP_SHIFT,43,TOKEN_LEFTCURLY);
	//instance 37
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_INT);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_LONG);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_DOLLAR);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_DOUBLE);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_CHAR);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_SHORT);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_SIGNED);
	addInstanceAction(37,STEP_REDUCE,29,TOKEN_UNSIGNED);
	//instance 38
	addInstanceAction(38,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(38,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(38,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(38,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(38,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(38,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(38,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(38,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(38,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(38,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(38,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(38,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(38,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(38,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(38,VAR_E1,56);
	addInstanceGoto(38,VAR_F1,14);
	addInstanceGoto(38,VAR_G1,15);
	addInstanceGoto(38,VAR_N1,44);
	addInstanceGoto(38,VAR_O1,45);
	addInstanceGoto(38,VAR_P1,49);
	addInstanceGoto(38,VAR_V1,57);
	addInstanceGoto(38,VAR_X1,47);
	addInstanceGoto(38,VAR_Y1,53);
	addInstanceGoto(38,VAR_A2,54);
	addInstanceGoto(38,VAR_D2,46);
	addInstanceGoto(38,VAR_E2,52);
	addInstanceGoto(38,VAR_H2,48);
	//instance 39
	addInstanceAction(39,STEP_REDUCE,27,TOKEN_RIGHTPAREN);
	addInstanceAction(39,STEP_SHIFT,62,TOKEN_COMMA);
	//instance 40
	addInstanceAction(40,STEP_REDUCE,17,TOKEN_ASSIGN);
	addInstanceAction(40,STEP_REDUCE,17,TOKEN_SEMICOLON);
	addInstanceAction(40,STEP_REDUCE,17,TOKEN_RIGHTPAREN);
	addInstanceAction(40,STEP_REDUCE,17,TOKEN_COMMA);
	//instance 41
	addInstanceAction(41,STEP_SHIFT,63,TOKEN_RIGHTBRACKET);
	//instance 42
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_INT);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_LONG);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_DOLLAR);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_DOUBLE);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_CHAR);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_SHORT);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_SIGNED);
	addInstanceAction(42,STEP_REDUCE,28,TOKEN_UNSIGNED);
	//instance 43
	addInstanceAction(43,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(43,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(43,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(43,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(43,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(43,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(43,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(43,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(43,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(43,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(43,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(43,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(43,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(43,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(43,VAR_E1,56);
	addInstanceGoto(43,VAR_F1,14);
	addInstanceGoto(43,VAR_G1,15);
	addInstanceGoto(43,VAR_N1,64);
	addInstanceGoto(43,VAR_O1,45);
	addInstanceGoto(43,VAR_P1,49);
	addInstanceGoto(43,VAR_V1,57);
	addInstanceGoto(43,VAR_X1,47);
	addInstanceGoto(43,VAR_Y1,53);
	addInstanceGoto(43,VAR_A2,54);
	addInstanceGoto(43,VAR_D2,46);
	addInstanceGoto(43,VAR_E2,52);
	addInstanceGoto(43,VAR_H2,48);
	//instance 44
	addInstanceAction(44,STEP_SHIFT,65,TOKEN_RIGHTCURLY);
	//instance 45
	addInstanceAction(45,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(45,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(45,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(45,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(45,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(45,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(45,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(45,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(45,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(45,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(45,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(45,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(45,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(45,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceAction(45,STEP_REDUCE,32,TOKEN_RIGHTCURLY);
	addInstanceGoto(45,VAR_E1,56);
	addInstanceGoto(45,VAR_F1,14);
	addInstanceGoto(45,VAR_G1,15);
	addInstanceGoto(45,VAR_N1,66);
	addInstanceGoto(45,VAR_O1,45);
	addInstanceGoto(45,VAR_P1,49);
	addInstanceGoto(45,VAR_V1,57);
	addInstanceGoto(45,VAR_X1,47);
	addInstanceGoto(45,VAR_Y1,53);
	addInstanceGoto(45,VAR_A2,54);
	addInstanceGoto(45,VAR_D2,46);
	addInstanceGoto(45,VAR_E2,52);
	addInstanceGoto(45,VAR_H2,48);
	//instance 46,47,48,49
	for (int i = 0; i < 4; i++){
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_ID);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_INT);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_LONG);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_IF);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_FOR);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_WHILE);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_DOUBLE);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_CHAR);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_SHORT);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_SIGNED);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_UNSIGNED);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_RETURN);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_BREAK);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_CONTINUE);
		addInstanceAction(i+46,STEP_REDUCE,i+33,TOKEN_RIGHTCURLY);
	}
	//instance 50
	addInstanceAction(50,STEP_SHIFT,67,TOKEN_SEMICOLON);
	//instance 51
	addInstanceAction(51,STEP_SHIFT,68,TOKEN_SEMICOLON);
	//instance 52
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_ID);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_INT);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_LONG);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_IF);
	addInstanceAction(52,STEP_SHIFT,72,TOKEN_ELSE);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_FOR);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_WHILE);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_DOUBLE);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_CHAR);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_SHORT);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_SIGNED);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_UNSIGNED);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_RETURN);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_BREAK);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_CONTINUE);
	addInstanceAction(52,STEP_REDUCE,79,TOKEN_RIGHTCURLY);
	addInstanceAction(52,STEP_SHIFT,71,TOKEN_ELSEIF);
	addInstanceGoto(52,VAR_F2,69);
	addInstanceGoto(52,VAR_G2,70);
	//instance 53
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_ID);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_INT);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_LONG);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_IF);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_FOR);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_WHILE);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_DOUBLE);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_CHAR);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_SHORT);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_SIGNED);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_UNSIGNED);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_RETURN);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_BREAK);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_CONTINUE);
	addInstanceAction(53,STEP_REDUCE,70,TOKEN_RIGHTCURLY);
	//instance 54
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_ID);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_INT);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_LONG);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_IF);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_FOR);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_WHILE);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_DOUBLE);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_CHAR);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_SHORT);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_SIGNED);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_UNSIGNED);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_RETURN);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_BREAK);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_CONTINUE);
	addInstanceAction(54,STEP_REDUCE,71,TOKEN_RIGHTCURLY);
	//instance 55
	addInstanceAction(55,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(55,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(55,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(55,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(55,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(55,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(55,VAR_J1,76);
	addInstanceGoto(55,VAR_R1,75);
	addInstanceGoto(55,VAR_U1,74);
	addInstanceGoto(55,VAR_V1,80);
	//instance 56
	addInstanceAction(56,STEP_SHIFT,121,TOKEN_ID);
	addInstanceAction(56,STEP_SHIFT,20,TOKEN_TILDE);
	addInstanceGoto(56,VAR_I1,82);
	addInstanceGoto(56,VAR_Q1,81);
	//instance 57
	addInstanceAction(57,STEP_SHIFT,132,TOKEN_SEMICOLON);
	//instance 58
	addInstanceAction(58,STEP_SHIFT,83,TOKEN_LEFTBRACKET);
	//instance 59
	addInstanceAction(59,STEP_SHIFT,84,TOKEN_LEFTPAREN);
	//instance 60
	addInstanceAction(60,STEP_SHIFT,85,TOKEN_LEFTPAREN);
	//instance 61
	addInstanceAction(61,STEP_SHIFT,86,TOKEN_LEFTPAREN);
	//instance 62
	addInstanceAction(62,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(62,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(62,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(62,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(62,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(62,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(62,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceGoto(62,VAR_E1,29);
	addInstanceGoto(62,VAR_F1,14);
	addInstanceGoto(62,VAR_G1,15);
	addInstanceGoto(62,VAR_L1,82);
	//instance 63
	addInstanceAction(63,STEP_REDUCE,20,TOKEN_ASSIGN);
	addInstanceAction(63,STEP_REDUCE,20,TOKEN_SEMICOLON);
	addInstanceAction(63,STEP_REDUCE,20,TOKEN_RIGHTPAREN);
	addInstanceAction(63,STEP_REDUCE,20,TOKEN_COMMA);
	//instance 64
	addInstanceAction(64,STEP_SHIFT,88,TOKEN_RIGHTCURLY);
	//instance 65
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_INT);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_LONG);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_DOLLAR);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_DOUBLE);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_CHAR);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_SHORT);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_SIGNED);
	addInstanceAction(65,STEP_REDUCE,25,TOKEN_UNSIGNED);
	//instance 66
	addInstanceAction(66,STEP_REDUCE,31,TOKEN_RIGHTCURLY);
	//instance 67
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_ID);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_INT);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_LONG);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_IF);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_FOR);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_WHILE);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_DOUBLE);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_CHAR);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_SHORT);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_SIGNED);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_UNSIGNED);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_RETURN);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_BREAK);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_CONTINUE);
	addInstanceAction(67,STEP_REDUCE,85,TOKEN_RIGHTCURLY);
	//instance 68
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_ID);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_INT);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_LONG);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_IF);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_FOR);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_WHILE);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_DOUBLE);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_CHAR);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_SHORT);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_SIGNED);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_UNSIGNED);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_RETURN);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_BREAK);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_CONTINUE);
	addInstanceAction(68,STEP_REDUCE,86,TOKEN_RIGHTCURLY);
	//instance 69
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_ID);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_INT);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_LONG);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_IF);
	addInstanceAction(69,STEP_SHIFT,72,TOKEN_ELSE);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_FOR);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_WHILE);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_DOUBLE);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_CHAR);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_SHORT);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_SIGNED);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_UNSIGNED);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_RETURN);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_BREAK);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_CONTINUE);
	addInstanceAction(69,STEP_REDUCE,77,TOKEN_RIGHTCURLY);
	addInstanceGoto(69,VAR_G2,89);
	//instance 70
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_ID);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_INT);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_LONG);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_IF);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_FOR);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_WHILE);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_DOUBLE);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_CHAR);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_SHORT);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_SIGNED);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_UNSIGNED);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_RETURN);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_BREAK);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_CONTINUE);
	addInstanceAction(70,STEP_REDUCE,78,TOKEN_RIGHTCURLY);
	//instance 71
	addInstanceAction(71,STEP_SHIFT,90,TOKEN_LEFTPAREN);
	//instance 72
	addInstanceAction(72,STEP_SHIFT,91,TOKEN_LEFTCURLY);
	//instance 73
	addInstanceAction(73,STEP_REDUCE,50,TOKEN_NUM);
	addInstanceAction(73,STEP_REDUCE,50,TOKEN_ID);
	addInstanceAction(73,STEP_REDUCE,50,TOKEN_AT);
	addInstanceAction(73,STEP_REDUCE,50,TOKEN_CHARCONST);
	addInstanceAction(73,STEP_REDUCE,50,TOKEN_STRINGCONST);
	addInstanceAction(73,STEP_REDUCE,50,TOKEN_QUESTIONMARK);
	//instance 74
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_PLUS);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_MINUS);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_SEMICOLON);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_RIGHTBRACKET);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_RIGHTPAREN);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_EQUAL);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_GREATEQUAL);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_LESSEQUAL);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_GREAT);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_LESS);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_AND);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_OR);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_NOT);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_MULT);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_DIV);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_MOD);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_COMMA);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_BITWISEAND);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_BITWISEOR);
	addInstanceAction(74,STEP_REDUCE,41,TOKEN_BITWISEXOR);
	//instance 75
	addInstanceAction(75,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(75,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(75,STEP_SHIFT,92,TOKEN_SEMICOLON);
	addInstanceAction(75,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(75,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(75,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(75,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(75,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(75,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(75,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(75,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(75,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(75,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(75,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(75,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(75,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(75,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(75,VAR_S1,93);
	addInstanceGoto(75,VAR_T1,94);
	//instance 76
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_PLUS);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_MINUS);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_SEMICOLON);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_RIGHTBRACKET);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_RIGHTPAREN);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_EQUAL);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_GREATEQUAL);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_LESSEQUAL);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_GREAT);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_LESS);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_AND);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_OR);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_NOT);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_MULT);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_DIV);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_MOD);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_COMMA);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_BITWISEAND);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_BITWISEOR);
	addInstanceAction(76,STEP_REDUCE,60,TOKEN_BITWISEXOR);
	//instance 77
	addInstanceAction(77,STEP_SHIFT,110,TOKEN_ID);
	//instance 78
	addInstanceAction(78,STEP_SHIFT,111,TOKEN_ID);
	//instance 79
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_PLUS);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_MINUS);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_SEMICOLON);
	addInstanceAction(79,STEP_SHIFT,112,TOKEN_LEFTBRACKET);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_RIGHTBRACKET);
	addInstanceAction(79,STEP_SHIFT,86,TOKEN_LEFTPAREN);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_RIGHTPAREN);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_EQUAL);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_GREATEQUAL);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_LESSEQUAL);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_GREAT);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_LESS);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_AND);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_OR);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_NOT);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_MULT);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_DIV);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_MOD);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_COMMA);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_BITWISEAND);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_BITWISEOR);
	addInstanceAction(79,STEP_REDUCE,63,TOKEN_BITWISEXOR);
	//instance 80
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_PLUS);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_MINUS);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_SEMICOLON);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_RIGHTBRACKET);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_RIGHTPAREN);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_EQUAL);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_GREATEQUAL);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_LESSEQUAL);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_GREAT);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_LESS);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_AND);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_OR);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_NOT);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_MULT);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_DIV);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_MOD);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_COMMA);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_BITWISEAND);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_BITWISEOR);
	addInstanceAction(80,STEP_REDUCE,65,TOKEN_BITWISEXOR);
	//instance 81
	addInstanceAction(81,STEP_SHIFT,118,TOKEN_SEMICOLON);
	//instance 82
	addInstanceAction(82,STEP_SHIFT,114,TOKEN_ASSIGN);
	addInstanceAction(82,STEP_REDUCE,39,TOKEN_SEMICOLON);
	//instance 83
	addInstanceAction(83,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(83,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(83,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(83,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(83,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(83,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(83,VAR_J1,76);
	addInstanceGoto(83,VAR_R1,115);
	addInstanceGoto(83,VAR_U1,74);
	addInstanceGoto(83,VAR_V1,80);
	//instance 84
	addInstanceAction(84,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(84,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(84,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(84,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(84,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(84,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(84,VAR_J1,76);
	addInstanceGoto(84,VAR_R1,116);
	addInstanceGoto(84,VAR_U1,74);
	addInstanceGoto(84,VAR_V1,80);
	//instance 85
	addInstanceAction(85,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(85,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(85,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(85,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(85,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(85,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(85,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceGoto(85,VAR_E1,117);
	addInstanceGoto(85,VAR_F1,14);
	addInstanceGoto(85,VAR_G1,15);
	addInstanceGoto(85,VAR_J1,76);
	//instance 86
	addInstanceAction(86,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(86,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(86,STEP_SHIFT,160,TOKEN_RIGHTPAREN);
	addInstanceAction(86,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(86,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(86,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(86,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(86,VAR_R1,119);
	addInstanceGoto(86,VAR_U1,74);
	addInstanceGoto(86,VAR_V1,80);
	addInstanceGoto(86,VAR_W1,118);
	//instance 87
	addInstanceAction(87,STEP_REDUCE,26,TOKEN_RIGHTPAREN);
	//instance 88
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_INT);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_LONG);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_DOLLAR);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_DOUBLE);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_CHAR);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_SHORT);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_SIGNED);
	addInstanceAction(88,STEP_REDUCE,24,TOKEN_UNSIGNED);
	//instance 89
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_ID);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_INT);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_LONG);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_IF);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_FOR);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_WHILE);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_DOUBLE);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_CHAR);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_SHORT);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_SIGNED);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_UNSIGNED);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_RETURN);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_BREAK);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_CONTINUE);
	addInstanceAction(89,STEP_REDUCE,76,TOKEN_RIGHTCURLY);
	//instance 90
	addInstanceAction(90,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(90,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(90,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(90,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(90,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(90,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(90,VAR_J1,76);
	addInstanceGoto(90,VAR_R1,120);
	addInstanceGoto(90,VAR_U1,74);
	addInstanceGoto(90,VAR_V1,80);
	//instance 91
	addInstanceAction(91,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(91,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(91,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(91,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(91,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(91,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(91,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(91,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(91,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(91,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(91,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(91,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(91,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(91,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(91,VAR_E1,56);
	addInstanceGoto(91,VAR_F1,14);
	addInstanceGoto(91,VAR_G1,15);
	addInstanceGoto(91,VAR_N1,141);
	addInstanceGoto(91,VAR_O1,45);
	addInstanceGoto(91,VAR_P1,49);
	addInstanceGoto(91,VAR_V1,57);
	addInstanceGoto(91,VAR_X1,47);
	addInstanceGoto(91,VAR_Y1,53);
	addInstanceGoto(91,VAR_A2,54);
	addInstanceGoto(91,VAR_D2,46);
	addInstanceGoto(91,VAR_E2,52);
	addInstanceGoto(91,VAR_H2,48);
	//instance 92
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_ID);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_INT);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_LONG);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_IF);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_FOR);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_WHILE);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_DOUBLE);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_CHAR);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_SHORT);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_SIGNED);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_UNSIGNED);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_RETURN);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_BREAK);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_CONTINUE);
	addInstanceAction(92,STEP_REDUCE,84,TOKEN_RIGHTCURLY);
	//instance 93
	addInstanceAction(93,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(93,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(93,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(93,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(93,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(93,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(93,VAR_J1,76);
	addInstanceGoto(93,VAR_U1,122);
	addInstanceGoto(93,VAR_V1,80);
	//instance 94
	addInstanceAction(94,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(94,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(94,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(94,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(94,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceGoto(94,VAR_J1,76);
	addInstanceGoto(94,VAR_U1,123);
	addInstanceGoto(94,VAR_V1,80);
	//instance 95-109
	for (int i = 0; i < 15; i++) {
	if (i == 6) continue;
	addInstanceAction(95+i,STEP_REDUCE,44+i,TOKEN_NUM);
	addInstanceAction(95+i,STEP_REDUCE,44+i,TOKEN_ID);
	addInstanceAction(95+i,STEP_REDUCE,44+i,TOKEN_AT);
	addInstanceAction(95+i,STEP_REDUCE,44+i,TOKEN_CHARCONST);
	addInstanceAction(95+i,STEP_REDUCE,44+i,TOKEN_STRINGCONST);
	addInstanceAction(95+i,STEP_REDUCE,44+i,TOKEN_QUESTIONMARK);
	}
	//instance 110,111
	for (int i = 0; i < 2; i++) {
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_PLUS);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_MINUS);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_SEMICOLON);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_RIGHTBRACKET);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_RIGHTPAREN);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_EQUAL);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_GREATEQUAL);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_LESSEQUAL);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_GREAT);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_LESS);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_AND);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_OR);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_NOT);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_MULT);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_DIV);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_MOD);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_COMMA);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_BITWISEAND);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_BITWISEOR);
	addInstanceAction(110+i,STEP_REDUCE,61+i,TOKEN_BITWISEXOR);
	}
	//instance 112
	addInstanceAction(112,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(112,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(112,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(112,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(112,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(112,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(112,VAR_J1,76);
	addInstanceGoto(112,VAR_R1,124);
	addInstanceGoto(112,VAR_U1,74);
	addInstanceGoto(112,VAR_V1,80);
	//instance 113
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_ID);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_INT);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_LONG);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_IF);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_FOR);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_WHILE);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_DOUBLE);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_CHAR);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_SHORT);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_SIGNED);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_UNSIGNED);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_RETURN);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_BREAK);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_CONTINUE);
	addInstanceAction(113,STEP_REDUCE,38,TOKEN_RIGHTCURLY);
	//instancd 114
	addInstanceAction(114,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(114,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(114,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(114,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(114,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(114,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(114,VAR_J1,76);
	addInstanceGoto(114,VAR_R1,125);
	addInstanceGoto(114,VAR_U1,74);
	addInstanceGoto(114,VAR_V1,80);
	//instance 115
	addInstanceAction(115,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(115,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(115,STEP_SHIFT,126,TOKEN_RIGHTPAREN);
	addInstanceAction(115,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(115,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(115,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(115,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(115,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(115,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(115,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(115,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(115,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(115,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(115,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(115,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(115,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(115,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(115,VAR_S1,93);
	addInstanceGoto(115,VAR_T1,94);
	//instance 116
	addInstanceAction(116,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(116,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(116,STEP_SHIFT,127,TOKEN_RIGHTPAREN);
	addInstanceAction(116,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(116,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(116,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(116,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(116,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(116,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(116,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(116,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(116,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(116,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(116,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(116,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(116,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(116,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(116,VAR_S1,93);
	addInstanceGoto(116,VAR_T1,94);
	//instance 117
	addInstanceAction(117,STEP_SHIFT,121,TOKEN_ID);
	addInstanceAction(117,STEP_SHIFT,20,TOKEN_TILDE);
	addInstanceGoto(117,VAR_I1,82);
	addInstanceGoto(117,VAR_Q1,128);
	//instance 118
	addInstanceAction(118,STEP_SHIFT,129,TOKEN_RIGHTPAREN);
	//instance 119
	addInstanceAction(119,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(119,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(119,STEP_REDUCE,69,TOKEN_RIGHTPAREN);
	addInstanceAction(119,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(119,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(119,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(119,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(119,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(119,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(119,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(119,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(119,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(119,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(119,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(119,STEP_SHIFT,130,TOKEN_COMMA);
	addInstanceAction(119,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(119,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(119,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(119,VAR_S1,93);
	addInstanceGoto(119,VAR_T1,94);
	//instance 120
	addInstanceAction(120,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(120,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(120,STEP_SHIFT,131,TOKEN_RIGHTPAREN);
	addInstanceAction(120,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(120,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(120,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(120,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(120,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(120,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(120,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(120,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(120,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(120,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(120,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(120,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(120,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(120,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(120,VAR_S1,93);
	addInstanceGoto(120,VAR_T1,94);
	//instance 121
	addInstanceAction(121,STEP_REDUCE,18,TOKEN_ASSIGN);
	addInstanceAction(121,STEP_REDUCE,18,TOKEN_SEMICOLON);
	addInstanceAction(121,STEP_SHIFT,24,TOKEN_LEFTBRACKET);
	addInstanceAction(121,STEP_REDUCE,18,TOKEN_RIGHTPAREN);
	addInstanceAction(121,STEP_REDUCE,18,TOKEN_COMMA);
	//instance 122,123
	for (int i = 0; i < 2; i++) {
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_PLUS);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_MINUS);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_SEMICOLON);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_RIGHTBRACKET);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_RIGHTPAREN);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_EQUAL);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_GREATEQUAL);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_LESSEQUAL);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_GREAT);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_LESS);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_AND);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_OR);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_NOT);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_MULT);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_DIV);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_MOD);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_COMMA);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_BITWISEAND);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_BITWISEOR);
	addInstanceAction(122+i,STEP_REDUCE,42+i,TOKEN_BITWISEXOR);
	}
	//instancd 124
	addInstanceAction(124,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(124,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(124,STEP_SHIFT,133,TOKEN_RIGHTBRACKET);
	addInstanceAction(124,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(124,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(124,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(124,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(124,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(124,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(124,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(124,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(124,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(124,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(124,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(124,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(124,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(124,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(124,VAR_S1,93);
	addInstanceGoto(124,VAR_T1,94);
	//instance 125
	addInstanceAction(125,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(125,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(125,STEP_SHIFT,40,TOKEN_SEMICOLON);
	addInstanceAction(125,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(125,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(125,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(125,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(125,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(125,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(125,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(125,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(125,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(125,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(125,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(125,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(125,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(125,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(125,VAR_S1,93);
	addInstanceGoto(125,VAR_T1,94);
	//instance 126
	addInstanceAction(126,STEP_SHIFT,134,TOKEN_LEFTCURLY);
	//instance 127
	addInstanceAction(127,STEP_SHIFT,135,TOKEN_LEFTCURLY);
	//instance 128
	addInstanceAction(128,STEP_SHIFT,136,TOKEN_SEMICOLON);
	//instancd 129
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_PLUS);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_MINUS);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_SEMICOLON);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_RIGHTBRACKET);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_RIGHTPAREN);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_EQUAL);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_GREATEQUAL);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_LESSEQUAL);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_GREAT);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_LESS);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_AND);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_OR);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_NOT);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_MULT);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_DIV);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_MOD);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_COMMA);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_BITWISEAND);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_BITWISEOR);
	addInstanceAction(129,STEP_REDUCE,66,TOKEN_BITWISEXOR);
	//instance 130
	addInstanceAction(130,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(130,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(130,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(130,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(130,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(130,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(130,VAR_R1,119);
	addInstanceGoto(130,VAR_U1,74);
	addInstanceGoto(130,VAR_V1,80);
	addInstanceGoto(130,VAR_W1,137);
	//instance 131
	addInstanceAction(131,STEP_SHIFT,138,TOKEN_LEFTCURLY);
	//instance 132
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_ID);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_INT);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_LONG);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_IF);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_FOR);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_WHILE);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_DOUBLE);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_CHAR);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_SHORT);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_SIGNED);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_UNSIGNED);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_RETURN);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_BREAK);
	addInstanceAction(132,STEP_REDUCE,37,TOKEN_CONTINUE);
	//instance 133
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_PLUS);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_MINUS);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_SEMICOLON);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_RIGHTBRACKET);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_RIGHTPAREN);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_EQUAL);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_GREATEQUAL);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_LESSEQUAL);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_GREAT);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_LESS);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_AND);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_OR);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_NOT);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_MULT);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_DIV);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_MOD);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_COMMA);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_BITWISEAND);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_BITWISEOR);
	addInstanceAction(133,STEP_REDUCE,64,TOKEN_BITWISEXOR);
	//instance 134
	addInstanceAction(134,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(134,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(134,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(134,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(134,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(134,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(134,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(134,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(134,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(134,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(134,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(134,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(134,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(134,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(134,VAR_E1,56);
	addInstanceGoto(134,VAR_F1,14);
	addInstanceGoto(134,VAR_G1,15);
	addInstanceGoto(134,VAR_N1,139);
	addInstanceGoto(134,VAR_O1,45);
	addInstanceGoto(134,VAR_P1,49);
	addInstanceGoto(134,VAR_V1,57);
	addInstanceGoto(134,VAR_X1,47);
	addInstanceGoto(134,VAR_Y1,53);
	addInstanceGoto(134,VAR_A2,54);
	addInstanceGoto(134,VAR_D2,46);
	addInstanceGoto(134,VAR_E2,52);
	addInstanceGoto(134,VAR_H2,48);
	//instance 135
	addInstanceAction(135,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(135,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(135,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(135,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(135,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(135,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(135,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(135,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(135,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(135,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(135,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(135,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(135,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(135,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(135,VAR_E1,56);
	addInstanceGoto(135,VAR_F1,14);
	addInstanceGoto(135,VAR_G1,15);
	addInstanceGoto(135,VAR_N1,140);
	addInstanceGoto(135,VAR_O1,45);
	addInstanceGoto(135,VAR_P1,49);
	addInstanceGoto(135,VAR_V1,57);
	addInstanceGoto(135,VAR_X1,47);
	addInstanceGoto(135,VAR_Y1,53);
	addInstanceGoto(135,VAR_A2,54);
	addInstanceGoto(135,VAR_D2,46);
	addInstanceGoto(135,VAR_E2,52);
	addInstanceGoto(135,VAR_H2,48);
	//instance 136
	addInstanceAction(136,STEP_SHIFT,144,TOKEN_ID);
	addInstanceGoto(136,VAR_B2,143);
	//instance 137
	addInstanceAction(137,STEP_REDUCE,68,TOKEN_RIGHTPAREN);
	//instance 138
	addInstanceAction(138,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(138,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(138,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(138,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(138,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(138,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(138,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(138,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(138,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(138,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(138,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(138,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(138,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(138,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(138,VAR_E1,56);
	addInstanceGoto(138,VAR_F1,14);
	addInstanceGoto(138,VAR_G1,15);
	addInstanceGoto(138,VAR_N1,145);
	addInstanceGoto(138,VAR_O1,45);
	addInstanceGoto(138,VAR_P1,49);
	addInstanceGoto(138,VAR_V1,57);
	addInstanceGoto(138,VAR_X1,47);
	addInstanceGoto(138,VAR_Y1,53);
	addInstanceGoto(138,VAR_A2,54);
	addInstanceGoto(138,VAR_D2,46);
	addInstanceGoto(138,VAR_E2,52);
	addInstanceGoto(138,VAR_H2,48);
	//instance 139
	addInstanceAction(139,STEP_SHIFT,146,TOKEN_RIGHTCURLY);
	//instance 140
	addInstanceAction(140,STEP_SHIFT,147,TOKEN_RIGHTCURLY);
	//instance 141
	addInstanceAction(141,STEP_SHIFT,142,TOKEN_RIGHTCURLY);
	//instance 142
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_ID);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_INT);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_LONG);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_IF);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_FOR);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_WHILE);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_DOUBLE);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_CHAR);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_SHORT);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_SIGNED);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_UNSIGNED);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_RETURN);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_BREAK);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_CONTINUE);
	addInstanceAction(142,STEP_REDUCE,83,TOKEN_RIGHTCURLY);
	//instance 143
	addInstanceAction(143,STEP_SHIFT,148,TOKEN_SEMICOLON);
	//instance 144
	addInstanceAction(144,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(144,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(144,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(144,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(144,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(144,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(144,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(144,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceGoto(144,VAR_T1,149);
	//instance 145
	addInstanceAction(145,STEP_SHIFT,150,TOKEN_RIGHTCURLY);
	//instance 146
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_ID);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_INT);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_LONG);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_IF);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_ELSE);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_FOR);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_WHILE);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_DOUBLE);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_CHAR);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_SHORT);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_SIGNED);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_UNSIGNED);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_RETURN);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_BREAK);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_CONTINUE);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_RIGHTCURLY);
	addInstanceAction(146,STEP_REDUCE,80,TOKEN_ELSEIF);
	//instance 147
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_ID);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_INT);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_LONG);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_IF);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_FOR);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_WHILE);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_DOUBLE);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_CHAR);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_SHORT);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_SIGNED);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_UNSIGNED);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_RETURN);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_BREAK);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_CONTINUE);
	addInstanceAction(147,STEP_REDUCE,72,TOKEN_RIGHTCURLY);
	//instance 148
	addInstanceAction(148,STEP_SHIFT,152,TOKEN_ID);
	addInstanceGoto(148,VAR_C2,151);
	//instance 149
	addInstanceAction(149,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(149,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(149,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(149,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(149,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(149,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(149,VAR_J1,76);
	addInstanceGoto(149,VAR_R1,153);
	addInstanceGoto(149,VAR_U1,74);
	addInstanceGoto(149,VAR_V1,80);
	//instance 150
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_ID);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_INT);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_LONG);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_IF);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_ELSE);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_FOR);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_WHILE);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_DOUBLE);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_CHAR);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_SHORT);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_SIGNED);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_UNSIGNED);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_RETURN);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_BREAK);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_CONTINUE);
	addInstanceAction(150,STEP_REDUCE,81,TOKEN_RIGHTCURLY);
	addInstanceAction(150,STEP_SHIFT,71,TOKEN_ELSEIF);
	addInstanceGoto(150,VAR_F2,161);
	//instance 151
	addInstanceAction(151,STEP_SHIFT,154,TOKEN_RIGHTPAREN);
	//instance 152
	addInstanceAction(152,STEP_SHIFT,155,TOKEN_ASSIGN);
	//instance 153
	addInstanceAction(153,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(153,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(153,STEP_REDUCE,74,TOKEN_SEMICOLON);
	addInstanceAction(153,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(153,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(153,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(153,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(153,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(153,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(153,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(153,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(153,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(153,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(153,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(153,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(153,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(153,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(153,VAR_S1,93);
	addInstanceGoto(153,VAR_T1,94);
	//instance 154
	addInstanceAction(154,STEP_SHIFT,156,TOKEN_LEFTCURLY);
	//instancee 155
	addInstanceAction(155,STEP_SHIFT,32,TOKEN_NUM);
	addInstanceAction(155,STEP_SHIFT,79,TOKEN_ID);
	addInstanceAction(155,STEP_SHIFT,77,TOKEN_AT);
	addInstanceAction(155,STEP_SHIFT,33,TOKEN_CHARCONST);
	addInstanceAction(155,STEP_SHIFT,34,TOKEN_STRINGCONST);
	addInstanceAction(155,STEP_SHIFT,78,TOKEN_QUESTIONMARK);
	addInstanceGoto(155,VAR_J1,76);
	addInstanceGoto(155,VAR_R1,157);
	addInstanceGoto(155,VAR_U1,74);
	addInstanceGoto(155,VAR_V1,80);
	//instance 156
	addInstanceAction(156,STEP_SHIFT,61,TOKEN_ID);
	addInstanceAction(156,STEP_SHIFT,4,TOKEN_INT);
	addInstanceAction(156,STEP_SHIFT,5,TOKEN_LONG);
	addInstanceAction(156,STEP_SHIFT,58,TOKEN_IF);
	addInstanceAction(156,STEP_SHIFT,60,TOKEN_FOR);
	addInstanceAction(156,STEP_SHIFT,59,TOKEN_WHILE);
	addInstanceAction(156,STEP_SHIFT,8,TOKEN_DOUBLE);
	addInstanceAction(156,STEP_SHIFT,7,TOKEN_CHAR);
	addInstanceAction(156,STEP_SHIFT,6,TOKEN_SHORT);
	addInstanceAction(156,STEP_SHIFT,3,TOKEN_SIGNED);
	addInstanceAction(156,STEP_SHIFT,2,TOKEN_UNSIGNED);
	addInstanceAction(156,STEP_SHIFT,55,TOKEN_RETURN);
	addInstanceAction(156,STEP_SHIFT,50,TOKEN_BREAK);
	addInstanceAction(156,STEP_SHIFT,51,TOKEN_CONTINUE);
	addInstanceGoto(156,VAR_E1,56);
	addInstanceGoto(156,VAR_F1,14);
	addInstanceGoto(156,VAR_G1,15);
	addInstanceGoto(156,VAR_N1,158);
	addInstanceGoto(156,VAR_O1,45);
	addInstanceGoto(156,VAR_P1,49);
	addInstanceGoto(156,VAR_V1,57);
	addInstanceGoto(156,VAR_X1,47);
	addInstanceGoto(156,VAR_Y1,53);
	addInstanceGoto(156,VAR_A2,54);
	addInstanceGoto(156,VAR_D2,46);
	addInstanceGoto(156,VAR_E2,52);
	addInstanceGoto(156,VAR_H2,48);
	//instance 157
	addInstanceAction(157,STEP_SHIFT,95,TOKEN_PLUS);
	addInstanceAction(157,STEP_SHIFT,99,TOKEN_MINUS);
	addInstanceAction(157,STEP_REDUCE,75,TOKEN_RIGHTPAREN);
	addInstanceAction(157,STEP_SHIFT,102,TOKEN_EQUAL);
	addInstanceAction(157,STEP_SHIFT,104,TOKEN_GREATEQUAL);
	addInstanceAction(157,STEP_SHIFT,103,TOKEN_LESSEQUAL);
	addInstanceAction(157,STEP_SHIFT,105,TOKEN_GREAT);
	addInstanceAction(157,STEP_SHIFT,106,TOKEN_LESS);
	addInstanceAction(157,STEP_SHIFT,107,TOKEN_AND);
	addInstanceAction(157,STEP_SHIFT,109,TOKEN_OR);
	addInstanceAction(157,STEP_SHIFT,108,TOKEN_NOT);
	addInstanceAction(157,STEP_SHIFT,96,TOKEN_MULT);
	addInstanceAction(157,STEP_SHIFT,98,TOKEN_DIV);
	addInstanceAction(157,STEP_SHIFT,97,TOKEN_MOD);
	addInstanceAction(157,STEP_SHIFT,100,TOKEN_BITWISEAND);
	addInstanceAction(157,STEP_SHIFT,73,TOKEN_BITWISEOR);
	addInstanceAction(157,STEP_SHIFT,101,TOKEN_BITWISEXOR);
	addInstanceGoto(157,VAR_S1,93);
	addInstanceGoto(157,VAR_T1,94);
	//instance 158
	addInstanceAction(158,STEP_SHIFT,159,TOKEN_RIGHTCURLY);
	//instance 159
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_ID);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_INT);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_LONG);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_IF);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_FOR);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_WHILE);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_DOUBLE);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_CHAR);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_SHORT);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_SIGNED);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_UNSIGNED);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_RETURN);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_BREAK);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_CONTINUE);
	addInstanceAction(159,STEP_REDUCE,73,TOKEN_RIGHTCURLY);
	//instance 160
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_PLUS);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_MINUS);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_SEMICOLON);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_RIGHTBRACKET);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_EQUAL);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_GREATEQUAL);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_LESSEQUAL);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_GREAT);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_LESS);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_AND);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_OR);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_NOT);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_MULT);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_DIV);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_MOD);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_COMMA);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_BITWISEAND);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_BITWISEOR);
	addInstanceAction(160,STEP_REDUCE,67,TOKEN_BITWISEXOR);
	//instance 161
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_ID);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_INT);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_LONG);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_IF);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_ELSE);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_FOR);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_WHILE);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_DOUBLE);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_CHAR);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_SHORT);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_SIGNED);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_UNSIGNED);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_RETURN);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_BREAK);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_CONTINUE);
	addInstanceAction(161,STEP_REDUCE,82,TOKEN_RIGHTCURLY);
}
