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
    OP_JUMPIF,
    OP_MOD,
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
    if (type->pointer) return VAL_POINTER;
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
    if (eval->eval == ID) {
        return createArg(eval->name,getTypeQuad(eval->type),0);
    }
    if (eval->eval == VALUE) {
        return createArg(NULL,getTypeQuad(eval->type),strtol(eval->value->value,NULL,10));
    }
    return NULL;
}

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
    //a b c d
    //a b -> t1
    //t1 c -> t2
    //t2 d -> t3
    char *tempName = addQuad(createQuad(evalToArg(e->eval),evalToArg(e->expr->eval),getQuadOp(*e->op),createName("t",temp)));
    e = e->expr->expr;
    while (e) {
        tempName = addQuad(createQuad(createArg(tempName,-1,0),evalToArg(e->eval),getQuadOp(*e->op),createName("t",temp)));
        e = e->expr;
    }
    addQuad(createQuad(createArg(tempName,getTypeQuad(var->type),0), NULL, OP_ASSIGN, var->name));
}

void createQuadFunc(struct FuncDecl *func) {
    struct Statement *stmt = func->statements;
    while (stmt) {
        if (stmt->stmt == ASSIGNMENT || stmt->stmt == DECLARATION) {
            createQuadVar(stmt->var);
        }
    }
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
        case OP_JUMPIF:
            printf("JUMPIF");
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
 * Prints quads
*/
void printQuads() {
    printf("----------QUADS----------\n");
    for (int i = 0; i < numQuads; i++) {
        printf("Op: ");
        opToString(quads[i]->operation);
        printf(" | Arg1: ");
        if (!quads[i]->arg1) {
            printf("_");
        }
        else if (!quads[i]->arg1->name) {
            //printf("that fails\n");
            printValue(quads[i]->arg1->val_t,quads[i]->arg1->value);
        }
        else {
            //printf("this fails\n");
            printf("%p",quads[i]->arg1->name);
        }
        printf(" | Arg2: ");
        if (!quads[i]->arg2) {
            printf("_");
        }
        else if (!quads[i]->arg2->name) {
            printValue(quads[i]->arg2->val_t,quads[i]->arg2->value);
        }
        else {
            printf("%s",quads[i]->arg2->name);
        }
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