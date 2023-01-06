#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include "token.h"

struct StackItem {
	token_t instance;
	Token token;
	token_t var;
	void *ptr;
} StackItem;


typedef enum type_t {
	INT,
	LONG,
	SHORT,
	CHAR,
	DOUBLE
} type_t;

typedef enum signed_t {
	SIGNED,
	UNSIGNED
} signed_t;

struct Value {
	char *string;
	int num;
	char charconst;
} Value;

struct Name {
	char *name;
	int *length;//length of array
	int *pointer;//0 for not a pointer 1 for yes a pointer
} Name;

struct Type {
	type_t *dataType;
	signed_t *sign;
} Type;

struct Decl {
	struct Name *name;
	struct Value *value;
} Decl;

struct VarDecl {
	struct Type *type;
	struct Decl *decl;
} VarDecl;



//D1
struct VarDecl *addVarDecl(struct Type *type, struct Decl *decl) {
	struct VarDecl *var = calloc(1,sizeof(struct VarDecl));
	var->type = type;
	var->decl = decl;
	return var;
}

//H1
struct Decl *addDecl(struct Name *name, struct Value *value) {
	struct Decl *decl = calloc(1,sizeof(struct Decl));
	decl->name = name;
	decl->value = value;
	return decl;
}

//I1
struct Name *addName(char *name, int length, int pointer) {
	struct Name *n = calloc(1,sizeof(struct Name));
	n->name = name;
	*(n->length) = length;
	*(n->pointer) = pointer;
	return n;
}

//J1
struct Value *addValue(int num, char charconst, char *string) {
	struct Value *val = calloc(1,sizeof(struct Value));
	val->num = num;
	val->charconst = charconst;
	val->string = string;
	return val;
}

//G1
type_t *addDataType(type_t dataType) {
	type_t *t = malloc(sizeof(type_t));
	*t = dataType;
	return t;
}

//F1
signed_t *addSign(signed_t sign) {
	signed_t *s = malloc(sizeof(signed_t));
	*s = sign;
	return s;
}

//E1
struct Type *addType(type_t *dataType, signed_t *sign) {
	struct Type *type = calloc(1,sizeof(struct Type));
	type->dataType = dataType;
	type->sign = sign;
	return type;
}		


#endif
