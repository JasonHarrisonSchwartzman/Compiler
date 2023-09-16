#ifndef QUAD_H
#define QUAD_H

//operations
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
    OP_MOD,
    OP_BITOR,
    OP_BITAND,
    OP_BITXOR,
    OP_REF,
    OP_DEREF,
    OP_ARRAY_INDEX,
    OP_CALL,
    OP_ASSIGN,
    OP_JUMP,
    OP_JUMPIF,
    OP_JUMPIFNOT,
    OP_RET,
    OP_LABEL,
    OP_PARAM,
    OP_ARRAY_CREATE,
} op;

//value types
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
    VAL_ARR_CHAR,
    VAL_ARR_UCHAR,
    VAL_ARR_SHORT,
    VAL_ARR_USHORT,
    VAL_ARR_INT,
    VAL_ARR_UINT,
    VAL_ARR_LONG,
    VAL_ARR_ULONG,
    VAL_ARR_DOUBLE,
    VAL_ARR_POINTER,
} val_t;

//value of the argument 
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

//argument for the quadruple contains information about the name or value 
struct argument {
    char *name;
    enum val_t val_t;
    union value value;
};

//the quadruple
struct quad {
    struct argument *arg1;
    struct argument *arg2;
    enum op operation;
    char *result;

    int reg;//for code generation
    struct Symbol *symbol;
    int numQuad;
} quad;

long getValue();

#endif
