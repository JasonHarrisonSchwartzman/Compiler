#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include "token.h"
#include <stdlib.h>

/*
 * Includes all structures that can be created during the reduction phase of the parser. 
 */

struct Declaration *syntaxTree;

typedef enum type_t {
	CHAR,
	SHORT,
	INT,
	LONG,
	DOUBLE
} type_t;

typedef enum value_t {
	NUM,
	CHARCONST,
	STRINGCONST
} value_t;

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
	BITWISEXOR,
	EQUAL,
	GREATEQUAL,
	LESSEQUAL,
	LESS,
	GREAT,
	AND,
	NOT,
	OR
} operation_t;

typedef enum statement_t {
	DECLARATION,
	FOR,
	WHILE,
	IF,
	ELSEIF,
	ELSE,
	RETURN,
	FUNCCALL,
	ASSIGNMENT,
	BREAK,
	CONTINUE
} statement_t;

typedef enum eval_t {
	VALUE,
	REF,
	DEREF,
	ID,
	ARRAYINDEX,
	FUNCRETURN
} eval_t;

typedef enum dec_t {
	FUNC,
	VAR
} dec_t;

/*
 * for semantics
 */
typedef enum symbol_t {
	SYMBOL_GLOBAL,
	SYMBOL_LOCAL
} symbol_t;

struct Symbol {
	char *name;
	struct Type *type;
	symbol_t sym;
	dec_t dec;
	struct Symbol *next;
	
	unsigned long sequence;

	unsigned long line;

	struct Params *params;
} Symbol;



//B1
struct Declaration {
	dec_t dec;
	struct FuncDecl *funcdecl;
	struct VarDecl *vardecl;
	struct Declaration *next;
} Declaration;

//D1
struct VarDecl {
	struct Type *type;
	char *name;
	struct Expression *expr;

	struct Symbol *symbol;//for symbol table
	unsigned long line;
} VarDecl;

//K1 M1
struct FuncDecl {
	struct Statement *statements;
	struct Type *type;
	struct Params *params;
	char *name;

	struct Symbol *symbol;//for symbol table
	unsigned long line;
} FuncDecl;

//J1
struct Value {
	value_t val_t;
	char *value;

	unsigned long line;
} Value;
//I1 I2
struct Name {
	char *name;
	struct Expression *expr;//length of array
	int pointer;//0 for not a pointer 1 for yes a pointer
	unsigned long line;
} Name;

//E1
struct Type {
	type_t dataType;
	signed_t sign;
	int pointer; //1 means pointer
	struct Expression *length; // for arrays
} Type;
//H1 Q1
struct Decl {
	struct Name *name;
	struct Expression *expr;
} Decl;

//R1
struct Expression{ 
	struct Expression *expr;
	operation_t *op;
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

	struct Symbol *symbol;//for symbol table
	unsigned long line;
} FunctionCall;

//U1
struct Evaluation{ 
	eval_t eval;
	struct Value *value;
	char *name;
	int dereference;
	int reference;
	struct Expression *expr;
	struct FunctionCall *funccall;

	struct Symbol *symbol;//for symbol table
	struct Type *type;//for type checking
	unsigned long line;

} Evaluation;

//O1 N1
struct Statement {
	statement_t stmt;
	struct VarDecl *var;//can be either a declaration or assignment
	struct FunctionCall *funccall;
	struct Expression *returnstmt;
	struct Loop *loop;
	struct CondStatement *condstmt;
	struct Statement *next;
} Statement;

//L1
struct Params {
	//struct Type *type;
	//char *name;
	struct Params *next;
	
	struct VarDecl *var;
} Params;

struct CondStatement {
	statement_t stmt;
	struct Expression *expr;
	struct Statement *stmts;
	struct CondStatement *next;
} CondStatement;

//A2
struct Loop {
	statement_t stmt;
	struct VarDecl *init;
	struct VarDecl *mod;//assignment
	struct Expression *expr;//for loop end or while loop expression
	struct Statement *stmts;
} Loop;

struct FunctionStatement {
	statement_t stmt;
	struct FunctionCall *funccall;
	struct VarDecl *vardecl;
} FunctionStatement;

struct FunctionStatement *addFunctionStatement(statement_t stmt, struct FunctionCall *funccall, struct VarDecl *vardecl) {
	struct FunctionStatement *f = calloc(1,sizeof(struct FunctionStatement));
	f->stmt = stmt;
	f->funccall = funccall;
	f->vardecl = vardecl;
	return f;
}

struct Declaration *addDeclarations(struct Declaration *decl, struct Declaration *next) {
	decl->next = next;
	return decl;
}

struct Declaration *addDeclaration(dec_t dec, struct FuncDecl *funcdecl, struct VarDecl *vardecl) {
	struct Declaration *d = calloc(1,sizeof(Declaration));
	d->dec = dec;
	d->funcdecl = funcdecl;
	d->vardecl = vardecl;
	return d;
}

//D2
struct CondStatement *addCondStatements(struct CondStatement *ifstmt, struct CondStatement *elseifstmt, struct CondStatement *elsestmt) {
	if (elseifstmt) ifstmt->next = elseifstmt;
	else {
		ifstmt->next = elsestmt;
		return ifstmt;
	}
	if (!elsestmt) return ifstmt;
	struct CondStatement *c = elseifstmt;
	elseifstmt = elseifstmt->next;
	while (elseifstmt) {
		c = c->next;
		elseifstmt = elseifstmt->next;
	}
	c->next = elsestmt;
	return ifstmt;
}

//E2 F2 G2
struct CondStatement *addCondStatement(struct Expression *expr, struct Statement *statement, statement_t stmt, struct CondStatement *next) {
	struct CondStatement *c = calloc(1,sizeof(CondStatement));
	c->expr = expr;
	c->stmts = statement;
	c->stmt = stmt;
	c->next = next;
	return c;
}

struct Type *addType();

//D1                      (E1, H1)
struct VarDecl *addVarDecl(struct Type *type, struct Decl *decl) {
	struct VarDecl *var = calloc(1,sizeof(struct VarDecl));
	var->type = type;
	if (decl->name->expr && !type) {
		var->type = addType(NULL,NULL);
		var->type->length = decl->name->expr;
	}
	if (type) {
		var->type->length = decl->name->expr;
		var->type->pointer = decl->name->pointer;
	}
	var->name = decl->name->name;
	var->expr = decl->expr;
	var->line = decl->name->line;
	return var;
}

//H1 Q1
struct Decl *addDecl(struct Name *name, struct Expression *expr) {
	struct Decl *decl = calloc(1,sizeof(struct Decl));
	decl->name = name;
	decl->expr = expr;
	return decl;
}

//L1
struct Params *addParam(struct Params *params, struct Type *type, struct Name *name) {
	struct Params *p = calloc(1,sizeof(Params));
	p->next = params;
	p->var = addVarDecl(type,addDecl(name,name->expr));
	return p;
}

//K1, M1
struct FuncDecl *addFuncDecl(struct Statement *stmts, struct Type *type, struct Params *params, char *name, unsigned long line) {
	struct FuncDecl *f = calloc(1,sizeof(FuncDecl));
	f->statements = stmts;
	f->type = type;
	f->params = params;
	f->name = name;
	f->line = line;
	return f;
}

//J1
struct Value *addValue(value_t val_t, char *value,unsigned long line) {
	struct Value *val = calloc(1,sizeof(struct Value));
	val->val_t = val_t;
	val->value = value;
	val->line = line;
	return val;
}

//G1
type_t *addDataType(type_t dataType) {
	type_t *t = calloc(1,sizeof(type_t));
	*t = dataType;
	return t;
}

//F1
signed_t *addSign(signed_t sign) {
	signed_t *s = calloc(1,sizeof(signed_t));
	*s = sign;
	return s;
}

//S1 T1
operation_t *addOperation(operation_t op) {
	operation_t *o = calloc(1,sizeof(signed_t));
	*o = op;
	return o;
}

//E1
struct Type *addType(type_t *dataType, signed_t *sign) {
	struct Type *type = calloc(1,sizeof(struct Type));
	if (!dataType && !sign) {
		return type;
	}
	else {
		type->dataType = *dataType;
		if (!sign) {
			if (*dataType == CHAR) type->sign = UNSIGNED;
			else type->sign = SIGNED;
		}
		else type->sign = *sign;
	}
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
struct FunctionCall *addFuncCall(char *name, struct FunctionArgs *funcargs,unsigned long line) {
	struct FunctionCall *f = calloc(1,sizeof(struct FunctionCall));
	f->name = name;
	f->funcargs = funcargs;
	f->line = line;
	return f;
}

//U1
struct Evaluation *addEval(eval_t eval, struct Value *value, struct Expression *expr, char *name, int dereference, int reference, struct FunctionCall *funccall, unsigned long line) {
	struct Evaluation *e = calloc(1,sizeof(struct Evaluation));
	e->value = value;
	
	//e->expr = expr;expr should be part of type
	if (expr) {
		e->type = addType(NULL,NULL);
		e->type->length = expr;
	}
	e->name = name;
	e->dereference = dereference;
	e->reference = reference;
	e->funccall = funccall;
	e->eval = eval;
	e->line = line;
	return e;
}

//R1
struct Expression *addExpr(struct Expression *expr, operation_t *op, struct Evaluation *eval) {
	struct Expression *e = calloc(1,sizeof(struct Expression));
	e->expr = expr;
	e->op = op;
	e->eval = eval;
	return e;
}

//I1
struct Name *addName(char *name, char *length, int pointer, struct Expression *expr, unsigned long line) {
	struct Name *n = calloc(1,sizeof(struct Name));
	n->name = name;
	n->line = line;
	if (length) n->expr = addExpr(NULL,NULL,addEval(VALUE,addValue(NUM,length,line),NULL,NULL,-1,-1,NULL,line));
	if (expr) n->expr = expr;
	n->pointer = pointer;
	return n;
}

//01 N1
struct Statement *addStatement(statement_t stmt, struct VarDecl *vardecl, struct FunctionCall *funccall, struct Expression *returnstmt, struct Loop *loop, struct CondStatement *condstmt) {
	struct Statement *s = calloc(1,sizeof(Statement));
	s->stmt = stmt;
	s->var = vardecl;
	s->funccall = funccall;
	s->returnstmt = returnstmt;
	s->loop = loop;
	s->condstmt = condstmt;
	return s;
}

struct Statement *addStatements(struct Statement *stmt, struct Statement *next) {
	stmt->next = next;
	return stmt;
}

struct Loop *addLoop(statement_t stmt, struct VarDecl *init, struct VarDecl *mod, struct Expression *expr, struct Statement *stmts) {
	struct Loop *l = calloc(1,sizeof(struct Loop));
	l->stmt = stmt;
	l->init = init;
	l->mod = mod;
	l->expr = expr;
	l->stmts = stmts;
	return l;
}



#endif
