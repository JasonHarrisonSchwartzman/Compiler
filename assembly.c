#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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

char **data;//global variables
int numDataLines = 0;

/**
 * concats two strings

char *concatenateStrings(char *str1, char *str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *result = (char *)malloc(len1 + len2 + 1);
    if (result == NULL) return NULL;
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}*/
char *concatenateStrings(int numStrings, ...) {
    // Initialize the result string with an empty string
    char *result = (char *)malloc(1);
    if (result == NULL) return NULL;
    result[0] = '\0';

    va_list args;
    va_start(args, numStrings);

    for (int i = 0; i < numStrings; i++) {
        char *str = va_arg(args, char *);
        if (str == NULL) {
            // Handle NULL strings gracefully
            continue;
        }
        size_t len1 = strlen(result);
        size_t len2 = strlen(str);
        char *temp = (char *)realloc(result, len1 + len2 + 1);
        if (temp == NULL) {
            free(result);
            va_end(args);
            return NULL;
        }
        result = temp;
        strcat(result, str);
    }

    va_end(args);
    return result;
}

/**
 * adds a string to the code array
*/
void addCode(char *str) {
    code = realloc(code, sizeof(char*) * (numCodeLines + 1));
    code[numCodeLines++] = str;
}

/**
 * adds a string to the data array
*/
void addData(char *str) {
    data = realloc(data,sizeof(char*) * (numDataLines+1));
    data[numDataLines++] = str;
}

/**
 * prints lines of data
*/
void printData(FILE *f) {
    if (f) {
        fprintf(f,".data\n");
        for (int i = 0; i < numDataLines; i++) fprintf(f,"%s\n",data[i]);
        return;
    }
    printf(".data\n");
    for (int i = 0; i < numDataLines; i++) {
        printf("%s\n",data[i]);
    }
}

/**
 * prints all the lines of code
*/
void printCode(FILE *f) {
    if (f) {
        fprintf(f,".text\n");
        fprintf(f,".globl _start\n");
        for (int i = 0; i < numCodeLines; i++) fprintf(f,"%s\n",code[i]);
        return;
    }
    printf(".text\n");
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
 * Used for reserve the registers needed to pass to functions
*/
int scratch_reserve(int x) {
    registers[x].inUse = 1;
    return x;
}

/**
 * Marks register as free
*/
void scratch_free(int reg) {
    registers[reg].inUse = 0;
}

/**
 * gets the register index given the strings
*/
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

char* registerBytes[16] = {
    "%al",
    "%bl",
    "%cl",
    "%dl",
    "%sil",
    "%dil",
    "%bpl",
    "%spl",
    "%r8b",
    "%r9b",
    "%r10b",
    "%r11b",
    "%r12b",
    "%r13b",
    "%r14b",
    "%r15b",
};

/**
 * Returns the byte sub-regstier
*/
char *scratch_name_byte(int reg) {
    return registerBytes[reg];
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

struct SymbolAddress **symAdds;//array of sym addresses
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
    //printf("computing address of %s\n",s->name);
    if (address) {
        //printf("found address %s\n",address);
        return address;
    }
    int size = calculateSize(quad);
    if (numSymbols == 0) {
        return addSymbolAddress(s,createAddressName(8),size,8);
    }
    else {
        int placeOnStack = symAdds[numSymbols-1]->placeOnStack + symAdds[numSymbols-1]->size;
        printf("%d\n",placeOnStack);
        return addSymbolAddress(s,createAddressName(placeOnStack),size,placeOnStack);
    }
    return NULL;
}



/**
 * Returns the address computation for a given symbol 
*/
char *symbol_codegen(struct quad *quad, struct Symbol *s) {
    if (s->sym == SYMBOL_GLOBAL) {
        printf("GLOBAL SYMBOL\n");
        return s->name;
    }
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
    if (!arg->name) {//might need to add || arg->val_t == VAL_STRING here
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
            //printf("%d\n",i);
            if (quads[i]->result && strcmp(arg->name,quads[i]->result) == 0) {
                if (quads[i]->symbol && quads[i]->symbol->sym == SYMBOL_GLOBAL) return quads[i]->symbol->name;
                //printf("name %s\n",quads[i]->result);
                if (quads[i]->symbol && quads[i]->symbol->sym == SYMBOL_LOCAL) return symbol_codegen(quads[i],quads[i]->symbol);
                return registers[quads[i]->reg].name;//value on reg
            }
            if (quads[i]->symbol && strcmp(arg->name,quads[i]->symbol->name) == 0) {
                //printf("computing variable %s\n",quads[i]->symbol->name);
                return symbol_codegen(quads[i],quads[i]->symbol);//variable
            }
        }
    }
    printf("unable to find symbol of arg name%s\n",arg->name);
    return NULL;
}

/**
 * moves the quad to a register and returns the register it's going to
*/
int move(struct quad *quad, struct argument *arg) {
    char *operand = symbolToOperand(quad,arg);
    int reg1 = scratch_alloc();
    char *code1 = concatenateStrings(4,"MOVQ ",operand,", ",scratch_name(reg1));
    if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand)); //figure out why this line was needed
    addCode(code1);
    return reg1;
}


/**
 * Creates code based on an expression (quad)
*/
void expr_codegen(struct quad *quad) {
    switch (quad->operation) {
        case OP_ADD:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *add = concatenateStrings(4,"ADDQ ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(add);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_SUB: {
            //OP: SUB | Arg1: 9 | Arg2: x | Result: t0
            //sub eax, ebx  ; Subtract the value in ebx from eax, and store the result in eax
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings(4,"SUBQ ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_MULT: {
        int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings(4,"IMUL ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_DIV: {
            int raxUsed = 0;
            int rdxUsed = 0;
            int raxTemp;
            char *movRax;
            int rdxTemp;
            char *movRdx;
            if (registers[0].inUse) {
                raxUsed = 1;
                raxTemp = scratch_alloc();
                movRax = concatenateStrings(2,"MOVQ %rax, ", scratch_name(raxTemp));
                addCode(movRax);
            }
            if (registers[3].inUse) {
                rdxUsed = 1;
                rdxTemp = scratch_alloc();
                movRdx = concatenateStrings(2,"MOVQ %rdx, ", scratch_name(rdxTemp));
                addCode(movRdx);
            }
            char *dividend = symbolToOperand(quad,quad->arg1);
            char *movDividend = concatenateStrings(3,"MOVQ ", dividend,", %rax");
            addCode(movDividend);
            char *divisor = symbolToOperand(quad,quad->arg2);
            int divisorReg = scratch_alloc();
            char *movDivisor = concatenateStrings(4,"MOVQ ", divisor,", ",scratch_name(divisorReg));
            addCode(movDivisor);
            char *div = concatenateStrings(2,"IDIVQ ",scratch_name(divisorReg));
            addCode(div);
            int reg = scratch_alloc();
            char *movResult = concatenateStrings(2,"MOVQ %rax, ",scratch_name(reg));
            addCode(movResult);
            quad->reg = reg;
            scratch_free(divisorReg);

            if (raxUsed) {
                char *movRaxBack = concatenateStrings(3, "MOVQ ", scratch_name(raxTemp), ", %rax");
                addCode(movRaxBack);
                scratch_free(raxTemp);
            }
            if (rdxUsed) {
                char *movRdxBack = concatenateStrings(3,"MOVQ ", scratch_name(rdxTemp), ", %rdx");
                addCode(movRdxBack);
                scratch_free(rdxTemp);
            }
            break; }
        case OP_MOD: {
            int raxUsed = 0;
            int rdxUsed = 0;
            int raxTemp;
            char *movRax;
            int rdxTemp;
            char *movRdx;
            if (registers[0].inUse) {
                raxUsed = 1;
                raxTemp = scratch_alloc();
                movRax = concatenateStrings(2,"MOVQ %rax, ", scratch_name(raxTemp));
                addCode(movRax);
            }
            if (registers[3].inUse) {
                rdxUsed = 1;
                rdxTemp = scratch_alloc();
                movRdx = concatenateStrings(2,"MOVQ %rdx, ", scratch_name(rdxTemp));
                addCode(movRdx);
            }

            char *dividend = symbolToOperand(quad,quad->arg1);
            char *movDividend = concatenateStrings(3,"MOVQ ", dividend,", %rax");
            addCode(movDividend);
            char *divisor = symbolToOperand(quad,quad->arg2);
            int divisorReg = scratch_alloc();
            char *movDivisor = concatenateStrings(4,"MOVQ ", divisor,", ",scratch_name(divisorReg));
            addCode(movDivisor);
            char *div = concatenateStrings(2,"IDIVQ ",scratch_name(divisorReg));
            addCode(div);
            int reg = scratch_alloc();
            char *movResult = concatenateStrings(2,"MOVQ %rdx, ",scratch_name(reg));
            addCode(movResult);
            quad->reg = reg;
            scratch_free(divisorReg);

            if (raxUsed) {
                char *movRaxBack = concatenateStrings(3,"MOVQ ", scratch_name(raxTemp), ", %rax");
                addCode(movRaxBack);
                scratch_free(raxTemp);
            }
            if (rdxUsed) {
                char *movRdxBack = concatenateStrings(3,"MOVQ ", scratch_name(rdxTemp),", %rdx");
                addCode(movRdxBack);
                scratch_free(rdxTemp);
            }
            break; }
        case OP_BITAND: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *and = concatenateStrings(4,"AND ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(and);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_BITOR: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *or = concatenateStrings(4,"OR ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(or);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_BITXOR: {
            printf("XOR\n");
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *xor = concatenateStrings(4,"XORQ ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(xor);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_AND: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *and = concatenateStrings(4,"TEST ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(and);
            scratch_free(reg1);
            int reg3 = scratch_alloc();
            char *move = concatenateStrings(2,"MOVQ $0, ",scratch_name(reg3));
            addCode(move);
            char *cmp = concatenateStrings(2,"CMP $0, ",scratch_name(reg2));
            addCode(cmp);
            char *cmov = concatenateStrings(2,"CMOVNE $1,", scratch_name(reg3));
            addCode(cmov);
            scratch_free(reg2);
            quad->reg = reg3;
            break; }
        case OP_OR: {
            //THIS CODE COULD BE USED FOR SHORT CIRCUITING MAYBE
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *or = concatenateStrings(4,"OR ",scratch_name(reg1),", ",scratch_name(reg2));
            addCode(or);
            scratch_free(reg1);
            int reg3 = scratch_alloc();
            char *move = concatenateStrings(2,"MOVQ $0, ",scratch_name(reg3));
            addCode(move);
            char *cmp = concatenateStrings(2,"CMP $0, ",scratch_name(reg2));
            addCode(cmp);
            char *cmov = concatenateStrings(2,"CMOVNE $1,", scratch_name(reg3));
            addCode(cmov);
            scratch_free(reg2);
            quad->reg = reg3;
            break;}
        case OP_NEQ:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *cmp = concatenateStrings(4,"CMP ",scratch_name(reg2),", ",scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);
            int reg3 = scratch_alloc();
            char *set = concatenateStrings(2,"SETNE ",scratch_name_byte(reg3));
            addCode(set);
            char *clear = concatenateStrings(2,"AND $0x000000FF, ", scratch_name(reg3));
            addCode(clear);
            quad->reg = reg3;
            break; }
        case OP_GEQ:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *cmp = concatenateStrings(4,"CMP ",scratch_name(reg2),", ",scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings(2,"SETGE ",scratch_name_byte(reg3));
            addCode(set);
            char *clear = concatenateStrings(2,"AND $0x000000FF, ", scratch_name(reg3));
            addCode(clear);
            quad->reg = reg3;
            break; }
        case OP_LEQ:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *cmp = concatenateStrings(4,"CMP ",scratch_name(reg2),", ",scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings(2,"SETLE ",scratch_name_byte(reg3));
            addCode(set);
            char *clear = concatenateStrings(2,"AND $0x000000FF, ", scratch_name(reg3));
            addCode(clear);
            quad->reg = reg3;
            break; }
        case OP_GREAT:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings(4,"CMP ",scratch_name(reg2),", ",scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings(2,"SETG ",scratch_name_byte(reg3));
            addCode(set);
            char *clear = concatenateStrings(2,"AND $0x000000FF, ", scratch_name(reg3));
            addCode(clear);
            quad->reg = reg3;
            break; }
        case OP_LESS: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings(4,"CMP ",scratch_name(reg2),", ",scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);
            int reg3 = scratch_alloc();
            char *set = concatenateStrings(2,"SETL ",scratch_name_byte(reg3));
            addCode(set);
            char *clear = concatenateStrings(2,"AND $0x000000FF, ", scratch_name(reg3));
            addCode(clear);
            quad->reg = reg3;
            break; }
        case OP_EQ: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings(4,"CMP ",scratch_name(reg2),", ",scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings(2,"SETE ",scratch_name_byte(reg3));
            addCode(set);
            char *clear = concatenateStrings(2,"AND $0x000000FF, ", scratch_name(reg3));
            addCode(clear);
            quad->reg = reg3;
            break; }
        case OP_REF:{
            char *var = symbolToOperand(quad,quad->arg1);
            int reg1 = scratch_alloc();
            quad->reg = reg1;
            char *lea = concatenateStrings(4,"LEAQ ", var,", ",scratch_name(reg1));
            addCode(lea);
            break; }
        case OP_DEREF: {
            char *var = symbolToOperand(quad,quad->arg1);
            int reg1 = scratch_alloc();
            char *move = concatenateStrings(4,"MOVQ ", var,", ",scratch_name(reg1));
            quad->reg = reg1;
            addCode(move);
            break; }
        case OP_ARRAY_INDEX: {
            //lea (array + index * 8) = rax

            //loading address of array

            if (!quad->symbol) {//right side of expression
                char *operand = symbolToOperand(quad,quad->arg1);
                int reg1 = scratch_alloc();
                char *code1 = concatenateStrings(4,"LEAQ (",operand,"), ",scratch_name(reg1));
                if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand)); //figure out why this line was needed
                addCode(code1);

                int reg2 = move(quad,quad->arg2);
                int reg3 = scratch_alloc();
                char *arrayIndex = concatenateStrings(6,"MOVSLQ (",scratch_name(reg1),",",scratch_name(reg2),",8), ",scratch_name(reg3));
                addCode(arrayIndex);
                scratch_free(reg1);
                scratch_free(reg2);
                quad->reg = reg3;
            }
            else {//left side of expression

                char *operand = symbolToOperand(quad,quad->arg1);
                int reg1 = scratch_alloc();
                char *code1 = concatenateStrings(4,"LEAQ (",operand,"), ",scratch_name(reg1));
                if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand)); //figure out why this line was needed
                addCode(code1);

                int reg2 = move(quad,quad->arg1);
                int reg3 = move(quad,quad->arg2);

                char *store = concatenateStrings(7,"MOVQ ",scratch_name(reg3),", (",scratch_name(reg1),",",scratch_name(reg2),",8)");
                addCode(store);
                scratch_free(reg1);
                scratch_free(reg2);
                scratch_free(reg3);
            }
            break; }
        default:
        return;
    }
}

/**
 * adds parameters to the stack given the amount of parameters
 * free registers containing params
*/
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

/**
 * converts the number param to the corresponding register
*/
char *argNumToReg(int x) {
    if (x == 6) return "%r9";
    else if (x==5) return "%r8";
    else if (x==4) return "%rcx";
    else if (x==3) return "%rdx";
    else if (x==2) return "%rsi";
    else if (x==1) return "%rdi";
    else return "ARG NUM NOT FOUND";
}

/**
 * prints the used registers
*/
void printUsedRegisters() {
    for (int i = 0; i < numRegisters; i++) {
        if (registers[i].inUse) {
            printf("%s\n",registers[i].name);
        }
    }
}

/**
 * converts int to string
*/
char *intToString(int num) {
    int totalLength = snprintf(NULL, 0, "%d", num) + 1;
    char *result = (char *)malloc(totalLength);
    if (result != NULL) {
        snprintf(result, totalLength, "%d", num);
    }
    return result;
}

/**
 * adds a global variable to data array
*/
void globalDecl(struct quad *quad) {
    if (quad->arg1->val_t == VAL_STRING) {
        char *decl = concatenateStrings(3,".",quad->result,":");
        addData(decl);
        char *decl2 = concatenateStrings(3,".string ",quad->arg1->name,"\"");
        addData(decl2);
        char *decl3 = concatenateStrings(2,quad->result,":");
        addData(decl3);
        char *decl4 = concatenateStrings(2,".quad .",quad->result);
        addData(decl4);
        return;
    }
    char *decl = concatenateStrings(2,quad->result, ": .quad ");
    if (quad->operation == OP_ARRAY_CREATE) {//array
        printf("CREATING GLOBAL ARRAY\n");
        for (int i = 0; i < getValue(quad->arg1->val_t,quad->arg1->value) - 1; i++) {
            decl = concatenateStrings(3,decl, intToString(getValue(quad->arg2->val_t,quad->arg2->value)),", ");
        }
        decl = concatenateStrings(2,decl, intToString(getValue(quad->arg2->val_t,quad->arg2->value)));
    }
    else {//single var
        decl = concatenateStrings(2,decl, intToString(getValue(quad->arg1->val_t,quad->arg1->value)));
    }
    addData(decl);
}

/**
 * Prints a string
*/
void addPrints() {
    addCode("_prints:");
    addCode("PUSHQ %rbp");
    addCode("MOVQ %rsp, %rbp");
    addCode("PUSHQ %rdi");
    addCode("XOR %rcx, %rcx");
    addCode("NOT %rcx");
    addCode("XOR %al,%al");
    addCode("CLD");
    addCode("REPNZ scasb");
    addCode("NOT %rcx");
    addCode("DEC %rcx");
    addCode("MOVQ %rcx,%rdx");
    addCode("MOVQ -8(%rbp), %rsi");
    addCode("MOVQ $1, %rax");
    addCode("MOVQ %rax, %rdi");
    addCode("SYSCALL");
    addCode("MOVQ %rbp, %rsp");
    addCode("POPQ %rbp");
    addCode("RET");
}

/**
 * Prints a digit
*/
void addPrintd() {
    addCode("_printd:");
    addCode("PUSHQ %rbp");
    addCode("MOVQ %rsp, %rbp");
    addCode("TEST %rdi, %rdi");
    addCode("JNS .positive");
    addCode("NEG %rdi");
    addCode("MOVQ %rdi, %r9");
    addCode("MOVQ negative, %rdi");
    addCode("CALL _prints");
    addCode("MOVQ %r9, %rdi");
    addCode(".positive:");
    addCode("MOVQ %rdi, %rax");
    addCode("MOVQ $10, %rcx");
    addCode("PUSHQ %rcx");
    addCode("MOVQ %rsp, %rsi");
    addCode("SUBQ $16, %rsp");
    addCode(".toascii_digit:");
    addCode("XOR %rdx, %rdx");
    addCode("DIVQ %rcx");
    addCode("ADDQ $48, %rdx");
    addCode("DEC %rsi");
    addCode("MOV %dl, (%rsi)");
    addCode("TEST %rax, %rax");
    addCode("JNZ .toascii_digit");
    addCode("MOVQ $1, %rax");
    addCode("MOVQ $1, %rdi");
    addCode("LEAQ 16(%rsp), %rdx");
    addCode("SUBQ %rsi,%rdx");
    addCode("SYSCALL");
    addCode("ADDQ $24, %rsp");
    addCode("POPQ %rbp");
    addCode("RET");
}

/**
 * Built in functions such as print
*/
void addBuiltInFunctions() {
    addPrints();
    addPrintd();
}
void addBuiltInData() {
    addData(".negative:");
    addData(".string \"-\"");
    addData("negative:");
    addData(".quad .negative");
}

/**
 * Generates code given IR
*/
void generateCode() {
    addBuiltInData();
    addBuiltInFunctions();
    int inFunction = 0;
    int paramNum = 1;
    int stackSize = 0;
    for (int i = 0; i < numQuads; i++) {
        quads[i]->numQuad = i;
        //printf("i: %d\n",i);
        if (quads[i]->symbol && quads[i]->operation != OP_LABEL) {//symbol address (local variable/parameter)
            //stack size
            if (inFunction && (quads[i]->symbol->sym == SYMBOL_LOCAL)) {
                char *address = lookUpAddress(quads[i]->symbol);
                if (!address) {
                    addCode("SUBQ $8, %rsp");
                    stackSize+=8;
                }
            }
            
            printf("calcing symbol %s\n",quads[i]->symbol->name);
            symbol_codegen(quads[i],quads[i]->symbol);
        }
        if (quads[i]->operation == OP_ARRAY_CREATE) {
            if (!inFunction) globalDecl(quads[i]);
            else {
                
            }
        }
        if (quads[i]->operation == OP_LABEL) {
            if (strcmp(quads[i]->result,"end func") == 0) {
                inFunction = 0;
                //printf("out of function\n");
                continue;
            }
            if (quads[i]->symbol) {//new function
                inFunction = 1;
                printf("In function\n");
                printSymbolAddress();

                symAdds = realloc(symAdds, 0);
                numSymbols = 0;
                stackSize = 0;

                char *func = concatenateStrings(3,"_",quads[i]->result,":");
                addCode(func);
                addCode("PUSHQ %rbp");
                addCode("MOVQ %rsp, %rbp");

                printf("added func\n");
                addParamsToStack(quads[i]);
                printf("params added to stack\n");
                int funcNum = i;
                for (int j = 0; j < getValue(quads[funcNum]->arg1->val_t,quads[funcNum]->arg1->value); j++) {
                    quads[i]->numQuad = i;
                    char *address = lookUpAddress(quads[i]->symbol);
                    if (!address) {
                        addCode("SUBQ $8, %rsp");
                        stackSize+=8;
                    }
                    symbol_codegen(quads[i],quads[i]->symbol);
                    i++;
                }
                printf("done adding params\n");
            }
            else {
                char *label = concatenateStrings(2,quads[i]->result,":");
                addCode(label);
            }
        }
        else if (quads[i]->operation == OP_ASSIGN) {//make sure to do someting different with globals
            //printf("ASSIGN\n");
            if (inFunction == 0) {
                symbol_codegen(quads[i],quads[i]->symbol);
                //printf("GLOBAL VAR %s\n",quads[i]->result);
                globalDecl(quads[i]);
                continue;
            }
            //printf("LOCAL VAR %s\n",quads[i]->symbol->name);
            quads[i]->reg = move(quads[i],quads[i]->arg1);
            char *storeVar = concatenateStrings(4,"MOVQ ", scratch_name(quads[i]->reg),", ",symbol_codegen(quads[i],quads[i]->symbol));
            scratch_free(quads[i]->reg);
            addCode(storeVar);
        }
        else if (quads[i]->operation == OP_PARAM) {
            printf("quad %d PARAM name: %s\n",i,quads[i]->arg1->name);
            char *arg = symbolToOperand(quads[i],quads[i]->arg1);
            char *regArg = argNumToReg(paramNum);
            char *movArg = concatenateStrings(4,"MOVQ ", arg,", ",regArg);
            addCode(movArg);
            paramNum++;
        }
        else if (quads[i]->operation == OP_CALL) {
            char *call = concatenateStrings(2,"CALL _",quads[i]->arg1->name);
            addCode(call);
            for (int j = 1; j < paramNum; j++) {
                scratch_free(regNameToNum(argNumToReg(j)));//frees the argument register
            }
            paramNum = 1;
        }
        else if (quads[i]->operation < 19) {
            expr_codegen(quads[i]);
        }
        else if (quads[i]->operation == OP_RET) {
            char *operand = symbolToOperand(quads[i],quads[i]->arg1);
            if (regNameToNum(operand) == 0) {addCode("RET");continue;}//if value being returned is already on %rax
            char *ret = concatenateStrings(3,"MOVQ ",operand,", %rax");
            registers[0].inUse = 1;
            if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand));
            addCode(ret);

            char *addBackToStack = concatenateStrings(3,"ADDQ $",intToString(stackSize),", %rsp");
            addCode(addBackToStack);
            addCode("MOVQ %rbp, %rsp");
            addCode("POPQ %rbp");
            addCode("RET");
        }
        else if (quads[i]->operation == OP_JUMP) {
            char *jump = concatenateStrings(2,"JMP ",quads[i]->result);
            addCode(jump);
        }
        else if (quads[i]->operation == OP_JUMPIFNOT) {
            int reg1 = move(quads[i],quads[i]->arg1);
            char *cmp = concatenateStrings(2,"CMP $1, ",scratch_name(reg1));
            addCode(cmp);
            char *jump = concatenateStrings(2,"JNE ", quads[i]->result);
            addCode(jump);
            scratch_free(reg1);
        }
    }
    printf("MAIN CODE GEN FINISHED\n");
    printSymbolAddress();
    printf("--------------------------\n");

    //exits program
    addCode("MOVQ $60, %rax");
    addCode("XOR %rdi, %rdi");
    addCode("syscall");

    printData(NULL);
    printCode(NULL);
    printf("-------Used Registers--------\n");
    printUsedRegisters();
}