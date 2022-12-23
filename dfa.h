#ifndef DFA_H
#define DFA_H
#include "token.h"

void addTransition(int,char,int,int);
void initialize();
int takeTransition(int,char,int*);

#endif
