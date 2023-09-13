#include "syntaxtree.h"
#include "ir.h"

/***
 * This file is responsible for the Intermediate Representation which utilizes Quadruples
 * Each Quadruple contains:
 * an Operation (enum op)
 * two Arguments (struct argument) which contains a type and a name or a value
 * a result which is a name
 * Optimizations include constant folding which is when you reduce a constant expression ex. 5 + 5 into 10 
 * which saves an instruction
*/

struct quad **quads;//array of quads
int numQuads = 0;//index of quads

int temp = 0;//used to create temp names for storing values
int label = 0;//used for label names

/***
 * 3 functions create a name tX or rX where X is the number 
*/
char *createTempName(int num) {
    int totalLength = snprintf(NULL, 0, "t%d", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, "t%d", num);
    }
    temp++;
    return result;
}

char *createLabelName(int num) {
    int totalLength = snprintf(NULL, 0, ".L%d", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, ".L%d", num);
    }
    label++;
    return result;
}

char *createResultName(int num) {
    int totalLength = snprintf(NULL, 0, "r%d", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, "r%d", num);
    }
    return result;
}


char *createName(char *c, int num) {
    if (strcmp(c,"t") == 0) {
        return createTempName(num);
    }
    else if (strcmp(c,"l") == 0) {
        return createLabelName(num);
    }
    else {
        return createResultName(num);
    }
}

char *getTempName(int num) {
    int totalLength = snprintf(NULL, 0, "t%d", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, "t%d", num);
    }
    temp++;
    return result;
}
/**
 * 
*/

/***
 * Allocates memory for quads
*/
struct quad *createQuad(struct argument *arg1, struct argument *arg2, enum op operation, char *result) {
    struct quad *q = calloc(1,sizeof(struct quad));
    q->arg1 = arg1;
    q->arg2 = arg2;
    q->operation = operation;
    q->result = result;
    return q;
}

struct quad *createQuad2(struct argument *arg1, struct argument *arg2, enum op operation, struct Symbol *result) {
    struct quad *q = calloc(1,sizeof(struct quad));
    q->arg1 = arg1;
    q->arg2 = arg2;
    q->operation = operation;
    q->result = result->name;
    q->symbol = result;
    return q;
}

/**
 * Adds quad to quad array
*/
char *addQuad(struct quad *quad) {
    quads = realloc(quads,sizeof(struct quad) * (1 + numQuads));
    quads[numQuads] = quad;
    numQuads++;
    //printf("ADDED QUAD WITH RESULT: %s\n", quad->result);
    return quad->result;
}

/**
 * Allocates memory for an argument struct with either a name or a value
*/
struct argument *createArg(char *name, enum val_t val, long value) {
    struct argument *a = calloc(1,sizeof(struct argument));
    a->val_t = val;
    if (name) a->name = name;
    else {
        a->val_t = val;
        switch (val) {
            case VAL_CHAR:
                a->value.char_value = (char)value;
                break;
            case VAL_UCHAR:
                a->value.uchar_value = (unsigned char)value;
                break;
            case VAL_SHORT:
                a->value.short_value = (short)value;
                break;
            case VAL_USHORT:
                a->value.ushort_value = (unsigned short)value;
                break;
            case VAL_INT:
                a->value.int_value = (int)value;
                break;
            case VAL_UINT:
                a->value.uint_value = (unsigned int)value;
                break;
            case VAL_LONG:
                a->value.long_value = (long)value;
                break;
            case VAL_ULONG:
                a->value.ulong_value = (unsigned long)value;
                break;
            case VAL_DOUBLE:
                a->value.double_value = (double)value;
                break;
            default:
                printf("VAL NOT FOUND %d\n",val);
        }
    }
    return a;
}

/**
 * Converts a type struct into a val_t enum
*/
val_t getTypeQuad(struct Type *type) {
    if (!type) printf("TYPE NULL WHEN GETTING TYPE ERROR\n");
    if (type->pointer > 0) {
        printf("POINTER or ARRAY POINTER\n");
        return type->length != NULL ? VAL_POINTER : VAL_ARR_POINTER;
    }
    switch (type->dataType) {
        case CHAR:
            if (type->length) return type->sign == SIGNED ? VAL_ARR_CHAR : VAL_ARR_UCHAR;
            else return type->sign == SIGNED ? VAL_CHAR : VAL_UCHAR;
        case SHORT:
            if (type->length) return type->sign == SIGNED ? VAL_ARR_SHORT : VAL_ARR_USHORT;
            else return type->sign == SIGNED ? VAL_SHORT : VAL_USHORT;
        case INT:
            if (type->length) return type->sign == SIGNED ? VAL_ARR_INT : VAL_ARR_UINT;
            else return type->sign == SIGNED ? VAL_INT : VAL_UINT;
        case LONG:
            if (type->length) return type->sign == SIGNED ? VAL_ARR_LONG : VAL_ARR_ULONG;
            return type->sign == SIGNED ? VAL_LONG : VAL_ULONG;
        case DOUBLE:
            if (type->length) return VAL_ARR_DOUBLE;
            else return VAL_DOUBLE; //no such thing as unsigned double
    }
    printf("NO TYPE FOUND\n");
    return -1;
}
char *createQuadFuncCall();


/**
 * Creates an argument struct given an eval
*/
struct argument *evalToArg(struct Evaluation *eval) {
    if (!eval->type) {
        printf("error no type of eval %s\n",eval->value->value);
    }
    if (eval->eval == ID) {
        if (!eval->type) printf("no eval name\n");
        return createArg(eval->name,getTypeQuad(eval->type),0);
    }
    if (eval->eval == VALUE) {
        return createArg(NULL,getTypeQuad(eval->type),strtol(eval->value->value,NULL,10));
    }
    if (eval->eval == FUNCRETURN) {
        char *tempName = createQuadFuncCall(eval->funccall);
        return createArg(tempName,getTypeQuad(eval->type),0);
    }
    if (eval->eval == REF) {
        printf("REFERENCE\n");
        char *tempName = addQuad(createQuad(createArg(eval->name,VAL_POINTER,0),NULL,OP_REF,createName("t",temp)));
        return createArg(tempName,VAL_POINTER,0);
    }
    if (eval->eval == DEREF) {
        printf("DEREREFENCE\n");
        char *tempName = addQuad(createQuad(createArg(eval->name,getTypeQuad(eval->type),0),NULL,OP_DEREF,createName("t",temp)));
        return createArg(tempName,getTypeQuad(eval->type),0);
    }
    if (eval->eval == ARRAYINDEX) {
        printf("ARRAYINDEX\n");
        if (!eval->type) printf("eval type null\n");
        if (!eval->type->length->eval->type) printf("eval length type null\n");
        char *tempName = addQuad(createQuad(createArg(eval->name,getTypeQuad(eval->type),0),evalToArg(eval->type->length->eval),OP_ARRAY_INDEX,createName("t",temp)));
        return createArg(tempName,getTypeQuad(eval->type),0);
    }
    return NULL;
}
/**
 * converts op from syntax tree to op in IR
*/
op getQuadOp(operation_t op) {
    switch(op) {
        case PLUS: return OP_ADD;
        case MINUS: return OP_SUB;
        case MULT: return OP_MULT;
        case DIV: return OP_DIV;
        case MOD: return OP_MOD;
        case BITWISEAND: return OP_BITAND;
        case BITWISEOR: return OP_BITOR;
        case BITWISEXOR: return OP_BITXOR;
        case EQUAL: return OP_EQ;
        case GREATEQUAL: return OP_GEQ;
        case LESSEQUAL: return OP_LEQ;
        case LESS: return OP_LESS;
        case GREAT: return OP_GREAT;
        case AND: return OP_AND;
        case NOT: return OP_NEQ;
        case OR: return OP_OR;
        default: return -1;
    }
}

/**
 * generic Quad creator to create long expression 
 * single or null expressions are dealt with on higher-up functions (THEY SHOULD BE)
*/
char *createQuadExpr(struct Expression *expr) {
    //a b c d
    //a b -> t1
    //t1 c -> t2
    //t2 d -> t3
    struct Expression *e = expr;

    //creates quad using the first two evals in an expression
    char *tempName = addQuad(createQuad(evalToArg(e->eval),evalToArg(e->expr->eval),getQuadOp(*e->op),createName("t",temp)));
    e = e->expr->expr;
    struct Expression *op = expr->expr; //storing operation
    while (e) {
        tempName = addQuad(createQuad(createArg(tempName,-1,0),evalToArg(e->eval),getQuadOp(*op->op),createName("t",temp)));
        e = e->expr;
        op = op->expr;
    }
    return tempName;
}

/**
 * creates a quad for an array
*/
void createQuadArr(struct VarDecl *var) {
    if (var->symbol->line == var->line) {//declaration of array
        printf("declaration of array %s\n",var->name);
        struct argument *tempSize;
        
        //size/index of array
        /*if (var->type->length->expr) {
            //multi eval expression
            printf("multi eval\n");
            tempNameSize = createQuadExpr(var->type->length);
        }*/
        //else {
        //single eval expression
        tempSize = evalToArg(var->type->length->eval);
        //}

        char *tempNameValue;

        //value assigned to array index
        if (var->expr){//value assigned
            if (!var->expr->expr) {
                //single eval expression
                addQuad(createQuad2(tempSize,evalToArg(var->expr->eval),OP_ARRAY_CREATE,var->symbol));
                return;
            }
            //multi eval expression
            else tempNameValue = createQuadExpr(var->expr);
        }
        else {//no value assigned
            tempNameValue = NULL;
        }
        if (tempNameValue) addQuad(createQuad2(tempSize,createArg(tempNameValue,getTypeQuad(var->expr->eval->type),0),OP_ARRAY_CREATE,var->symbol));
        else addQuad(createQuad2(tempSize,NULL,OP_ARRAY_CREATE,var->symbol));
    }
    else {//assignment
        if (!var->expr->expr) {//single eval expression (for value)
            if (!var->type->length->expr) {//single eval expression (for index)
                addQuad(createQuad2(evalToArg(var->type->length->eval),evalToArg(var->expr->eval),OP_ARRAY_INDEX,var->symbol));
            }
            else {//multi eval expression (for index)
                char *tempName = createQuadExpr(var->type->length);
                addQuad(createQuad2(createArg(tempName,VAL_ULONG,0),evalToArg(var->expr->eval),OP_ARRAY_INDEX,var->symbol));
            }
        }
        else {//multi eval expression (for value)
            if (!var->type->length->expr) {//single eval expression (for index)
                printf("multi single\n");
                char *tempName = createQuadExpr(var->expr);
                addQuad(createQuad2(evalToArg(var->type->length->eval),createArg(tempName,getTypeQuad(var->expr->eval->type),0),OP_ARRAY_INDEX,var->symbol));
            }
            else {//multi eval expression (for index)
                printf("multi multi\n");
                char *tempNameIndex = createQuadExpr(var->type->length);
                char *tempNameValue = createQuadExpr(var->expr);
                addQuad(createQuad2(createArg(tempNameIndex,VAL_ULONG,0),createArg(tempNameValue,getTypeQuad(var->expr->eval->type),0),OP_ARRAY_INDEX,var->symbol));
            }
        }
    }
}
/**
 * Creates a quad for a variable declaration
 * 
 * a = b + c + d + e;
 * 
 * t1 = b + c;
 * t2 = t1 + d;
 * t3 = t2 + e;
 * a = t3
*/
void createQuadVar(struct VarDecl *var) {
    struct Expression *e = var->expr; 
    if (var->type->length) {
        createQuadArr(var);
        return;
    }
    if (!e) {
        addQuad(createQuad2(createArg(NULL,getTypeQuad(var->type),0),NULL,OP_ASSIGN,var->symbol));
        return;
    }
    if (!e->expr) {
        addQuad(createQuad2(evalToArg(e->eval), NULL, OP_ASSIGN, var->symbol));
        return;
    }
    char *tempName = createQuadExpr(var->expr);
    addQuad(createQuad2(createArg(tempName,getTypeQuad(var->type),0), NULL, OP_ASSIGN, var->symbol));
}

void printQuad();

/**
 * creates a quad for a return statement
*/
void createQuadReturn(struct Expression *expr) {
    if (!expr) {
        struct quad *quad = createQuad(NULL,NULL,OP_RET,NULL);
        addQuad(quad);
        printQuad(quad);
        return;
    }
    struct Expression *e = expr;
    if (!e->expr) {
        addQuad(createQuad(evalToArg(e->eval), NULL, OP_RET, NULL));
        return;
    }
    char *tempName = createQuadExpr(expr);
    addQuad(createQuad(createArg(tempName,getTypeQuad(expr->eval->type),0),NULL,OP_RET,NULL));
}


void createQuadStatements(struct Statement *stmt);

/**
 * Creates quad for a conditional statement
 * if statements looks for elseif to jump to followed by else
*/
void createQuadConditional(struct CondStatement *cond) {
    char *tempName = createQuadExpr(cond->expr);
    char *labelName = createName("l",label);
    addQuad(createQuad(createArg(tempName,VAL_LONG,0),NULL,OP_JUMPIFNOT,labelName));
    createQuadStatements(cond->stmts);
    cond = cond->next;
    while (cond && cond->stmt != ELSE) {//elseif
        addQuad(createQuad(NULL,NULL,OP_LABEL,labelName));

        tempName = createQuadExpr(cond->expr);
        labelName = createName("l",label);
        addQuad(createQuad(createArg(tempName,VAL_LONG,0),NULL,OP_JUMPIFNOT,labelName));
        createQuadStatements(cond->stmts);
        cond = cond->next;
    }
    //else
    if (cond) {
        addQuad(createQuad(NULL,NULL,OP_LABEL,labelName));

        createQuadStatements(cond->stmts);
    }
}

void createQuadLoop(struct Loop *loop) {
    if (loop->stmt == FOR) {
        createQuadVar(loop->init);
    }
    char *labelStartLoop = createName("l",label);
    char *labelEndLoop = createName("l",label);

    addQuad(createQuad(createArg("loop_start",-1,-1),NULL,OP_LABEL,labelStartLoop));

    char *tempName = createQuadExpr(loop->expr);
    addQuad(createQuad(createArg(tempName,VAL_LONG,0),createArg("jump_end",-1,-1),OP_JUMPIFNOT,labelEndLoop));
    
    createQuadStatements(loop->stmts);
    if (loop->stmt == FOR) {
        createQuadVar(loop->mod);
    }
    addQuad(createQuad(NULL,createArg("jump_start",-1,-1),OP_JUMP,labelStartLoop));
    addQuad(createQuad(createArg("loop_end",-1,-1),NULL,OP_LABEL,labelEndLoop));
}

/**
 * creates a quad for a break statement
 * finds the lop associated with it
*/
void createQuadBreak(int currentIndex) {
    int numInner = 0;
    while (currentIndex > 1) {
        if (quads[currentIndex]->operation == OP_LABEL && strcmp(quads[currentIndex]->arg1->name,"loop_end") == 0) numInner++; //find corresponding loop
        else if (quads[currentIndex]->operation == OP_JUMPIFNOT && strcmp(quads[currentIndex]->arg2->name,"jump_end") == 0) {
            if (numInner > 0) numInner--;
            else addQuad(createQuad(NULL,NULL,OP_JUMP,quads[currentIndex]->result));
        }
        currentIndex--;
    }
}

/**
 * creates quad for a continue statement
 * finds the loop associated with it if another loop precedes it
*/
void createQuadContinue(int currentIndex) {
    int numInner = 0;
    while (currentIndex > 1) {
        if (quads[currentIndex]->operation == OP_LABEL && strcmp(quads[currentIndex]->arg1->name,"loop_end") == 0) numInner++; //find corresponding loop
        else if (quads[currentIndex]->operation == OP_JUMPIFNOT && strcmp(quads[currentIndex]->arg2->name,"jump_end") == 0) {
            numInner--;
        }
        if (quads[currentIndex]->operation == OP_LABEL && strcmp(quads[currentIndex]->arg1->name,"loop_start") == 0) {
            addQuad(createQuad(NULL,NULL,OP_JUMP,quads[currentIndex]->result));
        }
        currentIndex--;
    }
}

/**
 * quad for a function call
*/
char *createQuadFuncCall(struct FunctionCall *call) {
    struct FunctionArgs *count = call->funcargs;
    unsigned long num = 0;
    while (count) {
        num++;
        count = count->funcargs;
    }
    char *tempName = addQuad(createQuad(createArg(call->name,-1,0),createArg(NULL,VAL_ULONG,num),OP_CALL,createName("t",temp)));
    struct FunctionArgs *args = call->funcargs;
    while (args) {
        if (!args->expr->expr) {
            printf("Problem starts here 1\n");
            if (!args->expr->eval) printf("Problem lol\n");
            addQuad(createQuad(evalToArg(args->expr->eval), NULL, OP_PARAM, NULL));
            printf("DONE\n");
        }
        else {
            printf("Problem starts here 2\n");
            char *tempName = createQuadExpr(args->expr);
            addQuad(createQuad(createArg(tempName,VAL_LONG,0),NULL,OP_PARAM,NULL));
        }
        args = args->funcargs;
    }
    printf("DONE with funccall\n");
    return tempName;
}

/**
 * creates quad depending on type of statement
*/
void createQuadStatements(struct Statement *stmt) {
    while (stmt) {
        if ((stmt->stmt == ASSIGNMENT) || (stmt->stmt == DECLARATION)) createQuadVar(stmt->var);
        else if (stmt->stmt == RETURN) createQuadReturn(stmt->returnstmt);
        else if (stmt->stmt == IF) createQuadConditional(stmt->condstmt);
        else if ((stmt->stmt == WHILE) || (stmt->stmt == FOR)) createQuadLoop(stmt->loop);
        else if (stmt->stmt == FUNCCALL) createQuadFuncCall(stmt->funccall);
        else if (stmt->stmt == BREAK) createQuadBreak(numQuads);
        else if (stmt->stmt == CONTINUE) createQuadContinue(numQuads);
        stmt = stmt->next;
    }
}

/**
 * creates quad for a function
*/
void createQuadFunc(struct FuncDecl *func) {
    addQuad(createQuad2(NULL,NULL,OP_LABEL,func->symbol));
    struct Params *params = func->params;
    while (params) {
        addQuad(createQuad2(createArg(params->var->name,getTypeQuad(params->var->type),0),NULL,OP_PARAM,params->var->symbol));
        params = params->next;
    }
    struct Statement *stmt = func->statements;
    createQuadStatements(stmt);
    addQuad(createQuad(NULL,NULL,OP_LABEL,"end func"));
}

/**
 * Converts operation enum to a string and prints it
*/
void opToString(enum op op) {
    switch(op) {
        case OP_MULT:
            printf("MULT");
            break;
        case OP_DIV:
            printf("DIV");
            break;
        case OP_ADD:
            printf("ADD");
            break;
        case OP_SUB:
            printf("SUB");
            break;
        case OP_EQ:
            printf("EQ");
            break;
        case OP_LEQ:
            printf("LEQ");
            break;
        case OP_GEQ:
            printf("GEQ");
            break;
        case OP_NEQ:
            printf("NEQ");
            break;
        case OP_LESS:
            printf("LESS");
            break;
        case OP_GREAT:
            printf("GREAT");
            break;
        case OP_OR:
            printf("OR");
            break;
        case OP_AND:
            printf("AND");
            break;
        case OP_ASSIGN:
            printf("ASSIGN");
            break;
        case OP_BITOR:
            printf("BITOR");
            break;
        case OP_BITAND:
            printf("BITAND");
            break;
        case OP_BITXOR:
            printf("BITXOR");
            break;
        case OP_CALL:
            printf("CALL");
            break;
        case OP_JUMPIFNOT:
            printf("JUMPIFNOT");
            break;
        case OP_JUMPIF:
            printf("JUMPIF");
            break;
        case OP_JUMP:
            printf("JUMP");
            break;
        case OP_LABEL:
            printf("LABEL");
            break;
        case OP_RET:
            printf("RET");
            break;
        case OP_PARAM:
            printf("PARAM");
            break;
        case OP_REF:
            printf("REF");
            break;
        case OP_DEREF:
            printf("DEREF");
            break;
        case OP_ARRAY_CREATE:
            printf("ARRAY CREATE");
            break;
        case OP_ARRAY_INDEX:
            printf("ARRAY INDEX");
            break;
        default:
            printf("OP NOT FOUND");
    }
}

/**
 * Prints value of value union
*/
void printValue(enum val_t val, union value value) {
    switch (val) {
        case VAL_CHAR:
            printf("%d",(char)value.char_value);
            break;
        case VAL_UCHAR:
            printf("%u",value.uchar_value);
            break;
        case VAL_SHORT:
            printf("%d",value.short_value);
            break;
        case VAL_USHORT:
            printf("%u",value.ushort_value);
            break;
        case VAL_INT:
            printf("%d",value.int_value);
            break;
        case VAL_UINT:
            printf("%u",value.uint_value);
            break;
        case VAL_LONG:
            printf("%ld",value.long_value);
            break;
        case VAL_ULONG:
            printf("%lu",value.ulong_value);
            break;
        case VAL_DOUBLE:
            printf("%f",value.double_value);
            break;
        case VAL_POINTER:
            printf("POINTER");
            break;
        case VAL_ARR_CHAR:
            printf("ARR_CHAR");
            break;
        case VAL_ARR_SHORT:
            printf("ARR_SHORT");
            break;
        case VAL_ARR_INT:
            printf("ARR_INT");
            break;
        case VAL_ARR_LONG:
            printf("ARR_LONG");
            break;
        case VAL_ARR_UCHAR:
            printf("ARR_UCHAR");
            break;
        case VAL_ARR_USHORT:
            printf("ARR_USHORT");
            break;
        case VAL_ARR_UINT:
            printf("ARR_UINT");
            break;
        case VAL_ARR_ULONG:
            printf("ARR_ULONG");
            break;
        case VAL_ARR_DOUBLE:
            printf("ARR_DOUBLE");
            break;
        case VAL_ARR_POINTER:
            printf("ARR_POINTER");
            break;
        default: 
            printf("VALUE NOT FOUND");
    }
}

/**
 * Print argument struct
*/
void printArgument(struct argument *arg) {
    if (!arg) printf("_");
    else if (arg->name) printf("%s",arg->name);
    else printValue(arg->val_t,arg->value);
}

/**
 * prints a single quad
*/
void printQuad(struct quad *quad) {
    printf("Op: ");
    opToString(quad->operation);
    printf(" | Arg1: ");
    printArgument(quad->arg1);
    printf(" | Arg2: ");
    printArgument(quad->arg2);
    printf(" | Result: %s\n",quad->result);
}

/**
 * Prints quads
*/
void printQuads() {
    printf("----------QUADS----------\n");
    for (int i = 0; i < numQuads; i++) printQuad(quads[i]);
}

/**
 * Creates the full IR
*/
void createIR() {
    struct Declaration *d = syntaxTree;
    while (d) {
        if (d->dec == VAR) createQuadVar(d->vardecl);
        else createQuadFunc(d->funcdecl);
        d = d->next;
    }
}

/**
 * Retrieves a value from the union based on val_t
*/
long getValue(val_t val, union value value) {
    switch(val) {
        case VAL_CHAR:
            return value.char_value;
        case VAL_UCHAR:
            return value.uchar_value;
        case VAL_SHORT:
            return value.short_value;
        case VAL_USHORT:
            return value.ushort_value;
        case VAL_INT:
            return value.int_value;
        case VAL_UINT:
            return value.uint_value;
        case VAL_LONG:
            return value.long_value;
        case VAL_ULONG:
            return value.ulong_value;
        case VAL_DOUBLE:
            return value.double_value;
        default:
            return 0;
    }
}

/**
 * Performs operation on two constants
*/
long performOperation(struct argument *arg1, struct argument *arg2, enum op op) {
    long value1 = getValue(arg1->val_t,arg1->value);
    long value2 = getValue(arg2->val_t,arg2->value);
    switch(op) {
        case OP_ADD:
            return value2 + value1;
        case OP_SUB:
            return value2 - value1;
        case OP_MULT:
            return value2 * value1;
        case OP_DIV:
            return value2 / value1;
        case OP_MOD:
            return value2 % value1;
        case OP_EQ:
            return value2 == value1;
        case OP_GEQ:
            return value2 >= value1;
        case OP_LEQ:
            return value2 <= value1;
        case OP_LESS:
            return value2 < value1;
        case OP_GREAT: 
            return value2 > value1;
        case OP_AND:
            return value2 && value1;
        case OP_OR:
            return value2 || value1;
        case OP_NEQ:
            return value2 != value1;
        case OP_BITAND:
            return value2 & value1;
        case OP_BITOR:
            return value2 | value1;
        case OP_BITXOR:
            return value2 ^ value1;
        default:
        return 0;
    }
    return 0;
}

/**
 * Removes a quad from the quad array
*/
void removeQuad(int index) {
    for (int i = index; i < numQuads - 1; i++) {
        quads[i] = quads[i + 1];
    }
    numQuads--;
    quads = realloc(quads,sizeof(struct quad) * numQuads);
}

/**
 * Reduces adjacent constants in an expression
*/
void constantFolding() {
    for (int i = 0; i < numQuads - 1; i++) {
        if (!quads[i]->arg1 || !quads[i]->arg2) continue;
        if (quads[i]->operation == OP_ARRAY_CREATE) continue;
        char *name1 = quads[i]->arg1->name;
        char *name2 = quads[i]->arg2->name;
        if (!name1 && !name2) {
            char *oldResult = quads[i]->result;
            long foldedVal = performOperation(quads[i]->arg1,quads[i]->arg2,quads[i]->operation);
            struct argument *newArg = createArg(NULL,quads[i]->arg1->val_t,foldedVal);
            for (int j = i + 1; j < numQuads - 1; j++) {
                if (quads[j]->arg1 && quads[j]->arg1->name && strcmp(quads[j]->arg1->name,oldResult) == 0) {
                    quads[j]->arg1 = newArg;
                }
                if (quads[j]->arg2 && quads[j]->arg2->name && strcmp(quads[j]->arg2->name,oldResult) == 0) {
                    quads[j]->arg2 = newArg;
                }
            }
            removeQuad(i);
            i--;
        }
    }
}

/**
 * Optimizes the code in the intermediate representation
*/
void optimize() {
    constantFolding();
}