#include "syntaxtree.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * This file deals with the semantics of the language after being given a syntax tree from the parser.
 * Some semantics include the symbol table, expression resolution, etc.
 */

extern struct Declaration *syntaxTree;
extern char **lines;

unsigned long errorNumber = 1; //used to list all errors
unsigned char inLoop = 0;//if currently in loop

/*
 * Symbol contains a linked list of symbols in reverse order of appearence
 * Inner Symbol Tables the represent statements such as conditional and loop (anything enclosed in curly braces)
 * the level corresponds to how "deep" in the symbol table tree the current symbol table is starting at 0 for the root
 * Outer represents the higher up symbol table which is important for determining if a symbol has already been defined
 */
struct SymbolTable {
	struct Symbol *symbols;//linked list of symbols at current level
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
	struct Symbol *s = calloc(1,sizeof(struct Symbol));
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


/*

CHAR SHORT INT LONG can be manipulated with any of the operators or comparisons, they will be converted to the bigger type before computed.

 */

/*
* For inferring literals
*/
type_t determineSmallestType(char *str) {
    char *endptr;  // Pointer to store the end of conversion
	long value = strtol(str, &endptr, 10);
	if (endptr == str) {
        printf("No valid digits were found.\n");
		return -1;
    } 
	else if (*endptr != '\0') {
        printf("Invalid character encountered: %c\n", *endptr);
		return -1;
    } 
	else if (value < SCHAR_MAX && value > SCHAR_MIN) {
		printf("Inferred type of number: %ld | CHAR\n", value);
		return CHAR;
	}
	else if (value < SHRT_MAX && value > SHRT_MIN) {
		printf("Inferred type of number: %ld | SHORT\n", value);
        return SHORT;
    } 
	else if (value < INT_MAX && value > INT_MIN) {
		printf("Inferred type of number: %ld | INT\n", value);
		return INT;
	}
	else {
		printf("Inferred type of number: %ld | LONG\n", value);
        return LONG;
    }
}

/*
* Infers type of a literal value
*/
struct Type *inferLiteral(struct Value *value) {
	char *val = value->value;
	//printf("Literal to infer: %s\n",val);
	if (value->val_t == NUM) {
		struct Type *type = calloc(1,sizeof(struct Type));
		type->pointer = 0;
		type->length = NULL;
		type->sign = SIGNED;

		if (memchr(val, '.', sizeof(val)) != NULL) {
			type->dataType = DOUBLE;
			printf("DOUBLE\n");
		}
		else {
			type->dataType = determineSmallestType(val);
		}
		type->dataType = CHAR;
		return type;
	}
	else if(value->val_t == CHARCONST) {
		struct Type *type = calloc(1,sizeof(struct Type));
		type->dataType = CHAR;
		type->sign = UNSIGNED;
		type->pointer = 0;
		type->length = NULL;
		return type;
	}
	else { //STRING
		printf("STRING INFERENCE-------------\n");
		struct Type *type = calloc(1,sizeof(struct Type));
		type->dataType = LONG;
		type->sign = UNSIGNED;
		type->pointer = 1;
		type->length = NULL;
		return type;
	}
}

struct Type *typeCheckExpr();

/*
* Gets type of eval (VALUE or other)
*/
struct Type *getType(struct Evaluation *eval) {
	if (eval->eval == VALUE) {
		//printf("Inferring literal value of %s\n",eval->value->value);
		return inferLiteral(eval->value);
	}
	else if (eval->eval == FUNCRETURN) {
		struct FunctionArgs *fargs = eval->funccall->funcargs;
		while(fargs) {
			typeCheckExpr(fargs->expr);
			fargs = fargs->funcargs;
		}
		return eval->funccall->symbol->type;
	}
	else {
		printf("EvAL: %d PROBLEMA? %d-----------------------\n",eval->eval,eval->symbol->type->pointer);
		return eval->symbol->type;
	}
}
/*
 * Type translation given operators and 1 or 2 Evaluations.
 */
struct Type *resolveType(struct Evaluation *eval1, operation_t *op, struct Evaluation *eval2) {
	//if (!eval1) return NULL;
	//if (eval1->value) printf("Infer value %s\n",eval1->value->value);
	if (!eval2) {
		return eval1->type = getType(eval1);
	}
	/*switch (*op) {
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
	*/
	struct Type *type1 = getType(eval1);
	struct Type *type2 = getType(eval2);

	if (type1->dataType >= type2->dataType) {//the biggest data type takes precedence
		type2->dataType = type1->dataType;
	}
	else {
		type1->dataType = type2->dataType;
	}
	if (type1->sign == SIGNED || type2->sign == SIGNED) {//sign takes precedence
		type1->sign = SIGNED;
		type2->sign = SIGNED;
	}
	if (type1->pointer || type2->pointer) {//both become points
		printf("THERE IS ONE POINTER -----------");
		printf("%d %d\n",type1->pointer,type2->pointer);
		type1->pointer = 1;
		type2->pointer = 1;
	}
	eval1->type = type1;
	eval2->type = type2;

	return type2;
}


/*
 * Given an expression, resolve all subexpression types given operations between two values and return the type computed when reduced all the way.
 */
struct Type *typeCheckExpr(struct Expression *expr) {
	if (!expr) printf("Expression null\n");
	if (!expr) return NULL; //sholdn't be null, might have to find the cause
	struct Evaluation **evalStack = calloc(1,sizeof(struct Evaluation*));
	operation_t **opStack = calloc(1,sizeof(operation_t *));
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
	//printf("Expression length %ld Operation length %ld\n",stackIndex,opStackIndex);
	long eval1 = stackIndex - 1;
	long eval2 = stackIndex - 2;
	long opIndex = opStackIndex - 1;
	while (eval2 > -1) {
		resolveType(evalStack[eval1--],opStack[opIndex--],evalStack[eval2--]);
	}
	//printf("Num: %ld\n",eval1);
	resolveType(evalStack[eval1--],NULL,NULL); //resolves single expressions
	//printf("test\n");
	if (!evalStack[0]) return NULL;
	//printf("Type: %d Name: %s\n",evalStack[0]->type->dataType,evalStack[0]->name);
	free(opStack);
	struct Type *ret = evalStack[0]->type;
	free(evalStack);
	return ret;
}

/*
 * Checking to see if assignment is legal between an expression into a variable
 */
struct Type *typeCheckAssignment(struct Type *varType, struct Expression *expr) {
	if (!expr) printf("Expression NULL here\n");
	struct Type *exprType = typeCheckExpr(expr);

	printf("before comparison pointer %d\n",exprType->pointer);
	printf("varType pointer %d\n",varType->pointer);
	exprType->sign = varType->sign;
	exprType->pointer = varType->pointer;
	if (varType->dataType >= exprType->dataType) {//the biggest data type takes precedence
		exprType->dataType = varType->dataType;
	}
	else {
		printf("Variable type data loss (left side has smaller data type than the right).\n");
		return NULL;
	}
	printf("within typeCheckAss pointer of X? %d\n",exprType->pointer);
	return varType;
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

int resolveExpr();

/*
 * takes a var decl to create a symbol to add to current scope
 */
void createSymbolTableVarDecl(struct SymbolTable *symTab, struct VarDecl *var) {
	struct Symbol *x = lookUpNameCurrentScope(var->name,symTab);
	if (x) {
		printError(1,x->name,var->line,x->line);
		return;
	}
	printf("before symbol created var type: %d\n",var->type->pointer);
	struct Symbol *s = createSymbol(var->name,var->type,symTab->level == 0 ? SYMBOL_GLOBAL : SYMBOL_LOCAL,VAR,var->line);
	addSymbol(symTab,s);
	var->symbol = s;
	//typeCheckExpr(var->expr);
	printf("var symbol type pointer %d\n",var->symbol->type->pointer);
	printf("before typeCheck pointer of var %d\n",var->type->pointer);
	int exprResolved = resolveExpr(symTab,var->expr);
	if (exprResolved) typeCheckAssignment(var->type,var->expr);
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
		printError(1,x->name,param->var->line,x->line);
		return;
	}
	struct Symbol *s = createSymbol(param->var->name,param->var->type,SYMBOL_LOCAL,VAR,param->var->line);
	addSymbol(symTab,s);
	param->var->symbol = s;
}

int resolveFuncCall();

/*
 * Looks up in the symbol table if the symbol exists exists
 * Return 0 if you cannot find symbol else return 1
 */
int resolveEval(struct SymbolTable *symTab, struct Evaluation *eval) {
	printf("TRYING TO RESOLVE EVAL\n");
	printf("Function: resolveEval Trying to resolve eval %d\n",eval->eval);
	if (eval->eval == FUNCRETURN) {
		printf("going to resolve FuncCall\n");
		return resolveFuncCall(symTab,eval->funccall);
	}
	if (eval->eval != VALUE) {
		eval->symbol = lookUpName(eval->name,symTab);
		if (!eval->symbol) {
			printError(2,eval->name,eval->line,0);
			return 0;
		}
		return 1;
	}
	return 1;
}
/*
 * Recursively looks up all evaluations in the symbol table within the expression
 * Return 1 if all found or 0 if one is not found
 */ 
int resolveExpr(struct SymbolTable *symTab, struct Expression *expr) {
	if (!expr) return 1;
	return 1 * resolveExpr(symTab,expr->expr) * resolveEval(symTab,expr->eval);
}

/*
 * Checks to see if the variable being assigned exists within the symbol table
 * Return 0 if you cannot find symbol else return 1 (Success)
 */
int resolveAssignment(struct SymbolTable *symTab, struct VarDecl *var) {
	var->symbol = lookUpName(var->name,symTab);
	if (!var->symbol) {
		printError(2, var->name,var->line,0);
		return 0;
	}
	var->type = var->symbol->type;
	return 1;
}

/*
 * determines if function call is within the symbol table and if it truly is a function
 */
int resolveFuncCall(struct SymbolTable *symTab, struct FunctionCall *funccall) {
	funccall->symbol = lookUpName(funccall->name,symTab);
	if (!funccall->symbol->type) printf("NULL HERE\n");
	if (!funccall->symbol) {
		printError(2, funccall->name,funccall->symbol->line,0);
		return 0;
	}
	if (funccall->symbol->dec == VAR) {
		printError(3, funccall->name,funccall->symbol->line,0);
		return 0;
	}
	struct FunctionArgs *fargs = funccall->funcargs;
	while (fargs) {
		if (resolveExpr(symTab, fargs->expr) == 0) return 0;
		fargs = fargs->funcargs;
	}
	printf("finished\n");
	return 1;
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
void createSymbolTableStatements(struct SymbolTable *symTab, struct Statement *statement, struct Type *returnType) {
	struct Statement *s = statement;
	while (s) {
		if (s->stmt == DECLARATION) {
			printf("RESOLVING DECLARATION of %s\n", s->var->name);
			createSymbolTableVarDecl(symTab,s->var);//var decl
			printf("CRETED SYMBOL FOR VARIABLE\n");
			//if(s->var->type) resolveExpr(symTab,s->var->type->length);//array size (if applicable)
			int resolvedExpr = resolveExpr(symTab,s->var->expr);//right hand side of dec
			printf("EXPRESSION OF DECLARATION RESOLVED\n");
			printf("pointer of %s? %d\n",s->var->name,s->var->type->pointer);
			
			if (resolvedExpr) typeCheckAssignment(s->var->type,s->var->expr);
			printf("\n\n");
		}
		if (s->stmt == FOR) {
			inLoop = 1;
			struct SymbolTable *innerStmts = addInner(symTab);
			createSymbolTableVarDecl(innerStmts,s->loop->init);

			int resolvedAss = resolveAssignment(innerStmts,s->loop->mod);
			int resolvedExpr1 = resolveExpr(innerStmts,s->loop->mod->expr);
			int resolvedExpr2 = resolveExpr(innerStmts,s->loop->expr);

			if (resolvedAss && resolvedExpr1) typeCheckAssignment(s->loop->mod->type,s->loop->mod->expr);
			if (resolvedExpr2) typeCheckExpr(s->loop->expr);

			createSymbolTableStatements(innerStmts,s->loop->stmts,returnType);
			
		}
		if (s->stmt == WHILE) {
			inLoop = 1;
			struct SymbolTable *innerStmts = addInner(symTab);
			int resolvedExpr = resolveExpr(innerStmts,s->loop->expr);

			if (resolvedExpr) typeCheckExpr(s->loop->expr);

			createSymbolTableStatements(innerStmts,s->loop->stmts,returnType);
		}
		if (s->stmt == IF) {
			struct CondStatement *c = s->condstmt;
			while (c) {
				struct SymbolTable *innerStmts = addInner(symTab);
				int resolvedExpr = resolveExpr(innerStmts,c->expr);//resolves condition expression

				if (resolvedExpr) typeCheckExpr(c->expr);

				createSymbolTableStatements(innerStmts,c->stmts,returnType);
				c = c->next;
			}
		}
		if (s->stmt == FUNCCALL) {
			resolveFuncCall(symTab,s->funccall);
			struct FunctionArgs *f = s->funccall->funcargs;
			while (f) {
				int resolvedExpr = resolveExpr(symTab,f->expr);

				if (resolvedExpr) typeCheckExpr(f->expr);
				f = f->funcargs;
			}
		}
		if (s->stmt == RETURN) {
			printf("RESOLVING RETURN\n");
			int resolvedExpr = resolveExpr(symTab,s->returnstmt);
			printf("Expression resolve?: %d\n",resolvedExpr);
			//remember to change this line to typeCheck return for function return type
			if (resolvedExpr) {
				typeCheckExpr(s->returnstmt); 
				printf("Type checked\n");
				if (!returnType) printf("NOT GOOOOOOOOOOOOOOD\n");
				typeCheckAssignment(returnType,s->returnstmt);
			}
			//testing type checking for return statements be sure to include this function in other lines
		}
		if (s->stmt == ASSIGNMENT) {
			printf("RESOLVING ASSIGNMENT of %s\n",s->var->name);
			int resolvedAss = resolveAssignment(symTab,s->var);//var assign
			if(s->var->type) resolveExpr(symTab,s->var->type->length);//array size if applicable
			int resolvedExpr = resolveExpr(symTab,s->var->expr);//right hand side of ass
			printf("TYPE CHECKING ASSIGNMENT\n");
			if (resolvedAss && resolvedExpr) typeCheckAssignment(s->var->type,s->var->expr); //if assignment is resolved you can type check
			
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
			//resolve type of right side of equal sign
			typeCheckExpr(d->vardecl->expr);
		}
		else {
			createSymbolTableFuncDecl(initial,d->funcdecl);
			struct SymbolTable *inFunc = addInner(initial);
			struct Params *tempParam = d->funcdecl->params;
			while (tempParam) {
				createSymbolTableParams(inFunc,tempParam);
				tempParam = tempParam->next;
			}
			createSymbolTableStatements(inFunc,d->funcdecl->statements,d->funcdecl->type);
		}
		d = d->next;
	}
}

/*
 * Mostly done
 */ 
int checkAll() {
	symbolTables = calloc(1,sizeof(struct SymbolTable));
	symbolTables->numInner = 0;
	symbolTables->level = 0;
	createSymbolTableDeclarations(symbolTables,syntaxTree);
	printSymbolTable(symbolTables);
	printf("Checked all semantics\n");
	if (errorNumber == 1) return 1;
	else return 0;
}
