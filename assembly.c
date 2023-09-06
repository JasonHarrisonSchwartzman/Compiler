#include <stdio.h>
#include <stdlib.h>
#include "ir.c"
#include "syntaxtree.h"

/**
 * This file converts the Intermediate Representation into assembly language.
*/

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

const char *label_name(int label) {
    int totalLength = snprintf(NULL, 0, ".L%d", labelName) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, ".L%d", labelName);
    }
    return result;
}

const char *symbol_codegen(struct Symbol *s) {
    return NULL;
}