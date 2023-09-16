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
    code = realloc(code, sizeof(char*) * (numCodeLines + 1));
    code[numCodeLines++] = str;
}

void printCode() {
    for (int i = 0; i < numCodeLines; i++) {
        printf("%s\n",code[i]);
    }
}


struct scratch_register registers[] = {
    {"%rax",0},//0
    {"%rbx",0},//1
    {"%rcx",0},//2
    {"%rdx",0},//3
    {"%rsi",0},//4
    {"%rdi",0},//5
    {"%rbp",0},//6
    {"%rsp",0},//7
    {"%r8",0},//8
    {"%r9",0},//9
    {"%r10",0},//10
    {"%r11",0},//11
    {"%r12",0},//12
    {"%r13",0},//13
    {"%r14",0},//14
    {"%r15",0},//15
};
int numRegisters = 16;

/**
 * Finds unused register and makes it in use
*/
int scratch_alloc() {
    for (int i = 0; i < numRegisters; i++) {
        if (i == 6 || i == 7) continue; //don't alloc rsp and rbp
        if (!registers[i].inUse) {
            registers[i].inUse = 1;
            return i;
        }
    }
    printf("ERROR NO REGISTER AVAILABLE\n");
    return -1;
}

/**
 * Marks register as free
*/
void scratch_free(int reg) {
    registers[reg].inUse = 0;
}

int regNameToNum(char *name) {
    for (int i = 0; i < numRegisters; i++) {
        if (strcmp(registers[i].name,name) == 0) return i;
    }
    return -1;
}

/**
 * Returns names of register
*/
char *scratch_name(int reg) {
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
char *label_name(int label) {
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
char *addSymbolAddress(struct Symbol *symbol, char *address, int size, int placeOnStack){ 
    struct SymbolAddress *symAdd = calloc(1,sizeof(struct SymbolAddress));
    symAdd->symbol = symbol;
    symAdd->address = address;
    symAdd->size = size;
    symAdd->placeOnStack = placeOnStack;
    symAdds = realloc(symAdds,sizeof(struct SymbolAddress) * (numSymbols + 1));
    symAdds[numSymbols++] = symAdd;
    return address;
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
        return addSymbolAddress(s,createAddressName(8),size,8);
    }
    else {
        int placeOnStack = symAdds[numSymbols-1]->placeOnStack + symAdds[numSymbols-1]->size;
        return addSymbolAddress(s,createAddressName(placeOnStack),size,placeOnStack);
    }
    return NULL;
}



/**
 * Returns the address computation for a given symbol 
*/
char *symbol_codegen(struct quad *quad, struct Symbol *s) {
    if (s->sym == SYMBOL_GLOBAL) return s->name;
    else {//LOCAL VARIABLES/PAREMETERS
        return addressCompute(quad,s);
    }
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
 * Creates an operand for the given argument 
 * Can be an immediate value $X
 * or a value on a register %rX
*/
char *symbolToOperand(struct quad *quad, struct argument *arg) {
    if (!arg->name) {
        long num = getValue(arg->val_t,arg->value);
        int totalLength = snprintf(NULL, 0, "$%ld", num) + 1;
        char *result = (char *)malloc(totalLength);
        if (result != NULL) {
            snprintf(result, totalLength, "$%ld", num);
        }
        return result;//immediate value
    }
    if (arg->name) {
        for (int i = quad->numQuad; i >= 0; i--) {
            if (quads[i]->result && strcmp(arg->name,quads[i]->result) == 0) {
                if (quads[i]->symbol && quads[i]->symbol->sym == SYMBOL_GLOBAL) return quads[i]->symbol->name;
                return registers[quads[i]->reg].name;//value on reg
            }
            if (quads[i]->symbol && strcmp(arg->name,quads[i]->symbol->name) == 0) {
                return symbol_codegen(quads[i],quads[i]->symbol);//variable
            }
        }
    }
    printf("unable to find symbol of arg name%s\n",arg->name);
    return NULL;
}

char *concatenateStrings(char *str1, char *str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *result = (char *)malloc(len1 + len2 + 1);
    if (result == NULL) return NULL;
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

int move(struct quad *quad, struct argument *arg) {
    char *operand = symbolToOperand(quad,arg);
    char *code1 = concatenateStrings("MOVQ ",operand);
    int reg1 = scratch_alloc();
    code1 = concatenateStrings(code1,", ");
    code1 = concatenateStrings(code1,scratch_name(reg1));
    if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand));
    addCode(code1);
    return reg1;
}



void expr_codegen(struct quad *quad) {
    switch (quad->operation) {
        case OP_ADD:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *add = concatenateStrings("ADDQ ",scratch_name(reg1));
            add = concatenateStrings(add, ", ");
            add = concatenateStrings(add,scratch_name(reg2));
            addCode(add);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_SUB: {
            //OP: SUB | Arg1: 9 | Arg2: x | Result: t0
            //sub eax, ebx  ; Subtract the value in ebx from eax, and store the result in eax
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings("SUBQ ",scratch_name(reg1));
            sub = concatenateStrings(sub, ", ");
            sub = concatenateStrings(sub,scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_MULT: {
        int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings("IMUL ",scratch_name(reg1));
            sub = concatenateStrings(sub, ", ");
            sub = concatenateStrings(sub,scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_DIV:
            break;
        case OP_MOD:
            break;
        case OP_BITAND: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings("AND ",scratch_name(reg1));
            sub = concatenateStrings(sub, ", ");
            sub = concatenateStrings(sub,scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_BITOR: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings("OR ",scratch_name(reg1));
            sub = concatenateStrings(sub, ", ");
            sub = concatenateStrings(sub,scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_BITXOR: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings("SUBQ ",scratch_name(reg1));
            sub = concatenateStrings(sub, ", ");
            sub = concatenateStrings(sub,scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_AND:
            break;
        case OP_OR:
            break;
        case OP_NEQ:
            break;
        case OP_GEQ:
            break;
        case OP_LEQ:
            break;
        case OP_GREAT:
            break;
        case OP_LESS:
            break;
        case OP_EQ:
            break;


        case OP_REF:
            break;
        case OP_DEREF:
            break;
        case OP_ARRAY_INDEX:
            break;
        default:
        return;
    }
}

void addParamsToStack(struct quad *quad) {
    switch (getValue(quad->arg1->val_t,quad->arg1->value)) {
        case 6:
            addCode("MOVQ %r9, -48(%rbp)");
            scratch_free(9);

        case 5:
            addCode("MOVQ %r8, -40(%rbp)");
            scratch_free(8);

        case 4:
            addCode("MOVQ %rcx, -32(%rbp)");
            scratch_free(2);

        case 3:
            addCode("MOVQ %rdx, -24(%rbp)");
            scratch_free(3);

        case 2:
            addCode("MOVQ %rsi, -16(%rbp)");
            scratch_free(4);

        case 1:
            addCode("MOVQ %rdi, -8(%rbp)");
            scratch_free(5);

        case 0:return;
        default:
        return;
    }
}

void printUsedRegisters() {
    for (int i = 0; i < numRegisters; i++) {
        if (registers[i].inUse) {
            printf("%s\n",registers[i].name);
        }
    }
}

/**
 * Generates code given IR
*/
void generateCode() {
    int inFunction = 0;
    for (int i = 0; i < numQuads; i++) {
        quads[i]->numQuad = i;
        if (quads[i]->symbol && quads[i]->operation != OP_LABEL) {//symbol address (local variable/parameter)
            symbol_codegen(quads[i],quads[i]->symbol);
        }
        if (quads[i]->operation == OP_LABEL) {
            if (strcmp(quads[i]->result,"end func") == 0) {
                inFunction = 0;
                //printf("out of function\n");
                continue;
            }
            if (quads[i]->symbol) {//new function
                inFunction = 1;
                //printf("In function\n");
                printSymbolAddress();
                symAdds = realloc(symAdds, 0);
                numSymbols = 0;
                char *func = concatenateStrings(quads[i]->result,":");
                addCode(func);
                addCode("PUSHQ %rbp");
                addCode("MOVQ %rsp, %rbp");

                printf("added func\n");
                addParamsToStack(quads[i]);
                i++;
                while (quads[i]->operation == OP_PARAM) {
                    i++;
                }
                printf("done adding params\n");
                i--;
            }
            else {
                char *label = concatenateStrings(quads[i]->result,":");
                addCode(label);
            }
        }
        else if (quads[i]->operation == OP_ASSIGN) {//make sure to do someting different with globals
            //printf("ASSIGN\n");
            if (inFunction == 0) {
                symbol_codegen(quads[i],quads[i]->symbol);
                printf("GLOBAL VAR %s\n",quads[i]->result);
                continue;
            }
            quads[i]->reg = move(quads[i],quads[i]->arg1);
        }
        else if (quads[i]->operation == OP_CALL) {
            char *call = concatenateStrings("CALL ",quads[i]->arg1->name);
            addCode(call);
        }
        else if (quads[i]->operation == OP_SUB || quads[i]->operation == OP_ADD) {
            expr_codegen(quads[i]);
        }
        else if (quads[i]->operation == OP_RET) {
            char *operand = symbolToOperand(quads[i],quads[i]->arg1);
            char *ret = concatenateStrings("MOVQ ",operand);
            ret = concatenateStrings(ret,", ");
            ret = concatenateStrings(ret,"%rax");
            registers[0].inUse = 1;
            if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand));
            addCode(ret);
            addCode("RET");
        }
        else if (quads[i]->operation == OP_JUMP) {
            char *jump = concatenateStrings("JMP ",quads[i]->result);
            addCode(jump);
        }
    }
    printSymbolAddress();
    printf("--------------------------\n");
    printCode();
    printUsedRegisters();
}