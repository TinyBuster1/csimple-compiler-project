
#include <stdlib.h>
#include <string.h>
#include "./typecheck.h"

char *getFuncName(Node *ast)
{
    return ast->left->left->data;
}

void createScope(ScopeStack **currentScope, char *name)
{
    printf("##SCOPE OF: %s\n", name);
    // create a new scope
    ScopeStack *newScope = malloc(sizeof(ScopeStack));
    newScope->name = name;
    push(currentScope, newScope);
}

void sementizeFunction(ScopeStack **currentScope, Node *ast)
{
}

void typecheck(ScopeStack **currentScope, Node *ast, int depth)
{
    if (strcmp("FUNCTION", ast->data) == 0)
    {
        sementizeFunction(currentScope, ast);
        ast = ast->right->right;
    }

    if (strcmp("code", ast->data) == 0)
    {
        //sementizeCode(currentScope, ast);
        ast = ast->right->right;
    }

    if (ast->left)
        typecheck(currentScope, ast->left, depth + 1);
    if (ast->middle)
        typecheck(currentScope, ast->middle, depth + 1);
    if (ast->right)
        typecheck(currentScope, ast->right, depth + 1);

    // AFTER WE FINISHED PROC THE SUB NODES
    if (strcmp("FUNCTION", ast->data) == 0)
        pop(currentScope);
}