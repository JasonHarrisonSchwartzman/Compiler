#include <stdio.h>
#include <stdlib.h>
#include "ir.c"
#include "syntaxtree.h"

/**
 * This file converts the Intermediate Representation into assembly language.
*/

//from ir.c
extern struct quad **quads;
extern int numQuads;

int labelName = 0;

struct scratch_register {
    char *name;
    int inUse;
} scratch_register;


struct scratch_register registers[] = {
    {"%rbx",0},
    {"%r10",0},
    {"%r11",0},
    {"%r12",0},
    {"%r13",0},
    {"%r14",0},
    {"%r15",0},
};
int numRegisters = 6;

/**
 * Finds unused register and makes it in use
*/
int scratch_alloc() {
    for (int i = 0; i < numRegisters; i++) {
        if (!registers[i].inUse) {
            registers[i].inUse = 1;
            return i;
        }
    }
    return -1;
}

/**
 * Marks register as free
*/
void scratch_free(int reg) {
    registers[reg].inUse = 0;
}

/**
 * Returns names of register
*/
const char *scratch_name(int reg) {
    return registers[reg].name;
}

/**
 * Increments label and returns current value
*/
int label_create() {
    return labelName++;
}

/**
 * Returns lable name .LX where X is the label number
*/
const char *label_name(int label) {
    int totalLength = snprintf(NULL, 0, ".L%d", labelName) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, ".L%d", labelName);
    }
    return result;
}



struct SymbolAddress {
    struct Symbol *symbol;
    char *address;
    unsigned long sequenceNum;
} SymbolAddress;

struct SymbolAddress **symAdds;
int numSymbols = 0;

/**
 * Adds symbol address to SymbolAddress table
*/
void addSymbolAddress(struct Symbol *symbol, char *address){ 
    struct SymbolAddress *symAdd = calloc(1,sizeof(struct SymbolAddress));
    symAdd->symbol = symbol;
    symAdd->address = address;
    symAdds = realloc(1,sizeof(struct SymbolAddress) * (numSymbols + 1));
    symAdds[numSymbols++] = symAdd;
}

/**
 * Looks up address given a symbol 
 * returns address is symbol exsists or return null
*/
char *lookUpAddress(struct Symbol *symbol) {
    for (int i = 0; i < numSymbols; i++) {
        if (symAdds[i]->symbol == symbol) return symAdds[i]->address;
    }
    return NULL;
}

/**
 * Computes the address where the local variable/param will be stored on the stack
*/
char *addressCompute(struct Symbol *s) {
    for (int i = 0; i < numQuads; i++) {
        if (quads[i]->symbol == s) {
            char *address = lookUpAddress(s);
            if (address) {
                return address;
            }
            else {
                
            }
        }
    }
    return NULL;
}

/**
 * Returns the address computation for a given symbol 
*/
const char *symbol_codegen(struct Symbol *s) {
    if (s->sym == SYMBOL_GLOBAL) return s->name;
    else {//LOCAL VARIABLES/PAREMETERS

    }
    return NULL;
}