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

char **data;//global variables
int numDataLines = 0;

/**
 * concats two strings
*/
char *concatenateStrings(char *str1, char *str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char *result = (char *)malloc(len1 + len2 + 1);
    if (result == NULL) return NULL;
    strcpy(result, str1);
    strcat(result, str2);
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
    char *code1 = concatenateStrings("MOVQ ",operand);
    int reg1 = scratch_alloc();
    code1 = concatenateStrings(code1,", ");
    code1 = concatenateStrings(code1,scratch_name(reg1));
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
                movRax = concatenateStrings("MOVQ %rax, ", scratch_name(raxTemp));
                addCode(movRax);
            }
            if (registers[3].inUse) {
                rdxUsed = 1;
                rdxTemp = scratch_alloc();
                movRdx = concatenateStrings("MOVQ %rdx, ", scratch_name(rdxTemp));
                addCode(movRdx);
            }

            char *dividend = symbolToOperand(quad,quad->arg1);
            char *movDividend = concatenateStrings("MOVQ ", dividend);
            movDividend = concatenateStrings(movDividend,", %rax");
            addCode(movDividend);
            char *divisor = symbolToOperand(quad,quad->arg2);
            char *movDivisor = concatenateStrings("MOVQ ", divisor);
            int divisorReg = scratch_alloc();
            movDivisor = concatenateStrings(movDivisor,", ");
            movDivisor = concatenateStrings(movDivisor,scratch_name(divisorReg));
            addCode(movDivisor);
            char *div = concatenateStrings("IDIVQ ",scratch_name(divisorReg));
            addCode(div);
            int reg = scratch_alloc();
            char *movResult = concatenateStrings("MOVQ %rax, ",scratch_name(reg));
            addCode(movResult);
            quad->reg = reg;
            scratch_free(divisorReg);

            if (raxUsed) {
                char *movRaxBack = concatenateStrings("MOVQ ", scratch_name(raxTemp));
                movRaxBack = concatenateStrings(movRaxBack, ", %rax");
                addCode(movRaxBack);
                scratch_free(raxTemp);
            }
            if (rdxUsed) {
                char *movRdxBack = concatenateStrings("MOVQ ", scratch_name(rdxTemp));
                movRdxBack = concatenateStrings(movRdxBack, ", %rdx");
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
                movRax = concatenateStrings("MOVQ %rax, ", scratch_name(raxTemp));
                addCode(movRax);
            }
            if (registers[3].inUse) {
                rdxUsed = 1;
                rdxTemp = scratch_alloc();
                movRdx = concatenateStrings("MOVQ %rdx, ", scratch_name(rdxTemp));
                addCode(movRdx);
            }

            char *dividend = symbolToOperand(quad,quad->arg1);
            char *movDividend = concatenateStrings("MOVQ ", dividend);
            movDividend = concatenateStrings(movDividend,", %rax");
            addCode(movDividend);
            char *divisor = symbolToOperand(quad,quad->arg2);
            char *movDivisor = concatenateStrings("MOVQ ", divisor);
            int divisorReg = scratch_alloc();
            movDivisor = concatenateStrings(movDivisor,", ");
            movDivisor = concatenateStrings(movDivisor,scratch_name(divisorReg));
            addCode(movDivisor);
            char *div = concatenateStrings("IDIVQ ",scratch_name(divisorReg));
            addCode(div);
            int reg = scratch_alloc();
            char *movResult = concatenateStrings("MOVQ %rdx, ",scratch_name(reg));
            addCode(movResult);
            quad->reg = reg;
            scratch_free(divisorReg);

            if (raxUsed) {
                char *movRaxBack = concatenateStrings("MOVQ ", scratch_name(raxTemp));
                movRaxBack = concatenateStrings(movRaxBack, ", %rax");
                addCode(movRaxBack);
                scratch_free(raxTemp);
            }
            if (rdxUsed) {
                char *movRdxBack = concatenateStrings("MOVQ ", scratch_name(rdxTemp));
                movRdxBack = concatenateStrings(movRdxBack, ", %rdx");
                addCode(movRdxBack);
                scratch_free(rdxTemp);
            }
            break; }
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
            printf("XOR\n");
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *sub = concatenateStrings("XORQ ",scratch_name(reg1));
            sub = concatenateStrings(sub, ", ");
            sub = concatenateStrings(sub,scratch_name(reg2));
            addCode(sub);
            scratch_free(reg1);
            quad->reg = reg2;
            break; }
        case OP_AND: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *and = concatenateStrings("TEST ",scratch_name(reg1));
            and = concatenateStrings(and, ", ");
            and = concatenateStrings(and,scratch_name(reg2));
            addCode(and);
            scratch_free(reg1);
            int reg3 = scratch_alloc();
            char *move = concatenateStrings("MOVQ $0, ",scratch_name(reg3));
            addCode(move);
            char *cmp = concatenateStrings("CMP $0, ",scratch_name(reg2));
            addCode(cmp);
            char *cmov = concatenateStrings("CMOVNE $1,", scratch_name(reg3));
            addCode(cmov);
            scratch_free(reg2);
            quad->reg = reg3;
            break; }
        case OP_OR: {
            /*int reg1 = move(quad,quad->arg1);
            char *cmp = concatenateStrings("CMP $0, ", scratch_name(reg1));//comparies 0 and first value
            addCode(cmp);
            int dstReg1 = scratch_alloc();
            char *tempReg1 = concatenateStrings("MOVQ $0, ", scratch_name(dstReg1));
            addCode(tempReg1);
            char *cmovReg1 = concatenateStrings("CMOVNE ",scratch_name(reg1));
            cmovReg1 = concatenateStrings(cmovReg1, ", ");
            cmovReg1 = concatenateStrings(cmovReg1,scratch_name(dstReg1));*/ 
            //THIS CODE COULD BE USED FOR SHORT CIRCUITING MAYBE
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *or = concatenateStrings("OR ",scratch_name(reg1));
            or = concatenateStrings(or, ", ");
            or = concatenateStrings(or,scratch_name(reg2));
            addCode(or);
            scratch_free(reg1);
            int reg3 = scratch_alloc();
            char *move = concatenateStrings("MOVQ $0, ",scratch_name(reg3));
            addCode(move);
            char *cmp = concatenateStrings("CMP $0, ",scratch_name(reg2));
            addCode(cmp);
            char *cmov = concatenateStrings("CMOVNE $1,", scratch_name(reg3));
            addCode(cmov);
            scratch_free(reg2);
            quad->reg = reg3;

            break;}
        case OP_NEQ:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings("CMP ",scratch_name(reg1));
            cmp = concatenateStrings(cmp, ", ");
            cmp = concatenateStrings(cmp,scratch_name(reg2));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings("SETNE ",scratch_name_byte(reg3));
            addCode(set);
            quad->reg = reg3;
            break; }
        case OP_GEQ:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings("CMP ",scratch_name(reg1));
            cmp = concatenateStrings(cmp, ", ");
            cmp = concatenateStrings(cmp,scratch_name(reg2));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings("SETGE ",scratch_name_byte(reg3));
            addCode(set);
            quad->reg = reg3;
            break; }
        case OP_LEQ:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings("CMP ",scratch_name(reg1));
            cmp = concatenateStrings(cmp, ", ");
            cmp = concatenateStrings(cmp,scratch_name(reg2));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings("SETLE ",scratch_name_byte(reg3));
            addCode(set);
            quad->reg = reg3;
            break; }
        case OP_GREAT:{
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings("CMP ",scratch_name(reg1));
            cmp = concatenateStrings(cmp, ", ");
            cmp = concatenateStrings(cmp,scratch_name(reg2));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings("SETG ",scratch_name_byte(reg3));
            addCode(set);
            quad->reg = reg3;
            break; }
        case OP_LESS: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings("CMP ",scratch_name(reg2));
            cmp = concatenateStrings(cmp, ", ");
            cmp = concatenateStrings(cmp,scratch_name(reg1));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);
            int reg3 = scratch_alloc();
            char *set = concatenateStrings("SETL ",scratch_name_byte(reg3));
            addCode(set);
            quad->reg = reg3;
            break; }
        case OP_EQ: {
            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            //printf("REG1:%d REG2:%d\n",reg1,reg2);
            char *cmp = concatenateStrings("CMP ",scratch_name(reg1));
            cmp = concatenateStrings(cmp, ", ");
            cmp = concatenateStrings(cmp,scratch_name(reg2));
            addCode(cmp);
            scratch_free(reg1);
            scratch_free(reg2);

            int reg3 = scratch_alloc();
            char *set = concatenateStrings("SETE ",scratch_name_byte(reg3));
            addCode(set);
            quad->reg = reg3;
            break; }
        case OP_REF:{
            char *var = symbolToOperand(quad,quad->arg1);
            char *lea = concatenateStrings("LEAQ ", var);
            int reg1 = scratch_alloc();
            quad->reg = reg1;
            lea = concatenateStrings(lea, ", ");
            lea = concatenateStrings(lea,scratch_name(reg1));
            addCode(lea);
            break; }
        case OP_DEREF: {
            char *var = symbolToOperand(quad,quad->arg1);
            int reg1 = scratch_alloc();
            char *move = concatenateStrings("MOVQ ", var);
            move = concatenateStrings(move, ", ");
            move = concatenateStrings(move, scratch_name(reg1));
            quad->reg = reg1;
            addCode(move);
            break; }
        case OP_ARRAY_INDEX: {
            //lea (array + index * 8) = rax

            int reg1 = move(quad,quad->arg1);
            int reg2 = move(quad,quad->arg2);
            char *arrayIndex = concatenateStrings("MOVSLQ (",scratch_name(reg1));
            arrayIndex = concatenateStrings(arrayIndex, ",");
            arrayIndex = concatenateStrings(arrayIndex,scratch_name(reg2));
            arrayIndex = concatenateStrings(arrayIndex,",8), ");
            int reg3 = scratch_alloc();
            arrayIndex = concatenateStrings(arrayIndex, scratch_name(reg3));
            addCode(arrayIndex);
            scratch_free(reg1);
            scratch_free(reg2);
            quad->reg = reg3;
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
        char *decl = concatenateStrings(".",quad->result);
        decl = concatenateStrings(decl,":");
        addData(decl);
        char *decl2 = concatenateStrings(".string ",quad->arg1->name);
        decl2 = concatenateStrings(decl2,"\"");
        addData(decl2);
        char *decl3 = concatenateStrings(quad->result,":");
        addData(decl3);
        char *decl4 = concatenateStrings(".quad .",quad->result);
        addData(decl4);
        return;
    }
    char *decl = concatenateStrings(quad->result, ": ");
    decl = concatenateStrings(decl, ".quad ");
    if (quad->operation == OP_ARRAY_CREATE) {//array
        for (int i = 0; i < getValue(quad->arg1->val_t,quad->arg1->value) - 1; i++) {
            decl = concatenateStrings(decl, intToString(getValue(quad->arg1->val_t,quad->arg1->value)));
            decl = concatenateStrings(decl, ", ");
        }
        decl = concatenateStrings(decl, intToString(getValue(quad->arg1->val_t,quad->arg1->value)));
    }
    else {//single var
        decl = concatenateStrings(decl, intToString(getValue(quad->arg1->val_t,quad->arg1->value)));
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
        printf("i: %d\n",i);
        if (quads[i]->symbol && quads[i]->operation != OP_LABEL) {//symbol address (local variable/parameter)
            printf("calcing symbol %s\n",quads[i]->symbol->name);
            symbol_codegen(quads[i],quads[i]->symbol);
            
            //stack size
            addCode("SUBQ $8, %rsp");
            stackSize+=8;
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

                char *func = concatenateStrings("_",quads[i]->result);
                func = concatenateStrings(func,":");
                addCode(func);
                addCode("PUSHQ %rbp");
                addCode("MOVQ %rsp, %rbp");

                printf("added func\n");
                addParamsToStack(quads[i]);
                printf("params added to stack\n");
                int funcNum = i;
                for (int j = 0; j < getValue(quads[funcNum]->arg1->val_t,quads[funcNum]->arg1->value); j++) {
                    quads[i]->numQuad = i;
                    symbol_codegen(quads[i],quads[i]->symbol);
                    i++;
                }
                /*while (quads[i]->operation == OP_PARAM) {
                    quads[i]->numQuad = i;
                    symbol_codegen(quads[i],quads[i]->symbol);
                    i++;
                }*/
                printf("done adding params\n");
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
                //printf("GLOBAL VAR %s\n",quads[i]->result);
                globalDecl(quads[i]);
                continue;
            }
            //printf("LOCAL VAR %s\n",quads[i]->symbol->name);
            quads[i]->reg = move(quads[i],quads[i]->arg1);
            char *storeVar = concatenateStrings("MOVQ ", scratch_name(quads[i]->reg));
            storeVar = concatenateStrings(storeVar, ", ");
            storeVar = concatenateStrings(storeVar,symbol_codegen(quads[i],quads[i]->symbol));
            scratch_free(quads[i]->reg);
            addCode(storeVar);
        }
        else if (quads[i]->operation == OP_PARAM) {
            printf("PARAM\n");
            char *arg = symbolToOperand(quads[i],quads[i]->arg1);
            char *movArg = concatenateStrings("MOVQ ", arg);
            movArg = concatenateStrings(movArg, ", ");
            char *regArg = argNumToReg(paramNum);
            movArg = concatenateStrings(movArg,regArg);
            addCode(movArg);
            printf("SJLDKFJSD:\n");
            paramNum++;
        }
        else if (quads[i]->operation == OP_CALL) {
            char *call = concatenateStrings("CALL _",quads[i]->arg1->name);
            addCode(call);
            for (int j = 1; j < paramNum; j++) {
                scratch_free(regNameToNum(argNumToReg(j)));//frees the argument register
            }
            paramNum = 1;
        }
        /*else if (quads[i]->operation == OP_CALL) {
            //printf("CALL %s\n",quads[i]->arg1->name);
            int numArgs = getValue(quads[i]->arg2->val_t,quads[i]->arg2->value);
            for (int j = i + 1; j < numQuads; j++) {
                if (numArgs == 0) break;
                quads[j]->numQuad = j;
                if (quads[j]->operation == OP_PARAM) {
                    char *arg = symbolToOperand(quads[j],quads[j]->arg1);
                    char *movArg = concatenateStrings("MOVQ ", arg);
                    movArg = concatenateStrings(movArg, ", ");
                    char *regArg = argNumToReg(getValue(quads[i]->arg2->val_t,quads[i]->arg2->value) - numArgs + 1);
                    movArg = concatenateStrings(movArg,regArg);
                    addCode(movArg);
                    numArgs--;
                }
                else {
                    //printf("OPERATION %d\n",quads[j]->operation);
                    expr_codegen(quads[j]);
                }
            }
            char *call = concatenateStrings("CALL _",quads[i]->arg1->name);
            addCode(call);
        }*/
        else if (quads[i]->operation < 19) {
            expr_codegen(quads[i]);
        }
        else if (quads[i]->operation == OP_RET) {
            char *operand = symbolToOperand(quads[i],quads[i]->arg1);
            if (regNameToNum(operand) == 0) {addCode("RET");continue;}//if value being returned is already on %rax
            char *ret = concatenateStrings("MOVQ ",operand);
            ret = concatenateStrings(ret,", ");
            ret = concatenateStrings(ret,"%rax");
            registers[0].inUse = 1;
            if (regNameToNum(operand) > -1) scratch_free(regNameToNum(operand));
            addCode(ret);

            char *addBackToStack = concatenateStrings("ADDQ $",intToString(stackSize));
            addBackToStack = concatenateStrings(addBackToStack,", %rsp");
            addCode(addBackToStack);
            addCode("MOVQ %rbp, %rsp");
            addCode("POPQ %rbp");
            addCode("RET");
        }
        else if (quads[i]->operation == OP_JUMP) {
            char *jump = concatenateStrings("JMP ",quads[i]->result);
            addCode(jump);
        }
        else if (quads[i]->operation == OP_JUMPIFNOT) {
            int reg1 = move(quads[i],quads[i]->arg1);
            char *cmp = concatenateStrings("CMP $0, ",scratch_name(reg1));
            addCode(cmp);
            char *jump = concatenateStrings("JNE ", quads[i]->result);
            addCode(jump);
            scratch_free(reg1);
        }
    }
    printf("MAIN CODE GEN FINISHED\n");
    printSymbolAddress();
    printf("--------------------------\n");
    addCode("MOVQ $60, %rax");
    addCode("XOR %rdi, %rdi");
    addCode("syscall");
    printData(NULL);
    printCode(NULL);
    printf("-------Used Registers--------\n");
    printUsedRegisters();
}