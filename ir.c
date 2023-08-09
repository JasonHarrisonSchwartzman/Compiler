typedef enum {
    DAG_ASSIGN,
    DAG_DEREF,
    DAG_ADD,
    DAG_MULT,
    DAG_DIV,

    DAG_NAME,
    DAG_DOUBLE_VAL,
    DAG_INT_VAL
} dag_kind_t;
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