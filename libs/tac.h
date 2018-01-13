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
Tac *genLhs(Node *, int *);
char *genBlock(Node *, int *);
char *genAss(Node *, int *);
char *genReturn(Node *, int *);
Tac *genStringIndex(Node *, int *);
Tac *genFunctionCall(Node *, int *);

Tac *genContent(Node *, int *);
Tac *genAddress(Node *, int *);

Tac *genExrp(Node *, int *);
Tac *pushParams(Node *, int *);
char *genTac(char *, char *, char *);

int getSize(Node *);
int getVarsSize(Node *);
int getVarsCount(Node *);
char *getId(Node *);
char *parseUnary(char *);

bool isMathNode(char *);
bool isFuncCall(char *);
bool isSimpleNode(char *);
bool isPtrNode(char *);
bool isBooleanNode(char *);
bool isNegNode(char *);

char *freshVar();
#endif // TAC_H_