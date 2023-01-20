#include "semantics.h"
#include "syntaxtree.h"
#include <stdlib.h>
#include <string.h>

extern struct Declaration *syntaxTree;


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
struct Symbol *createSymbol(char *name, struct Type *type, symbol_t sym, dec_t dec) {
	struct Symbol *s = malloc(sizeof(struct Symbol));
	s->name = name;
	s->type = type;
	s->sym = sym;
	s->dec = dec;
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

/*
 * Determines whether a symbol has been defined at the current scope (within the same symbol table)
 * Note: I might want to return a pointer rather than an int for more information
 */
int lookUpNameCurrentScope(char *name, struct SymbolTable *symTab) {
	struct Symbol *tempSym = symTab->symbols;
	while (tempSym) {
		if (strcmp(name,tempSym->name) == 0) {
			printf("Redefinition of identifier \"%s\" defined on line [line] and attempted on line [line]\n",name);
			return 0;
		}
		tempSym = tempSym->next;
	}
	return 1;
}

/*
 * takes a var decl to create a symbol to add to current scope
 */
void createSymbolTableVarDecl(struct SymbolTable *symTab, struct VarDecl *var) {
	if (!lookUpNameCurrentScope(var->name,symTab)) return;
	struct Symbol *s = createSymbol(var->name,var->type,symTab->level == 0 ? SYMBOL_GLOBAL : SYMBOL_LOCAL,VAR);
	addSymbol(symTab,s);
	printf("Entered variable %s\n",var->name);
}

/*
 * Same as above function except for functions
 */ 
void createSymbolTableFuncDecl(struct SymbolTable *symTab, struct FuncDecl *func) {
	if (!lookUpNameCurrentScope(func->name,symTab)) return;
	struct Symbol *s = createSymbol(func->name,func->type,SYMBOL_GLOBAL,FUNC);
	addSymbol(symTab,s);
	printf("entered function %s\n",func->name);
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
			createSymbolTableVarDecl(symTab,s->var);
		}
		if (s->stmt == FOR) {
			struct SymbolTable *innerStmts = addInner(symTab);
			createSymbolTableVarDecl(innerStmts,s->loop->init);
			createSymbolTableStatements(innerStmts,s->loop->stmts);
		}
		if (s->stmt == WHILE) {
			struct SymbolTable *innerStmts = addInner(symTab);
			createSymbolTableStatements(innerStmts,s->loop->stmts);
		}
		if (s->stmt == IF) {
			struct CondStatement *c = s->condstmt;
			while (c) {
				struct SymbolTable *innerStmts = addInner(symTab);
				createSymbolTableStatements(innerStmts,c->stmts);
				c = c->next;
			}
		}

		s = s->next;

	}
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
	if (!lookUpNameCurrentScope(param->name,symTab)) return;
	struct Symbol *s = createSymbol(param->name,param->type,SYMBOL_LOCAL,VAR);
	addSymbol(symTab,s);
	printf("entered param %s\n",param->name);
}

void resolveEval(struct Evaluation *eval) {
	if ((eval->eval == ID) || (eval->eval == ARRAYINDEX)) {
		//lookup name
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
	printSymbolTable(symbolTables);
	printf("Checked all semantics\n");
	return 0;
}
