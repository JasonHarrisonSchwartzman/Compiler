#ifndef TOKEN_H
#define TOKEN_H
typedef enum token_t{
	TOKEN_NUM = 0,
	TOKEN_ID = 1,
	TOKEN_PLUS = 2,
	TOKEN_MINUS = 3,
	TOKEN_ASSIGN = 4,
	TOKEN_SEMICOLON = 5,
	TOKEN_INT = 6,
	TOKEN_LONG = 7,
	TOKEN_DOLLAR = 8,
	TOKEN_EOF = 9,
	TOKEN_WHITESPACE = 10,
	VAR_P = 11,//11
	VAR_L = 12,
	VAR_D = 13,
	VAR_S = 14,
	VAR_T = 15,
	VAR_E = 16,
	VAR_O = 17,
	VAR_X = 18
} token_t;
#endif
