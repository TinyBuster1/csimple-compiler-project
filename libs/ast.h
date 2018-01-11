#ifndef AST_H_
#define AST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum type {
    VOID,
    BOOLEAN,
    CHAR,
    STRING,
    INTEGER,
    INT_PTR,
    CHAR_PTR
} type;

// binary tree struct
typedef struct Node
{
    char *data;
    int line;
    type type;
    struct Node *left, *middle, *right, *four;
} Node;
// binary tree code declerations
Node *makeBaseLeaf(char *);
Node *makeParentNode(char *, Node *);
Node *makePairNode(char *, Node *, Node *);
Node *makeTripNode(char *, Node *, Node *, Node *);
void printInOrder(Node *, int);

#endif