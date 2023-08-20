enum op {
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

enum val_t {
    VAL_CHAR,
    VAL_UCHAR,
    VAL_SHORT,
    VAL_USHORT,
    VAL_INT,
    VAL_UINT,
    VAL_LONG,
    VAL_ULONG,
};

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