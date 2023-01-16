#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include "token.h"

struct Declarations *syntaxTree;

typedef enum type_t {
	INT,
	LONG,
	SHORT,
	CHAR,
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
//J1
struct Value {
	value_t val_t;
	char *value;
} Value;
//I1 I2
struct Name {
	char *name;
	struct Expression *expr;//length of array
	int pointer;//0 for not a pointer 1 for yes a pointer
} Name;

//E1
struct Type {
	type_t dataType;
	signed_t sign;
	int pointer; //1 means pointer 0 means not
	struct Expression *length; // for arrays
} Type;
//H1 Q1
struct Decl {
	struct Name *name;
	struct Value *value;
	struct Expression *expr;
} Decl;

//D1
struct VarDecl {
	struct Type *type;
	char *name;
	struct Expression *expr;
} VarDecl;

//R1
struct Expression{ 
	struct Expression *expr;
	operation_t op;
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

typedef enum eval_t {
	VALUE,
	REF,
	DEREF,
	ID,
	ARRAYINDEX,
	FUNCRETURN
} eval_t;

//U1
struct Evaluation{ 
	eval_t eval;
	struct Value *value;
	char *name;
	int dereference;
	int reference;
	struct Expression *expr;
	struct FunctionCall *funccall;

} Evaluation;

//N1
struct Statements {
	struct Statements *next;
	struct Statement *stmt;
} Statements;

//O1
struct Statement {
	statement_t stmt;
	struct FunctionStatement *funcstmt;
	struct ReturnState *returnstmt;
	struct Loop *loop;
	struct CondStatements *condstmt;
} Statement;

struct Statement *addStatement(statement_t stmt, struct FunctionStatement *funcstmt, struct ReturnState *returnstmt, struct Loop *loop, struct CondStatements *condstmt) {
	struct Statement *s = calloc(1,sizeof(Statement));
	s->stmt = stmt;
	s->funcstmt = funcstmt;
	s->returnstmt = returnstmt;
	s->loop = loop;
	s->condstmt = condstmt;
	return s;
}

struct Statements *addStatements(struct Statements *next, struct Statement *stmt) {
	struct Statements *s = calloc(1,sizeof(Statements));
	s->next = next;
	s->stmt = stmt;
	return s;
}

//L1
struct Params {
	struct Type *type;
	char *name;
	struct Params *next;
} Params;

struct CondStatement {
	statement_t stmt;
	struct Expression *expr;
	struct Statement *stmts;
	struct CondStatement *next;
} CondStatement;

//D2
struct CondStatements {
	struct CondStatement *ifstmt;
	struct CondStatement *elseifstmt;
	struct CondStatement *elsestmt;
} CondStatements;

//K1 M1
struct FuncDecl {
	struct Statement *statements;
	struct Type *type;
	struct Params *params;
	char *name;
} FuncDecl;

//A2
struct ForLoop {
	struct Type  *type;
	struct LoopMod *loopmod;
	struct LoopEnd *loopend;
	struct Statements *stmts;
	struct Decl *decl;
} ForLoop;

struct LoopMod { 
	struct Expression *expr;
	char *name;
} LoopMod;

struct ReturnState {
	struct Expression *expr;
} ReturnState;

//B2
struct LoopEnd {
	struct Expression *expr;
	operation_t *cond;
	char *name;
} LoopEnd;

struct WhileLoop {
	struct Expression *expr;
	struct Statements *stmts;
} WhileLoop;

//X1
struct Loop {
	statement_t stmt;
	struct WhileLoop *whileloop;
	struct ForLoop *forloop;
} Loop;

typedef enum dec_t {
	FUNC,
	VAR
} dec_t;

//B1
struct Declaration {
	dec_t dec;
	struct FuncDecl *funcdecl;
	struct VarDecl *vardecl;
	struct Declaration *next;
} Declaration;

struct Declaration *addDeclaration(struct FuncDecl *funcdecl, struct VarDecl *vardecl, struct Declaration *next) {
	struct Declaration *d = calloc(1,sizeof(Declaration));
	d->funcdecl = funcdecl;
	d->vardecl = vardecl;
	return d;
}

struct FunctionStatement {
	statement_t stmt;
	struct FunctionCall *funccall;
	struct VarDecl *vardecl;
	struct Decl *decl;
} FunctionStatement;

struct FunctionStatement *addFunctionStatement(statement_t stmt, struct FunctionCall *funccall, struct VarDecl *vardecl, struct Decl *decl) {
	struct FunctionStatement *f = calloc(1,sizeof(struct FunctionStatement));
	f->stmt = stmt;
	f->funccall = funccall;
	f->vardecl = vardecl;
	f->decl = decl;
	return f;
}

//Y1
struct WhileLoop *addWhileLoop(struct Expression *expr, struct Statements *stmts) {
	struct WhileLoop *w = calloc(1,sizeof(WhileLoop));
	w->expr = expr;
	w->stmts = stmts;
	return w;
}

struct Loop *addLoop(struct WhileLoop *whileloop, struct ForLoop *forloop, statement_t stmt) {
	struct Loop *l = calloc(1,sizeof(Loop));
	l->whileloop = whileloop;
	l->forloop = forloop;
	l->stmt = stmt;
	return l;
}

//A2
struct ForLoop *addForLoop(struct Type *type, struct LoopMod *loopmod, struct LoopEnd *loopend, struct Statements *stmts, struct Decl *decl) {
	struct ForLoop *f = calloc(1,sizeof(ForLoop));
	f->type = type;
	f->loopmod = loopmod;
	f->loopend = loopend;
	f->stmts = stmts;
	f->decl = decl;
	return f;
}

//D2
struct CondStatements *addCondStatements(struct CondStatement *ifstmt, struct CondStatement *elseifstmt, struct CondStatement *elsestmt) {
	struct CondStatements *c = calloc(1,sizeof(CondStatements));
	c->ifstmt = ifstmt;
	c->elseifstmt = elseifstmt;
	c->elsestmt = elsestmt;
	return c;
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

//H2
struct ReturnState *addReturnState(struct Expression *expr) {
	struct ReturnState *r = malloc(sizeof(struct Expression));
	r->expr = expr;
	return r;
}

//C2
struct LoopMod *addLoopMod(struct Expression *expr, char *name) {
	struct LoopMod *l = calloc(1,sizeof(LoopMod));
	l->expr = expr;
	l->name = name;
	return l;
}

//B2
struct LoopEnd *addLoopEnd(struct Expression *expr, operation_t *cond, char *name) {
	struct LoopEnd *l = calloc(1,sizeof(struct LoopEnd));
	l->name = name;
	l->cond = cond;
	l->expr = expr;
	return l;
}

//D1                      (E1, H1)
struct VarDecl *addVarDecl(struct Type *type, struct Decl *decl) {
	struct VarDecl *var = calloc(1,sizeof(struct VarDecl));
	var->type = type;
	var->type->length = decl->name->expr;
	var->type->pointer = decl->name->pointer;
	var->name = decl->name->name;
	var->expr = decl->expr;
	return var;
}



//L1
struct Params *addParam(struct Params *params, struct Type *type, struct Name *name) {
	struct Params *p = calloc(1,sizeof(Params));
	p->next = params;
	p->type = type;
	p->name = name->name;
	return p;
}

//K1, M1
struct FuncDecl *addFuncDecl(struct Statement *stmts, struct Type *type, struct Params *params, char *name) {
	struct FuncDecl *f = calloc(1,sizeof(FuncDecl));
	f->statements = stmts;
	f->type = type;
	f->params = params;
	f->name = name;
	return f;
}



//H1
struct Decl *addDecl(struct Name *name, struct Value *value, struct Expression *expr) {
	struct Decl *decl = calloc(1,sizeof(struct Decl));
	decl->name = name;
	decl->value = value;
	decl->expr = expr;
	return decl;
}

//J1
struct Value *addValue(value_t val_t, char *value) {
	struct Value *val = calloc(1,sizeof(struct Value));
	val->val_t = val_t;
	val->value = value;
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

//S1 T1
operation_t *addOperation(operation_t op) {
	operation_t *o = malloc(sizeof(signed_t));
	*o = op;
	return o;
}

//E1
struct Type *addType(type_t *dataType, signed_t *sign, int pointer, struct Expression *length) {
	struct Type *type = calloc(1,sizeof(struct Type));
	type->dataType = *dataType;
	type->sign = *sign;
	type->pointer = pointer;
	type->length = length;
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
struct Evaluation *addEval(eval_t eval, struct Value *value, struct Expression *expr, char *name, int dereference, int reference, struct FunctionCall *funccall) {
	struct Evaluation *e = calloc(1,sizeof(struct Evaluation));
	e->value = value;
	e->expr = expr;
	e->name = name;
	e->dereference = dereference;
	e->reference = reference;
	e->funccall = funccall;
	e->eval = eval;
	return e;
}

//R1
struct Expression *addExpr(struct Expression *expr, operation_t *op, struct Evaluation *eval) {
	struct Expression *e = calloc(1,sizeof(struct Expression));
	e->expr = expr;
	e->op = *op;
	e->eval = eval;
	return e;
}

//I1
struct Name *addName(char *name, char *length, int pointer, struct Expression *expr) {
	struct Name *n = calloc(1,sizeof(struct Name));
	n->name = name;
	if (length) n->expr = addExpr(NULL,NULL,addEvalution(VALUE,addValue(NUM,length),NULL,NULL,-1,-1,NULL));
	if (expr) n->expr = expr;
	n->pointer = pointer;
	return n;
}


#endif
