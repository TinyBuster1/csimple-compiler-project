#ifndef TAC_H_
#define TAC_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./ast.h"
#include "./symtab-scope.h"
typedef struct Tac
{
    char *code;
    char *var;
} Tac;

char *genProg(Node *);
char *genCode(Node *, int *);
char *genStatment(Node *, int *);
char *genFunc(Node *);
char *genBlock(Node *, int *);
char *genAss(Node *, int *);

Tac *genExrp(Node *, int *);
char *genTac(char *, char *, char *);

int getSize(Node *);
int getVarsSize(Node *);
int getVarsCount(Node *);
char *getId(Node *);

bool isMathNode(char *);
bool isSimpleNode(char *);
bool isPtrNode(char *);

char *freshVar();
#endif // TAC_H_