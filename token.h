#ifndef TOKEN_H
#define TOKEN_H
typedef enum token_t{
	TOKEN_KEY,//keywords if, else, while, etc...
	TOKEN_ID,//identifiers names of variables, functions, etc.
	TOKEN_NUM,//numbers
	TOKEN_STR,//strings, literal character sequences
	TOKEN_COMMENT,//
	TOKEN_SPACE,//includes '\n' and '\t'
	TOKEN_OPERATOR,//+ - * etc
	TOKEN_ASSIGNMENT,//=
	TOKEN_COMPARATOR,//== =< <= < >
	TOKEN_EOF
} token_t;

typedef enum var_t {
	VAR_P,
	VAR_L,
	VAR_D,
	VAR_S,
	VAR_T,
	VAR_E,
	VAR_O,
	VAR_X
} var_t;
#endif
