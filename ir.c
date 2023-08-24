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
void addQuad(struct quad *quad) {
    quads = realloc(quads,sizeof(struct quad) * (1 + numQuads));
    quads[numQuads] = quad;
    numQuads++;
    printf("ADDED QUAD WITH RESULT: %s\n", quad->result);
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
 * a = b + c + d + e;
 * 
 * t1 = b + c;
 * t2 = t1 + d;
 * t3 = t2 + e;
 * a = t3
*/

/**
 * Creates an argument struct given an eval
*/
struct argument *evalToArg(struct Evaluation *eval) {
    if (eval->eval == ID) {
        return createArg(eval->name,getTypeQuad(eval->type),0);
    }
    if (eval->eval == VALUE) {
        return createArg(NULL,getTypeQuad(eval->type),strtol(eval->value->value,NULL,10));
    }
    return NULL;
}

/**
 * Creates a quad for a variable declaration
*/
void createQuadVar(struct VarDecl *var) {
    struct Expression *e = var->expr;
    if (!e->expr) {
        addQuad(createQuad(evalToArg(e->eval), NULL, OP_ASSIGN, var->name));
    }
    while (e) {
        e = e->expr;
    }
}

void createQuadFunc(struct FuncDecl *func) {

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