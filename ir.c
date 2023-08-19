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
enum op {
    MULT,
    DIV,
    ADD,
    SUB,
    EQ,
    LEQ,
    GEQ,
    NEQ,
    LESS,
    GREAT,
    OR,
    AND,
    ASSIGN,
    BITOR,
    BITAND,
    BITXOR,
    CALL,
    JUMPIF,
} op;

struct argument {
    char *name;
    union value value;
};

struct result {
    char *name;

};
struct quad {
    struct argument *arg1;
    struct argument *arg2;
    enum op operation;
    struct result *result;
} quad;


void createIR() {

}