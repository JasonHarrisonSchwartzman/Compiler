#include <stdio.h>
#include <stdlib.h>
#include "syntaxtree.h"
#include "ir.h"

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

char **code;
int numCodeLines = 0;

void addCode(char *str) {
    code = realloc(code, sizeof(char*) * (numLines + 1));
    code[numLines++] = str;
}

void printCode() {
    for (int i = 0; i < numLines; i++) {
        printf("%s\n",code[i]);
    }
}


struct scratch_register registers[] = {
    {"%%rbx",0},
    {"%%r10",0},
    {"%%r11",0},
    {"%%r12",0},
    {"%%r13",0},
    {"%%r14",0},
    {"%%r15",0},
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

/**
 * Table of Symbols and their address
*/
struct SymbolAddress {
    struct Symbol *symbol;
    char *address;
    int size;
    int placeOnStack;
} SymbolAddress;

struct SymbolAddress **symAdds;
int numSymbols = 0;

/**
 * Adds symbol address to SymbolAddress table
*/
void addSymbolAddress(struct Symbol *symbol, char *address, int size, int placeOnStack){ 
    struct SymbolAddress *symAdd = calloc(1,sizeof(struct SymbolAddress));
    symAdd->symbol = symbol;
    symAdd->address = address;
    symAdd->size = size;
    symAdd->placeOnStack = placeOnStack;
    symAdds = realloc(symAdds,sizeof(struct SymbolAddress) * (numSymbols + 1));
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
 * Converts the number to a string for address name on stack
*/
char *createAddressName(int num) {
    int totalLength = snprintf(NULL, 0, "-%d(%%rbp)", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, "-%d(%%rbp)", num);
    }
    return result;
}

/**
 * Calculates size of a given local variable 
 * Either 8 * size (IF ARRAY)
 * otherwise 8
*/
int calculateSize(struct quad *quad) {
    if (quad->operation == OP_ARRAY_CREATE) {
        return (int)getValue(quad->arg1->val_t,quad->arg1->value) * 8;
    }
    else return 8;
}

/**
 * Computes the address where the local variable/param will be stored on the stack
*/
char *addressCompute(struct quad *quad, struct Symbol *s) {
    char *address = lookUpAddress(s);
    if (address) {
        return address;
    }
    int size = calculateSize(quad);
    if (numSymbols == 0) {
        addSymbolAddress(s,createAddressName(8),size,8);
    }
    else {
        int placeOnStack = symAdds[numSymbols-1]->placeOnStack + symAdds[numSymbols-1]->size;
        addSymbolAddress(s,createAddressName(placeOnStack),size,placeOnStack);
    }
    return NULL;
}

/**
 * Returns the address computation for a given symbol 
*/
const char *symbol_codegen(struct quad *quad, struct Symbol *s) {
    if (s->sym == SYMBOL_GLOBAL) return s->name;
    else {//LOCAL VARIABLES/PAREMETERS
        return addressCompute(quad,s);
    }
    return NULL;
}

/**
 * Prints symbol address table
*/
void printSymbolAddress() {
    for (int i = 0; i < numSymbols; i++) {
        printf("Name: %s | Location: %d\n",symAdds[i]->symbol->name,symAdds[i]->placeOnStack);
    }
}

/**
 * Generates code given IR
*/
void generateCode() {
    for (int i = 0; i < numQuads; i++) {
        if (quads[i]->symbol) {//symbol address (local variable/parameter)
            symbol_codegen(quads[i],quads[i]->symbol);
        }
        else if (quads[i]->operation == OP_LABEL) {
            if (quads[i]->symbol) {//new function
                printSymbolAddress();
                symAdds = realloc(symAdds, 0);
                numSymbols = 0;
            }
            else {
                //quads[i]->result;
            }
        }
        else if (quads[i]->operation == OP_CALL) {
            char *call = "CALL ";
            strcat(call,quads[i]->arg1->name);
            addCode(call);
        }
    }
    printSymbolAddress();
}