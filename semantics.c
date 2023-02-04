#include "syntaxtree.h"
#include <stdlib.h>
#include <string.h>

extern struct Declaration *syntaxTree;
extern char **lines;

unsigned long errorNumber = 1; //used to list all errors
unsigned char inLoop = 0;

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
	printf("Resolving type\n");
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


/*
 * Given an expression, resolve all subexpression types given operations between two values and return the type computed when reduced all the way.
 */
struct Type *typeCheckExpr(struct Expression *expr) {
	struct Evaluation **evalStack = malloc(sizeof(struct Evaluation*));
	operation_t **opStack = malloc(sizeof(operation_t *));
	long stackIndex = 0;
	long opStackIndex = 0;
	struct Expression *temp = expr;
	while (temp) {
		struct Evaluation *e = temp->eval;
		evalStack = realloc(evalStack,sizeof(struct Evaluation*) * ++stackIndex);
		evalStack[stackIndex-1] = e;

		operation_t *o = temp->op;
		if (o) {
			opStack = realloc(opStack,sizeof(operation_t *) * ++opStackIndex);
			opStack[opStackIndex-1] = o;
		}

		temp = temp->expr;
	}
	long eval1 = stackIndex - 1;
	long eval2 = stackIndex - 2;
	long opIndex = opStackIndex - 1;
	while (eval2 > -1) {
		resolveType(evalStack[eval1--],opStack[opIndex--],evalStack[eval2--]);
	}
	return evalStack[0]->type;
}

/*
 * Checking to see if assignment is legal between an expression into a variable
 */
struct Type *typeCheckAssignment(struct VarDecl *var, struct Expression *expr) {
	//struct Type *varType = var->type;
	//struct Type *exprType = expr->eval->type;
	return NULL;
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

/*
 * looks up the name of an identifier within the symbol table
 * begins at current scope and goes backwards to determine is symbol has been added to the table
 * in current level or levels below it
 */
struct Symbol *lookUpName(char *name, struct SymbolTable *symTab) {
	struct SymbolTable *t = symTab;
	while (t) {
		struct Symbol *s = lookUpNameCurrentScope(name,t);
		if (s) return s;
		t = t->outer;
	}

	return NULL;
}

/*
 * Given the error number and information prints out error messages in terms of the semantics of the language.
 */
void printError(int errorNum, char *name,unsigned long line1, unsigned long line2) {
	printf("Error #%lu:\n",errorNumber);
	switch(errorNum) {
		case 1:
			printf("Redeclaration of identifier \"%s\" attempted on line [%lu] and declared on line [%lu] within the same scope.\n",name,line1,line2);
			printLine(line2);
			printLine(line1);
			printf("To fix: either rename one of the identifiers and all uses of it or declare one of the identifiers in another scope.\n");
			break;
		case 2:
			printf("Identifier \"%s\" on line [%lu] not declared.\n",name,line1);
			printLine(line1);
			printf("To fix: declare the variable in a previous line or make this line a declaration\n");
			break;
		case 3:
			printf("Identifier \"%s\" on line [%lu] is declared as a variable and is attempting to use it as a function call\n",name,line1);
			printLine(line1);
			printf("To fix: either the name of the function is wrong or remove the parentheses\n");
			break;
		case 4:
			printf("Control flow statement outside of loop.\n");
			printf("To fix: remove this statement or put it within a loop.\n");
			break;
	}
	printf("\n");
	errorNumber++;
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
		printError(1,x->name,func->line,x->line);
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

/*
 * Looks up in the symbol table if the symbol exists exists
 */
void resolveEval(struct SymbolTable *symTab, struct Evaluation *eval) {
	if (eval->eval != VALUE) {
		eval->symbol = lookUpName(eval->name,symTab);
		if (!eval->symbol) printError(2,eval->name,eval->line,0);
	}
}
/*
 * Recursively looks up all evaluations in the symbol table within the expression
 */ 
void resolveExpr(struct SymbolTable *symTab, struct Expression *expr) {
	if (!expr) return;
	resolveExpr(symTab,expr->expr);
	resolveEval(symTab,expr->eval);
}

/*
 * Checks to see if the variable being assigned exists within the symbol table
 */
void resolveAssignment(struct SymbolTable *symTab, struct VarDecl *var) {
	var->symbol = lookUpName(var->name,symTab);
	if (!var->symbol) printError(2, var->name,var->line,0);
}

/*
 * determines if function call is within the symbol table and if it truly is a function
 */
void resolveFuncCall(struct SymbolTable *symTab, struct FunctionCall *funccall) {
	funccall->symbol = lookUpName(funccall->name,symTab);
	if (!funccall->symbol) {
		printError(2, funccall->name,funccall->symbol->line,0);
		return;
	}
	if (funccall->symbol->dec == VAR) {
		printError(3, funccall->name,funccall->symbol->line,0);
		return;
	}
}
/*
 * If the control statement is not within a loop then print error.
 */
void resolveControl(statement_t stmt) {
	if (!inLoop) {
		printError(4,NULL,0,0);
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
			inLoop = 1;
			struct SymbolTable *innerStmts = addInner(symTab);
			createSymbolTableVarDecl(innerStmts,s->loop->init);
			resolveAssignment(innerStmts,s->loop->mod);
			resolveExpr(innerStmts,s->loop->mod->expr);
			resolveExpr(innerStmts,s->loop->expr);

			createSymbolTableStatements(innerStmts,s->loop->stmts);
			
		}
		if (s->stmt == WHILE) {
			inLoop = 1;
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
			typeCheckExpr(s->returnstmt); //testing type checking for return statements be sure to include this function in other lines
		}
		if (s->stmt == ASSIGNMENT) {
			resolveAssignment(symTab,s->var);//var assign
			if(s->var->type) resolveExpr(symTab,s->var->type->length);//array size if applicable
			resolveExpr(symTab,s->var->expr);//right hand side of ass
		}
		if ((s->stmt == BREAK) || (s->stmt == CONTINUE)) {
			resolveControl(s->stmt);
		}

		inLoop = 0;

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
