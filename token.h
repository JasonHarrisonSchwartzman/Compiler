#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdlib.h>
/**
 * Contains instances, nonterminal variables and tokens
 * Helpful (in my opinion) to combine all into one type for the stack in the parser
 */
typedef enum token_t{
	I_0 = 0,
	I_1 = 1,
	I_2 = 2,
	I_3 = 3,
	I_4 = 4,
	I_5 = 5,
	I_6 = 6,
	I_7 = 7,
	I_8 = 8,
	I_9 = 9,
	I_10 = 10,
	I_11 = 11,
	I_12 = 12,
	I_13 = 13,
	I_14 = 14,
	I_15 = 15,
	I_16 = 16,
	I_17 = 17,
	I_18 = 18,
	I_19 = 19,
	I_20 = 20,
	I_21 = 21,
	I_22 = 22,
	I_23 = 23,
	I_24 = 24,
	I_25 = 25,
	I_26 = 26,
	I_27 = 27,
	I_28 = 28,
	I_29 = 29,
	I_30 = 30,
	I_31 = 31,
	I_32 = 32,
	I_33 = 33,
	I_34 = 34,
	I_35 = 35,
	I_36 = 36,
	I_37 = 37,
	I_38 = 38,
	I_39 = 39,
	I_40 = 40,
	I_41 = 41,
	I_42 = 42,
	I_43,
	I_44,
	I_45,
	I_46,
	I_47,
	I_48,
	I_49,
	I_50,
	I_51,
	I_52,
	I_53,
	I_54,
	I_55,
	I_56,
	I_57,
	I_58,
	I_59,
	I_60,
	I_61,
	I_62,
	I_63,
	I_64,
	I_65,
	I_66,
	I_67,
	I_68,
	I_69,
	I_70,
	I_71,
	I_72,
	I_73,
	I_74,
	I_75,
	I_76,
	I_77,
	I_78,
	I_79,
	I_80,
	I_81,
	I_82,
	I_83,
	I_84,
	I_85,
	I_86,
	I_87,
	I_88,
	I_89,
	I_90,
	I_91,
	I_92,
	I_93,
	I_94,
	I_95,
	I_96,
	I_97,
	I_98,
	I_99,
	I_100,
	I_101,
	I_102,
	I_103,
	I_104,
	I_105,
	I_106,
	I_107,
	I_108,
	I_109,
	I_110,
	I_111,
	I_112,
	I_113,
	I_114,
	I_115,
	I_116,
	I_117,
	I_118,
	I_119,
	I_120,
	I_121,
	I_122,
	I_123,
	I_124,
	I_125,
	I_126,
	I_127,
	I_128,
	I_129,
	I_130,
	I_131,
	I_132,
	I_133,
	I_134,
	I_135,
	I_136,
	I_137,
	I_138,
	I_139,
	I_140,
	I_141,
	I_142,
	I_143,
	I_144,
	I_145,
	I_146,
	I_147,
	I_148,
	I_149,
	I_150,
	I_151,
	I_152,
	I_153,
	I_154,
	I_155,
	I_156,
	I_157,
	I_158,
	I_159,
	I_160,
	I_161,
	I_162,
	I_163,
	I_164,
	I_165,
	I_166,
	I_167,
	I_168,
	I_169,
	I_170,
	I_171,
	I_172,
	I_173,
	I_174,
	I_175,
	I_176,
	I_177,
	I_178,
	I_179,
	I_180,
	I_181,
	I_182,
	I_183,
	I_184,
	I_185,
	I_186,
	I_187,
	I_188,
	I_189,
	I_190,
	I_191,
	I_192,
	I_193,
	I_194,
	I_195,
	I_196,
	I_197,
	I_198,
	I_199,
	I_200,
	I_201,
	I_202,
	I_203,
	I_204,
	I_205,
	I_206,
	I_207,
	I_208,
	I_209,
	I_210,
	I_211,
	I_212,
	I_213,
	I_214,
	I_215,
	I_216,
	I_217,
	I_218,
	I_219,
	I_220,
	I_221,
	I_222,
	I_223,
	I_224,
	I_225,
	I_226,
	I_227,
	I_228,
	I_229,
	I_230,
	I_231,
	I_232,
	I_233,
	I_234,
	I_235,
	I_236,
	I_237,
	I_238,
	I_239,
	I_240,
	I_241,
	I_242,
	I_243,
	I_244,
	I_245,
	I_246,
	I_247,
	I_248,
	I_249,
	I_250,
	I_251,
	I_252,
	I_253,
	I_254,
	I_255,
	I_256,
	I_257,
	I_258,
	I_259,
	I_260,
	I_261,
	I_262,
	I_263,
	I_264,
	I_265,
	I_266,
	I_267,
	I_268,
	I_269,
	I_270,
	I_271,
	I_272,
	I_273,
	I_274,
	I_275,
	I_276,
	I_277,
	I_278,
	I_279,
	I_280,
	I_281,
	I_282,
	I_283,
	I_284,
	I_285,
	I_286,
	I_287,
	I_288,
	I_289,
	I_290,
	I_291,
	I_292,
	I_293,
	I_294,
	I_295,
	I_296,
	I_297,
	I_298,
	I_299,
	TOKEN_NUM = 300,
	TOKEN_ID = 301,
	TOKEN_PLUS = 302,
	TOKEN_MINUS = 303,
	TOKEN_ASSIGN = 304,
	TOKEN_SEMICOLON = 305,
	TOKEN_INT = 306,
	TOKEN_LONG = 307,
	TOKEN_DOLLAR = 308,
	TOKEN_IF = 309,
	TOKEN_ELSE = 310,
	TOKEN_FOR = 311,
	TOKEN_WHILE = 312,
	TOKEN_DOUBLE = 313,
	TOKEN_CHAR = 314,
	TOKEN_SHORT = 315,
	TOKEN_SIGNED = 316,
	TOKEN_UNSIGNED = 317,
	TOKEN_RETURN = 318,
	TOKEN_BREAK = 319,
	TOKEN_CONTINUE = 320,
	TOKEN_LEFTBRACKET = 321,
	TOKEN_RIGHTBRACKET = 322,
	TOKEN_LEFTPAREN = 323,
	TOKEN_RIGHTPAREN = 324,
	TOKEN_LEFTCURLY = 325,
	TOKEN_RIGHTCURLY = 326,
	TOKEN_EQUAL = 327,
	TOKEN_LESSEQUAL = 328,
	TOKEN_GREATEQUAL = 329,
	TOKEN_GREAT = 330,
	TOKEN_LESS = 331,
	TOKEN_AND = 332,
	TOKEN_OR = 333,
	TOKEN_NOT = 334,
	TOKEN_MULT = 335,
	TOKEN_DIV = 336,
	TOKEN_MOD = 337,
	TOKEN_TILDE = 338,
	TOKEN_AT = 339,
	TOKEN_COMMA = 340,
	TOKEN_BITWISEAND = 341,
	TOKEN_BITWISEOR = 342,
	TOKEN_BITWISEXOR = 343,
	TOKEN_CHARCONST = 344,
	TOKEN_STRINGCONST = 345,
	TOKEN_QUESTIONMARK = 346,
	TOKEN_ELSEIF = 347,
	TOKEN_DECIMAL = 348,
	TOKEN_EOF = 349,
	TOKEN_WHITESPACE = 350,
	VAR_B1 = 351,
	VAR_C1 = 352,
	VAR_D1 = 353,
	VAR_E1 = 354,
	VAR_F1 = 355,
	VAR_G1 = 356,
	VAR_H1 = 357,
	VAR_I1 = 358,
	VAR_J1 = 359,
	VAR_K1 = 360,
	VAR_L1 = 361,
	VAR_M1 = 362,
	VAR_N1 = 363,
	VAR_O1 = 364,
	VAR_P1 = 365,
	VAR_Q1 = 366,
	VAR_R1 = 367,
	VAR_S1 = 368,
	VAR_T1 = 369,
	VAR_U1 = 370,
	VAR_V1 = 371,
	VAR_W1 = 372,
	VAR_X1 = 373,
	VAR_Y1 = 374,
	VAR_A2 = 375,
	VAR_B2 = 376,
	VAR_C2 = 377,
	VAR_D2 = 378,
	VAR_E2 = 379,
	VAR_F2 = 380,
	VAR_G2 = 381,
	VAR_H2 = 382,
	VAR_I2 = 383,
	VAR_A1 = 384,
	VAR_ERROR = 385
} token_t;

/*
 * Token structure. 
 * Each token contains a string representation, a token type, the line it is on and the token index within the line
 */
typedef struct Token{
	token_t tokenType;
	char* token;
	unsigned long line;
	unsigned long tokenIndex;
} Token;

struct Token **tokens;//contains stream of tokens 
int numTokens;//number of tokens in stream

unsigned long numLines = 0;//number of lines in program
char **lines; //contains line to line of entire program

//adds line to lines array 
void addLine(char *line) {
	lines = realloc(lines,sizeof(char*) * (++numLines));
	lines[numLines-1] = line;
}

//prints line from lines array
void printLine(unsigned long line) {
	printf("[%lu]: %s",line,lines[line-1]);
}

/*
 * Converts token enum to english for debugging
 */
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
		case TOKEN_UNSIGNED:
			str = "unsigned";
			break;
		case TOKEN_IF:
			str = "if";
			break;
		case TOKEN_ELSE: 
			str = "else";
			break;
		case TOKEN_FOR:
			str = "for";
			break;
		case TOKEN_WHILE:
			str = "while";
			break;
		case TOKEN_DOUBLE:
			str = "double";
			break;
		case TOKEN_CHAR:
			str = "char";
			break;
		case TOKEN_SHORT:
			str = "short";
			break;
		case TOKEN_RETURN:
			str = "return";
			break;
		case TOKEN_BREAK:
			str = "break";
			break;
		case TOKEN_CONTINUE:
			str = "continue";
			break;
		case TOKEN_LEFTBRACKET:
			str = "[";
			break;
		case TOKEN_RIGHTBRACKET:
			str = "]";
			break;
		case TOKEN_LEFTPAREN:
			str = "(";
			break;
		case TOKEN_RIGHTPAREN:
			str = ")";
			break;
		case TOKEN_LEFTCURLY:
			str = "{";
			break;
		case TOKEN_RIGHTCURLY:
			str = "}";
			break;
		case TOKEN_EQUAL:
			str = "==";
			break;
		case TOKEN_LESSEQUAL:
			str = "<=";
			break;
		case TOKEN_GREATEQUAL:
			str = ">=";
			break;
		case TOKEN_GREAT:
			str = ">";
			break;
		case TOKEN_LESS:
			str = "<";
			break;
		case TOKEN_AND:
			str = "&&";
			break;
		case TOKEN_OR:
			str = "||";
			break;
		case TOKEN_NOT:
			str = "!=";
			break;
		case TOKEN_MULT:
			str = "*";
			break;
		case TOKEN_DIV:
			str = "/";
			break;
		case TOKEN_MOD:
			str = "%";
			break;
		case TOKEN_TILDE:
			str = "~";
			break;
		case TOKEN_AT:
			str = "@";
			break;
		case TOKEN_COMMA:
			str = ",";
			break;
		case TOKEN_BITWISEAND:
			str = "&";
			break;
		case TOKEN_BITWISEOR:
			str = "|";
			break;
		case TOKEN_BITWISEXOR:
			str = "^";
			break;
		case TOKEN_CHARCONST:
			str = "charconst";
			break;
		case TOKEN_STRINGCONST:
			str = "stringconst";
			break;
		case TOKEN_QUESTIONMARK:
			str = "?";
			break;
		case TOKEN_ELSEIF:
			str = "elseif";
			break;
		case VAR_B1:
			str = "B1";
			break;
		case VAR_C1:
			str = "C1";
			break;
		case VAR_D1:
			str = "D1";
			break;
		case VAR_E1:
			str = "E1";
			break;
		case VAR_F1:
			str = "F1";
			break;
		case VAR_G1:
			str = "G1";
			break;
		case VAR_H1:
			str = "H1";
			break;
		case VAR_I1:
			str = "I1";
			break;
		case VAR_J1:
			str = "J1";
			break;
		case VAR_K1:
			str = "K1";
			break;
		case VAR_L1:
			str = "L1";
			break;
		case VAR_M1:
			str = "M1";
			break;
		case VAR_N1:
			str = "N1";
			break;
		case VAR_O1:
			str = "O1";
			break;
		case VAR_P1:
			str = "P1";
			break;
		case VAR_Q1:
			str = "Q1";
			break;
		case VAR_R1:
			str = "R1";
			break;
		case VAR_S1:
			str = "S1";
			break;
		case VAR_T1:
			str = "T1";
			break;
		case VAR_U1:
			str = "U1";
			break;
		case VAR_V1:
			str = "V1";
			break;
		case VAR_W1:
			str = "W1";
			break;
		case VAR_X1:
			str = "X1";
			break;
		case VAR_Y1:
			str = "Y1";
			break;
		case VAR_A2:
			str = "A2";
			break;
		case VAR_B2:
			str = "B2";
			break;
		case VAR_C2:
			str = "C2";
			break;
		case VAR_D2:
			str = "D2";
			break;
		case VAR_E2:
			str = "E2";
			break;
		case VAR_F2:
			str = "F2";
			break;
		case VAR_G2:
			str = "G2";
			break;
		case VAR_H2:
			str = "H2";
			break;
		case VAR_I2:
			str = "I2";
			break;
		case VAR_A1:
			str = "A1";
			break;
		default:
			str = "ERROR TOKEN NOT FOUND";
			break;
	}
	printf("%s",str);
}


#endif
