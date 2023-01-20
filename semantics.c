#include "semantics.h"
#include "syntaxtree.h"
#include <stdlib.h>
#include <string.h>

extern struct Declaration *syntaxTree;

struct SymbolTable {
	struct Symbol *symbols;
	struct SymbolTable **inner;//can have multiple inners
	unsigned long numInner;
	unsigned long level;
	struct SymbolTable *outer;
} SymbolTable;

struct SymbolTable *symbolTables;

struct Symbol *createSymbol(char *name, struct Type *type, symbol_t sym, dec_t dec) {
	struct Symbol *s = malloc(sizeof(struct Symbol));
	s->name = name;
	s->type = type;
	s->sym = sym;
	s->dec = dec;
	return s;
}

void addSymbol(struct SymbolTable *t, struct Symbol *s) {
	s->next = t->symbols;
	t->symbols = s;
}

struct SymbolTable *addInner(struct SymbolTable *t) {
	t->inner = realloc(t->inner,sizeof(struct SymbolTable) * (++(t->numInner)));
	t->inner[t->numInner-1] = calloc(1,sizeof(struct SymbolTable));
	t->inner[t->numInner-1]->numInner = 0;
	t->inner[t->numInner-1]->level = t->level + 1;
	(t->inner[t->numInner-1])->outer = t;
	return t->inner[t->numInner-1];
}

void printSymbolTable() {
	printf("-----------SYMBOL TABLE-----------\n");
	while (symbolTables->symbols) {
		printf("Level %lu Symbol: %s\n",symbolTables->level,symbolTables->symbols->name);
		symbolTables->symbols = symbolTables->symbols->next;
	}
	printf("----------------------------------\n");
}

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

void createSymbolTableVarDecl(struct SymbolTable *symTab, struct VarDecl *var) {
	if (!lookUpNameCurrentScope(var->name,symTab)) return;
	struct Symbol *s = createSymbol(var->name,var->type,symTab->level == 0 ? SYMBOL_GLOBAL : SYMBOL_LOCAL,VAR);
	addSymbol(symTab,s);
	printf("Entered variable %s\n",var->name);
}

void createSymbolTableFuncDecl(struct SymbolTable *symTab, struct FuncDecl *func) {
	if (!lookUpNameCurrentScope(func->name,symTab)) return;
	struct Symbol *s = createSymbol(func->name,func->type,SYMBOL_GLOBAL,FUNC);
	addSymbol(symTab,s);
	printf("entered function %s\n",func->name);
}

void createSymbolTableStatements(struct SymbolTable *symTab, struct Statement *statement) {
	struct Statement *s = statement;
	while (s) {
		if (s->stmt == DECLARATION) {
			createSymbolTableVarDecl(symTab,s->var);
		}
		s = s->next;
	}
}

void createSymbolTableParams(struct SymbolTable *symTab, struct Params *param) {
	if (!lookUpNameCurrentScope(param->name,symTab)) return;
	struct Symbol *s = createSymbol(param->name,param->type,SYMBOL_LOCAL,VAR);
	addSymbol(symTab,s);
	printf("entered param %s\n",param->name);
}


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

int checkAll() {
	symbolTables = calloc(1,sizeof(struct SymbolTable));
	symbolTables->numInner = 0;
	symbolTables->level = 0;
	createSymbolTableDeclarations(symbolTables,syntaxTree);
	printSymbolTable();
	printf("Checked all semantics\n");
	return 0;
}
