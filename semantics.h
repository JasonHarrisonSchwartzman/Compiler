#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "syntaxtree.h"

typedef enum symbol_t {
	SYMBOL_GLOBAL,
	SYMBOL_LOCAL,
	SYMBOL_PARAM
} symbol_t;

struct Symbol {
	char *name;
	struct Type *type;
	symbol_t sym;
	dec_t dec;
	struct Symbol *next;
} Symbol;

#endif
