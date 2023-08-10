#include "syntaxtree.h"



extern struct Declaration *syntaxTree;

typedef enum {
    DAG_PROGRAM, //init

    DAG_RETURN,
    DAG_ASSIGN,
    DAG_DEREF,
    DAG_REF,
    DAG_ARRAYINDEX,

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
struct dag_node {
    dag_kind_t kind;
    struct dag_node *left;
    struct dag_node *right;
    union {
        const char *name;
        double float_value;
        int integer_value;
    } u;
};

void createDAG() {
    
}


/*
    FUNCDEC
x


*/