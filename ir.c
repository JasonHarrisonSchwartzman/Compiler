#include "syntaxtree.h"

extern struct Declaration *syntaxTree;

typedef enum {
    DAG_PROGRAM, //init

    DAG_FUNCTIONDEC,

    DAG_RETURN,
    DAG_ASSIGN,
    DAG_DEREF,
    DAG_REF,
    DAG_ARRAYINDEX,
    DAG_FUNCTIONCALL,

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
    DAG_UDOUBLE_VAL,
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
    union payload payload;
};

struct dag_node *dag;

struct dag_node *createNode1(dag_kind_t kind, struct dag_node *left, struct dag_node *right) {
    struct dag_node *d = malloc(sizeof(struct dag_node));
    d->kind = kind;
    d->left = left;
    d->right = right;
    return d;
}

struct dag_node *createNode2(dag_kind_t kind, struct dag_node *left, struct dag_node *right, union payload payload) {
    struct dag_node *d = malloc(sizeof(struct dag_node));
    d->kind = kind;
    d->left = left;
    d->right = right;
    d->payload = payload;
    return d;
}

struct dag_node *createDAGexpression(struct Expression *expr, struct dag_node *dag) {

}

struct dag_node *createDAGvar(struct VarDecl *var, struct dag_node *dag) {
    struct dag_node *d = createNode1(DAG_ASSIGN,NULL,NULL);
    d->left = createNode2(DAG_NAME,NULL,NULL,(union payload){.name = var->name});
    d->right = createDAGexpression(var->expr,d);
    return d;
}

/**
 * Creates a DAG for intermediate representation given the syntax tree
*/
void createDAG() {
    struct Declaration *d = syntaxTree;
    while (d) {

        if (d->dec == VAR) {
            createDAGvar(d->vardecl, dag);
        }
        else {

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