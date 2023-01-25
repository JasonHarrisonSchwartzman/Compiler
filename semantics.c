#include "syntaxtree.h"
#include <stdlib.h>
#include <string.h>

extern struct Declaration *syntaxTree;
extern char **lines;

/*
 * Symbol contains a linked list of symbols in reverse order of appearence
 * Inner Symbol Tables the represent statements such as conditional and loop (anything enclosed in curly braces)
 * the level corresponds to how "deep" in the symbol table tree the current symbol table is starting at 0 for the root
 * Outer represents the higher up symbol table which is important for determining if a symbol has already been defined
 */
struct SymbolTable {
	struct Symbol *symbols;
	struct SymbolTable **inner;//can have multiple inners
	unsigned long numInner;
	unsigned long level;
	struct SymbolTable *outer;
} SymbolTable;

struct SymbolTable *symbolTables;//the symbol table

/*
 * Allocates mem for a Symbol
 */
struct Symbol *createSymbol(char *name, struct Type *type, symbol_t sym, dec_t dec, unsigned long line) {
	struct Symbol *s = malloc(sizeof(struct Symbol));
	s->name = name;
	s->type = type;
	s->sym = sym;
	s->dec = dec;
	s->line = line;
	return s;
}

/*
 * adds a symbol to a given symbol table
 */
void addSymbol(struct SymbolTable *t, struct Symbol *s) {
	s->next = t->symbols;
	t->symbols = s;
}

/*
 * Adds an inner symbol table (something with curly braces) for a given symbol table
 */
struct SymbolTable *addInner(struct SymbolTable *t) {
	t->inner = realloc(t->inner,sizeof(struct SymbolTable) * (++(t->numInner)));
	t->inner[t->numInner-1] = calloc(1,sizeof(struct SymbolTable));
	t->inner[t->numInner-1]->numInner = 0;
	t->inner[t->numInner-1]->level = t->level + 1;
	(t->inner[t->numInner-1])->outer = t;
	return t->inner[t->numInner-1];
}

/*
 * Prints symbol table for each symbol table with corresponding levels
 */
void printSymbolTable(struct SymbolTable *sym) {
	struct SymbolTable *temp = sym;
	while (temp->symbols) {
		printf("Level %lu Symbol: %s\n",temp->level,temp->symbols->name);
		temp->symbols = temp->symbols->next;
	}
	for (int i = 0; i < temp->numInner; i++ ) {
		printSymbolTable(temp->inner[i]);
	}
}

void printLine(unsigned long line) {
	printf("[%lu]: %s",line,lines[line-1]);
}


/*TODO:
 * Type checking
values: number decimal character string
types: char short int long
sign: signed unsigned
double
pointers
functioncalls which are equal to char short int long or double
arrays
arrays of pointers
Rules for types:
Operations: + - * / % & | ^
Comparison: == <= >= < > && || !=

CHAR SHORT INT LONG can be manipulated with any of the operators or comparisons, they will be converted to the bigger type before computed.




 */

/*
 * Type translation given operators.
 */
struct Type *resolveType(struct Evaluation *eval1, operation_t *op, struct Evaluation *eval2) {
	if (!eval2) {
		return eval1->type;
	}
	switch (*op) {
		case PLUS:
			break;
		case MINUS:
			break;
		case MULT:
			break;
		case DIV:
			break;
		case MOD:
			break;
		case BITWISEAND:
			break;
		case BITWISEOR:
			break;
		case BITWISEXOR:
			break;
		case EQUAL:
			break;
		case GREATEQUAL:
			break;
		case LESSEQUAL:
			break;
		case GREAT:
			break;
		case LESS:
			break;
		case AND:
			break;
		case NOT:
			break;
		case OR:
			break;
	}
	return NULL;
}

struct Type *typeCheckExpr(struct Expression *expr) {
	if (!expr->expr) return resolveType(expr->eval,NULL,NULL);	
	if (!expr->expr->eval->type) return resolveType(expr->expr->eval,expr->op,expr->eval);
	return typeCheckExpr(expr->expr);
}

/*
 * Determines whether a symbol has been defined at the current scope (within the same symbol table)
 */
struct Symbol *lookUpNameCurrentScope(char *name, struct SymbolTable *symTab) {
	struct Symbol *tempSym = symTab->symbols;
	while (tempSym) {
		if (strcmp(name,tempSym->name) == 0) {
			return tempSym;
		}
		tempSym = tempSym->next;
	}
	return NULL;
}

struct Symbol *lookUpName(char *name, struct SymbolTable *symTab) {
	struct SymbolTable *t = symTab;
	while (t) {
		struct Symbol *s = lookUpNameCurrentScope(name,t);
		if (s) return s;
		t = t->outer;
	}

	return NULL;
}

void printError(int errorNum, char *name,unsigned long line1, unsigned long line2) {
	switch(errorNum) {
		case 1:
			printf("Redeclaration of identifier \"%s\" attempted on line [%lu] and declared on line [%lu].\n",name,line1,line2);
			printLine(line2);
			printLine(line1);
			break;
		case 2:
			printf("error 2\n");
			printf("Identifier \"%s\" on line [%lu] not declared.\n",name,line1);
			break;
		case 3:
			printf("Identifier \"%s\" on line [%lu] is declared as a variable and is attempting to use it as a function call\n",name,line1);
			break;
	}
}



/*
 * takes a var decl to create a symbol to add to current scope
 */
void createSymbolTableVarDecl(struct SymbolTable *symTab, struct VarDecl *var) {
	struct Symbol *x = lookUpNameCurrentScope(var->name,symTab);
	if (x) {
		printError(1,x->name,var->line,x->line);
		return;
	}
	struct Symbol *s = createSymbol(var->name,var->type,symTab->level == 0 ? SYMBOL_GLOBAL : SYMBOL_LOCAL,VAR,var->line);
	addSymbol(symTab,s);
	var->symbol = s;
}

/*
 * Same as above function except for functions
 */ 
void createSymbolTableFuncDecl(struct SymbolTable *symTab, struct FuncDecl *func) {
	struct Symbol *x = lookUpNameCurrentScope(func->name,symTab);
	if (x) {
		printError(1,x->name,func->line,func->symbol->line);
		return;
	}
	struct Symbol *s = createSymbol(func->name,func->type,SYMBOL_GLOBAL,FUNC,func->line);
	addSymbol(symTab,s);
	func->symbol = s;
}

/*
 * creates symbols in the current symbol table for the parameters of a function
 * Parameters are considered to be local variables of a function at the same scope of level 1 variables
 * 1 int foo(int x, int y) {
 * 2	int x = 10;
 * 3	return x;
 * 4 }
 * This is illegal because the x on line 2 was defined in the parameter in line 1
 */
void createSymbolTableParams(struct SymbolTable *symTab, struct Params *param) {
	struct Symbol *x = lookUpNameCurrentScope(param->var->name,symTab);
	if (x) {
		printError(1,x->name,param->var->line,param->var->symbol->line);
		return;
	}
	struct Symbol *s = createSymbol(param->var->name,param->var->type,SYMBOL_LOCAL,VAR,param->var->line);
	addSymbol(symTab,s);
	param->var->symbol = s;
}

void resolveEval(struct SymbolTable *symTab, struct Evaluation *eval) {
	if ((eval->eval == ID) || (eval->eval == ARRAYINDEX)) {
		eval->symbol = lookUpName(eval->name,symTab);
		if (!eval->symbol) printError(2,eval->name,eval->line,eval->symbol->line);
	}
}

void resolveExpr(struct SymbolTable *symTab, struct Expression *expr) {
	if (!expr) return;
	resolveExpr(symTab,expr->expr);
	resolveEval(symTab,expr->eval);
}

void resolveAssignment(struct SymbolTable *symTab, struct VarDecl *var) {
	var->symbol = lookUpName(var->name,symTab);
	if (!var->symbol) printError(2, var->name,var->line,var->symbol->line);
}

void resolveFuncCall(struct SymbolTable *symTab, struct FunctionCall *funccall) {
	funccall->symbol = lookUpName(funccall->name,symTab);
	if (!funccall->symbol) {
		printError(2, funccall->name,funccall->symbol->line,funccall->line);
		return;
	}
	if (funccall->symbol->dec == VAR) {
		printError(3, funccall->name,funccall->symbol->line,0);
		return;
	}
}


/*
 * Given the statements within a function will add symbol for variables defined OR 
 * increase a level if inner curly braces are found (for if while)
 * Recursively creates symbols tables for statements
 */
void createSymbolTableStatements(struct SymbolTable *symTab, struct Statement *statement) {
	struct Statement *s = statement;
	while (s) {
		if (s->stmt == DECLARATION) {
			createSymbolTableVarDecl(symTab,s->var);//var decl
			if(s->var->type) resolveExpr(symTab,s->var->type->length);//array size (if applicable)
			resolveExpr(symTab,s->var->expr);//right hand side of dec
		}
		if (s->stmt == FOR) {
			struct SymbolTable *innerStmts = addInner(symTab);
			createSymbolTableVarDecl(innerStmts,s->loop->init);
			resolveAssignment(innerStmts,s->loop->mod);
			resolveExpr(innerStmts,s->loop->mod->expr);
			resolveExpr(innerStmts,s->loop->expr);

			createSymbolTableStatements(innerStmts,s->loop->stmts);
			
		}
		if (s->stmt == WHILE) {
			struct SymbolTable *innerStmts = addInner(symTab);
			resolveExpr(innerStmts,s->loop->expr);
			createSymbolTableStatements(innerStmts,s->loop->stmts);
		}
		if (s->stmt == IF) {
			struct CondStatement *c = s->condstmt;
			while (c) {
				struct SymbolTable *innerStmts = addInner(symTab);
				resolveExpr(innerStmts,c->expr);//resolves condition expression
				createSymbolTableStatements(innerStmts,c->stmts);
				c = c->next;
			}
		}
		if (s->stmt == FUNCCALL) {
			resolveFuncCall(symTab,s->funccall);
			struct FunctionArgs *f = s->funccall->funcargs;
			while (f) {
				resolveExpr(symTab,f->expr);
				f = f->funcargs;
			}
		}
		if (s->stmt == RETURN) {
			resolveExpr(symTab,s->returnstmt);
			typeCheckExpr(s->returnstmt);
		}
		if (s->stmt == ASSIGNMENT) {
			resolveAssignment(symTab,s->var);//var assign
			if(s->var->type) resolveExpr(symTab,s->var->type->length);//array size if applicable
			resolveExpr(symTab,s->var->expr);//right hand side of ass
		}

		s = s->next;

	}
}

/*
 * Loops through the declarations calling the appropriate createSymbolTable function
 * adds level when it sees a function declaration and calls createSymbolTableParam for params and 
 * createSymbolTableStatement for the statements
 */
void createSymbolTableDeclarations(struct SymbolTable *initial, struct Declaration *declarations) {
	struct Declaration *d = declarations;
	while (d) {
		if (d->dec == VAR) {
			createSymbolTableVarDecl(initial,d->vardecl);
		}
		else {
			createSymbolTableFuncDecl(initial,d->funcdecl);
			struct SymbolTable *inFunc = addInner(initial);
			struct Params *tempParam = d->funcdecl->params;
			while (tempParam) {
				createSymbolTableParams(inFunc,tempParam);
				tempParam = tempParam->next;
			}
			createSymbolTableStatements(inFunc,d->funcdecl->statements);
		}
		d = d->next;
	}
}

/*
 * Not compeleted yet
 */ 
int checkAll() {
	symbolTables = calloc(1,sizeof(struct SymbolTable));
	symbolTables->numInner = 0;
	symbolTables->level = 0;
	createSymbolTableDeclarations(symbolTables,syntaxTree);
	//printSymbolTable(symbolTables);
	printf("Checked all semantics\n");
	return 0;
}
