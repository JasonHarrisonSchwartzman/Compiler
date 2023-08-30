#include "syntaxtree.h"

typedef enum op {
    OP_MULT,
    OP_DIV,
    OP_ADD,
    OP_SUB,
    OP_EQ,
    OP_LEQ,
    OP_GEQ,
    OP_NEQ,
    OP_LESS,
    OP_GREAT,
    OP_OR,
    OP_AND,
    OP_ASSIGN,
    OP_BITOR,
    OP_BITAND,
    OP_BITXOR,
    OP_CALL,
    OP_JUMP,
    OP_JUMPIF,
    OP_JUMPIFNOT,
    OP_MOD,
    OP_RET,
    OP_LABEL,
    OP_PARAM,
} op;

typedef enum val_t {
    VAL_CHAR,
    VAL_UCHAR,
    VAL_SHORT,
    VAL_USHORT,
    VAL_INT,
    VAL_UINT,
    VAL_LONG,
    VAL_ULONG,
    VAL_DOUBLE,
    VAL_POINTER,
} val_t;

union value {
    const char *name;
    double double_value;
    int int_value;
    short short_value;
    char char_value;
    long long_value;
    unsigned int uint_value;
    unsigned short ushort_value;
    unsigned char uchar_value;
    unsigned long ulong_value;
};

struct argument {
    char *name;
    enum val_t val_t;
    union value value;
};

struct quad {
    struct argument *arg1;
    struct argument *arg2;
    enum op operation;
    char *result;
} quad;

struct quad **quads;
int numQuads = 0;

int temp = 0;
int label = 0;

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
    int totalLength = snprintf(NULL, 0, "l%d", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, "l%d", num);
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

/**
 * Adds quad to quad array
*/
char *addQuad(struct quad *quad) {
    quads = realloc(quads,sizeof(struct quad) * (1 + numQuads));
    quads[numQuads] = quad;
    numQuads++;
    printf("ADDED QUAD WITH RESULT: %s\n", quad->result);
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
                printf("VAL NOT FOUND\n");
        }
    }
    return a;
}

/**
 * Converts a type struct into a val_t enum
*/
val_t getTypeQuad(struct Type *type) {
    if (!type) printf("TYPE NULL WHEN GETTING TYPE ERROR\n");
    if (type->pointer > 0) return VAL_POINTER;
    switch (type->dataType) {
        case CHAR:
            return type->sign == SIGNED ? VAL_CHAR : VAL_UCHAR;
        case SHORT:
            return type->sign == SIGNED ? VAL_SHORT : VAL_USHORT;
        case INT:
            return type->sign == SIGNED ? VAL_INT : VAL_UINT;
        case LONG:
            return type->sign == SIGNED ? VAL_LONG : VAL_ULONG;
        case DOUBLE:
            return VAL_DOUBLE; //no such thing as unsigned double
    }
    return -1;
}


/**
 * Creates an argument struct given an eval
*/
struct argument *evalToArg(struct Evaluation *eval) {
    if (eval->name) printf("name %s:\n", eval->name);
    if (eval->value) printf("value %s:\n",eval->value->value);
    if (!eval->type) printf("error no type\n");
    if (eval->eval == ID) {
        printf("1\n");
        return createArg(eval->name,getTypeQuad(eval->type),0);
    }
    if (eval->eval == VALUE) {
        printf("2\n");
        return createArg(NULL,getTypeQuad(eval->type),strtol(eval->value->value,NULL,10));
    }
    return NULL;
}
/**
 * converts op from syntax tree to op in IR
*/
op getQuadOp(operation_t op) {
    switch(op) {
        case PLUS:
            return OP_ADD;
        case MINUS:
            return OP_SUB;
        case MULT:
            return OP_MULT;
        case DIV:
            return OP_DIV;
        case MOD:
            return OP_MOD;
        case BITWISEAND:
            return OP_BITAND;
        case BITWISEOR:
            return OP_BITOR;
        case BITWISEXOR:
            return OP_BITXOR;
        case EQUAL:
            return OP_EQ;
        case GREATEQUAL:
            return OP_GEQ;
        case LESSEQUAL:
            return OP_LEQ;
        case LESS:
            return OP_LESS;
        case GREAT:
            return OP_GREAT;
        case AND:
            return OP_AND;
        case NOT:
            return OP_NEQ;
        case OR:
            return OP_OR;
        default:
            return -1;
    }
}

/**
 * generic Quad creator to create long expression 
 * single or null expressions are dealt with on higher-up functions
*/
char *createQuadExpr(struct Expression *expr) {
    //a b c d
    //a b -> t1
    //t1 c -> t2
    //t2 d -> t3
    struct Expression *e = expr;
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
    if (!e->expr) {
        addQuad(createQuad(evalToArg(e->eval), NULL, OP_ASSIGN, var->name));
        return;
    }
    char *tempName = createQuadExpr(var->expr);
    addQuad(createQuad(createArg(tempName,getTypeQuad(var->type),0), NULL, OP_ASSIGN, var->name));
}

/**
 * creates a quad for a return statement
*/
void createQuadReturn(struct Expression *expr) {
    if (!expr) {
        addQuad(createQuad(NULL,NULL,OP_RET,NULL));
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
void createQuadFuncCall(struct FunctionCall *call) {
    struct FunctionArgs *count = call->funcargs;
    unsigned long num = 0;
    while (count) {
        num++;
        count = count->funcargs;
    }
    addQuad(createQuad(createArg(call->name,-1,0),createArg(NULL,VAL_ULONG,num),OP_CALL,NULL));
    struct FunctionArgs *args = call->funcargs;
    while (args) {
        char *tempName = createQuadExpr(args->expr);
        addQuad(createQuad(NULL,NULL,OP_PARAM,tempName));
        args = args->funcargs;
    }
}

/**
 * creates quad depending on type of statement
*/
void createQuadStatements(struct Statement *stmt) {
    while (stmt) {
        if ((stmt->stmt == ASSIGNMENT) || (stmt->stmt == DECLARATION)) {
            createQuadVar(stmt->var);
        }
        else if (stmt->stmt == RETURN) {
            createQuadReturn(stmt->returnstmt);
        }
        else if (stmt->stmt == IF) { // IF ELSE IF ELSE
            createQuadConditional(stmt->condstmt);
        }
        else if ((stmt->stmt == WHILE) || (stmt->stmt == FOR)) {
            printf("LOOP\n");
            createQuadLoop(stmt->loop);
        }
        else if (stmt->stmt == FUNCCALL) {
            createQuadFuncCall(stmt->funccall);
        }
        else if (stmt->stmt == BREAK) {
            createQuadBreak(numQuads);
        }
        else if (stmt->stmt == CONTINUE) {
            createQuadContinue(numQuads);
        }
        stmt = stmt->next;
    }
}

/**
 * creates quad for a function
*/
void createQuadFunc(struct FuncDecl *func) {
    addQuad(createQuad(NULL,NULL,OP_LABEL,func->name));
    struct Params *params = func->params;
    while (params) {
        addQuad(createQuad(createArg(params->var->name,getTypeQuad(params->var->type),0),NULL,OP_PARAM,NULL));
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
 * Prints quads
*/
void printQuads() {
    printf("----------QUADS----------\n");
    for (int i = 0; i < numQuads; i++) {
        printf("Op: ");
        opToString(quads[i]->operation);
        printf(" | Arg1: ");
        printArgument(quads[i]->arg1);
        printf(" | Arg2: ");
        printArgument(quads[i]->arg2);
        printf(" | Result: %s\n",quads[i]->result);
    }
}

/**
 * Creates the full IR
*/
void createIR() {
    struct Declaration *d = syntaxTree;
    while (d) {

        if (d->dec == VAR) {
            createQuadVar(d->vardecl);
        }
        else {
            createQuadFunc(d->funcdecl);
        }


        d = d->next;
    }
}
long performOperation(struct argument *arg1, struct argument *arg2, enum op op) {
    /*switch(op) {

    }*/
    return 0;
}

void reduceConstants() {
    for (int i = 0; i < numQuads; i++) {
        
    }
}

void optimize() {
    reduceConstants();
}