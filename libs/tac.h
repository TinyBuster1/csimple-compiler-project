#ifndef TAC_H_
#define TAC_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./ast.h"

char *genTac(Node *);
char *genCode(Node *);
char *genFunc(Node *);
char *genBlock(Node *, int *);

int getSize(Node *codeNode);
int getVarsSize(Node *varsNode);
int getVarsCount(Node *multiVars);

char *freshVar();
#endif // TAC_H_