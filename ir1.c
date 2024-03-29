#include "syntaxtree.h"

/**
 * NOTE: This file is no longer in use but contains some of the ideas that were considered when making 
 * the intermediate language which was initially going to be a DAG. Later, I decided that the IR should be
 * quadruples as it is closer to the targeted assembly language.
*/
extern struct Declaration *syntaxTree;

//all types of nodes within the DAG 
typedef enum {
    DAG_PROGRAM, //init

    DAG_FUNCTIONDEC,

    DAG_RETURN,
    DAG_ASSIGN,
    DAG_DEREF,
    DAG_REF,
    DAG_ARRAYINDEX,
    DAG_FUNCTIONCALL,
    DAG_POINTER,

    DAG_BREAK,
    DAG_CONTINUE,
    DAG_IF,
    DAG_ELSE,
    DAG_FOR,
    DAG_WHILE,
    

    DAG_ADD,
    DAG_MULT,
    DAG_DIV,
    DAG_MINUS,
    DAG_MOD,
    DAG_AND,
    DAG_OR,
    DAG_NOTEQUAL,
    DAG_BITAND,
    DAG_BITOR,
    DAG_BITXOR,
    DAG_EQUAL,
    DAG_LESSEQUAL,
    DAG_LESS,
    DAG_GREAT,
    DAG_GREATEQUAL,

    DAG_NAME,

    DAG_DOUBLE_VAL,
    DAG_INT_VAL,
    DAG_SHORT_VAL,
    DAG_LONG_VAL,
    DAG_CHAR_VAL,
    //DAG_UDOUBLE_VAL, no such thing as unsigned double in c who knew?
    DAG_UINT_VAL,
    DAG_USHORT_VAL,
    DAG_ULONG_VAL,
    DAG_UCHAR_VAL,
} dag_kind_t;
//DAG constructed by post-order traversal of the AST
//search nodes to re-use

union payload {
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
struct dag_node {
    dag_kind_t kind;
    struct dag_node *left;
    struct dag_node *right;
    struct dag_node *next; //for control flow
    union payload payload;
};

struct dag_node **dag; // DAG of program
int numDAG = 0; //number of DAGs (either global variables or functions)

/**
 * Creates a DAG node without a payload
*/
struct dag_node *createNode1(dag_kind_t kind, struct dag_node *left, struct dag_node *right) {
    struct dag_node *d = malloc(sizeof(struct dag_node));
    d->kind = kind;
    d->left = left;
    d->right = right;
    return d;
}

/**
 * Creates DAG node with a payload
*/
struct dag_node *createNode2(dag_kind_t kind, struct dag_node *left, struct dag_node *right, union payload payload) {
    printf("test\n");
    struct dag_node *d = malloc(sizeof(struct dag_node));
    d->kind = kind;
    d->left = left;
    d->right = right;
    d->payload = payload;
    return d;
}

/**
 * Converts a type struct into a DAG_KIND_T type
*/
dag_kind_t getTypeDAG(struct Type *type) {
    if (!type) printf("TYPE NULL WHEN GETTING TYPE ERROR\n");
    if (type->pointer) return DAG_POINTER;
    switch (type->dataType) {
        case CHAR:
            return type->sign == SIGNED ? DAG_CHAR_VAL : DAG_UCHAR_VAL;
        case SHORT:
            return type->sign == SIGNED ? DAG_SHORT_VAL : DAG_USHORT_VAL;
        case INT:
            return type->sign == SIGNED ? DAG_INT_VAL : DAG_UINT_VAL;
        case LONG:
            return type->sign == SIGNED ? DAG_LONG_VAL : DAG_ULONG_VAL;
        case DOUBLE:
            return DAG_DOUBLE_VAL; //no such thing as unsigned double
    }
    return -1;
}

/**
 * Converts the string storing the value to the actual value and returns a union payload
*/
union payload getPayload(dag_kind_t dagKind, struct Evaluation *eval) {
    char *endptr;
    switch(dagKind) {
        case DAG_CHAR_VAL:
            return (union payload){.char_value = *(eval->value->value)};
        case DAG_UCHAR_VAL:
            return (union payload){.uchar_value = *(eval->value->value)};
        case DAG_SHORT_VAL:
            return (union payload){.short_value = (short)strtol(eval->value->value,&endptr,10)};
        case DAG_USHORT_VAL:
            return (union payload){.ushort_value = (unsigned short)strtoul(eval->value->value,&endptr,10)};
        case DAG_INT_VAL:
            return (union payload){.int_value = (int)strtol(eval->value->value,&endptr,10)};
        case DAG_UINT_VAL:
            return (union payload){.uint_value = (unsigned int)strtoul(eval->value->value,&endptr,10)};
        case DAG_LONG_VAL:
            return (union payload){.long_value = (long)strtol(eval->value->value,&endptr,10)};
        case DAG_ULONG_VAL:
            return (union payload){.ulong_value = (unsigned long)strtoul(eval->value->value,&endptr,10)};
        case DAG_DOUBLE_VAL:
            return (union payload){.double_value = (double)strtod(eval->value->value, &endptr)};
        default:
            break;
    }
    printf("oh no\n");
    return (union payload){.name = NULL};
}

/**
 * Creates a DAG node representing an expression
*/
struct dag_node *createDAGexpression(struct Expression *expr, struct dag_node *dag) {
    if (!expr->expr) {
        struct dag_node *d;
        switch(expr->eval->eval) {
            case VALUE:
                printf("try\n");
                printf("Value: %s\n",expr->eval->value->value);
                //expr->eval->value->value
                dag_kind_t dagKind = getTypeDAG(expr->eval->type);
                printf("dag kind\n");
                d = createNode2(dagKind, NULL, NULL, getPayload(dagKind,expr->eval));
                printf("node for value created\n");
                break;
            case DEREF:
                d = NULL;
                break;
            case REF:
                d = NULL;
                break;
            case ID:
                d = NULL;
                break;
            case ARRAYINDEX:
                d = NULL;
                break;
            case FUNCRETURN:
                d = NULL;
                break;
        }
        return d;
    }
    return NULL;
}

/**
 * Creates a DAG node representing a variable assignment
*/
struct dag_node *createDAGvar(struct VarDecl *var, struct dag_node *dag) {
    struct dag_node *d = createNode1(DAG_ASSIGN,NULL,NULL);
    printf("node created\n");
    d->left = createNode2(DAG_NAME,NULL,NULL,(union payload){.name = var->name});
    printf("node added\n");
    d->right = createDAGexpression(var->expr,d);
    printf("done\n");
    return d;
}

struct dag_node *createDAGparams(struct Params *params, struct dag_node *dag) {
    struct Params *p = params;
    while (params->var) {
        
        p = p->next;
    }
}

/**
 * Creates a DAG node representing a function declaration
*/
struct dag_node *createDAGfunc(struct FuncDecl *func, struct dag_node *dag) {
    struct dag_node *d = createNode1(DAG_FUNCTIONDEC,NULL,NULL);
    d->left = createNode2(DAG_NAME,NULL,NULL,(union payload){.name = func->name});
    d->left->left = createNode1(getTypeDAG(func->type),NULL,NULL); //return type

    return NULL;
}

/**
 * Allocates memeory for another DAG
*/
void addDagNode() {
    numDAG++;
    dag = realloc(dag,numDAG * sizeof(struct dag_node));
}

/**
 * Creates a DAG for intermediate representation given the syntax tree
*/
void createDAG() {
    printf("test\n");
    struct Declaration *d = syntaxTree;
    while (d) {

        if (d->dec == VAR) {
            printf("hi mom\n");
            addDagNode();
            dag[numDAG-1] = createDAGvar(d->vardecl, dag[numDAG-1]);
        }
        else {
            addDagNode();
            dag[numDAG-1] = createDAGfunc(d->funcdecl,dag[numDAG-1]);
        }


        d = d->next;
    }
}

void printDAG() {

}


/*
    FUNCDEC
x


*/