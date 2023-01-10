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

typedef enum operation_t {
	PLUS,
	MINUS,
	MULT,
	DIV,
	MOD,
	BITWISEAND,
	BITWISEOR,
	BITWISEXOR
} operation_t;

typedef enum conditional_t {
	EQUAL,
	GREATEQUAL,
	LESSEQUAL,
	LESS,
	GREAT,
	AND,
	NOT,
	OR
} conditional_t;

typedef enum statement_t {
	DECLARATION,
	FOR,
	WHILE,
	CONDITIONAL,
	RETURN,
	FUNCCALL,
	ASSIGNMENT,
	BREAK,
	CONTINUE
} statement_t;
//J1
struct Value {
	char *string;
	int num;
	char charconst;
} Value;
//I1
struct Name {
	char *name;
	int length;//length of array
	int pointer;//0 for not a pointer 1 for yes a pointer
} Name;

//E1
struct Type {
	type_t dataType;
	signed_t sign;
} Type;
//H1
struct Decl {
	struct Name *name;
	struct Value *value;
} Decl;
//D1
struct VarDecl {
	struct Type *type;
	struct Decl *decl;
} VarDecl;

//R1
struct Expression{ 
	struct Expression *expr;
	operation_t op;
	conditional_t cond;
	struct Evaluation *eval;
} Expression;

//W1
struct FunctionArgs {
	struct Expression *expr;
	struct FunctionArgs *funcargs;
} FunctionArgs;

//V1
struct FunctionCall {
	char *name;
	struct FunctionArgs *funcargs;
} FunctionCall;

//U1
struct Evaluation{ 
	struct Value *value;
	char *name;
	int dereference;
	int reference;
	struct Expression *expr;
	struct FunctionCall *funccall;

} Evaluation;

//N1
struct Statement {
	statement_t stmt;
	struct Statement *next;
	struct Expression *expr;
} Statement;

//L1
struct Params {
	struct Type *type;
	struct Name *name;
	struct Params *next;
} Params;

//K1 M1
struct FuncDecl {
	struct Statement *statements;
	struct Type *type;
	struct Params *params;
} FuncDecl;
//B1
struct Declaration {
	int x;
} Declaration;

//H2


//D1
struct VarDecl *addVarDecl(struct Type *type, struct Decl *decl) {
	struct VarDecl *var = calloc(1,sizeof(struct VarDecl));
	var->type = type;
	var->decl = decl;
	return var;
}



//L1
struct Params *addParam(struct Params *params, struct Type *type, struct Name *name) {
	struct Params *p = calloc(1,sizeof(Params));
	p->next = params;
	p->type = type;
	p->name = name;
	return p;
}

//K1, M1
struct FuncDecl *addFuncDecl(struct Statement *stmts, struct Type *type, struct Params *params) {
	struct FuncDecl *f = calloc(1,sizeof(FuncDecl));
	f->statements = stmts;
	f->type = type;
	f->params = params;
	return f;
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
	n->length = length;
	n->pointer = pointer;
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

//S1
operation_t *addOperation(operation_t op) {
	operation_t *o = malloc(sizeof(signed_t));
	*o = op;
	return o;
}

//T1
conditional_t *addConditional(conditional_t cond) {
	conditional_t *c = malloc(sizeof(conditional_t));
	*c = cond;
	return c;
}

//E1
struct Type *addType(type_t dataType, signed_t sign) {
	struct Type *type = calloc(1,sizeof(struct Type));
	type->dataType = dataType;
	type->sign = sign;
	return type;
}

//W1
struct FunctionArgs *addFuncArgs(struct Expression *expr, struct FunctionArgs *funcArgs) {
	struct FunctionArgs *f = calloc(1,sizeof(struct FunctionArgs));
	f->expr = expr;
	f->funcargs = funcArgs;
	return f;
}

//V1
struct FunctionCall *addFuncCall(char *name, struct FunctionArgs *funcargs) {
	struct FunctionCall *f = calloc(1,sizeof(struct FunctionCall));
	f->name = name;
	f->funcargs = funcargs;
	return f;
}

//U1
struct Evaluation *addEval(struct Value *value, struct Expression *expr, char *name, int dereference, int reference, struct FunctionCall *funccall) {
	struct Evaluation *eval = calloc(1,sizeof(struct Evaluation));
	eval->value = value;
	eval->expr = expr;
	eval->name = name;
	eval->dereference = dereference;
	eval->reference = reference;
	eval->funccall = funccall;
	return eval;
}

//R1
struct Expression *addExpr(struct Expression *expr, conditional_t cond, operation_t op, struct Evaluation *eval) {
	struct Expression *e = calloc(1,sizeof(struct Expression));
	e->expr = expr;
	e->cond = cond;
	e->op = op;
	e->eval = eval;
	return e;
}


#endif
