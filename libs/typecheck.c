
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
    char *functionParamsScopeName;
    strcat(functionParamsScopeName, ast->middle->data);
    strcat(functionParamsScopeName, " PARAMS");
    createScope(currentScope, functionParamsScopeName);
}

void typecheck(ScopeStack **currentScope, Node *ast)
{
    if (strcmp("FUNCTION", ast->data) == 0)
    {
        sementizeFunction(currentScope, ast->left);
        createScope(currentScope, ast->data);
        typecheck(currentScope, ast->right);
    }
}