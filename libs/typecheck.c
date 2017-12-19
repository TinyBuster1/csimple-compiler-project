
#include <stdlib.h>
#include <string.h>
#include "./typecheck.h"
#include "./ast.h"

void createScope(ScopeStack **currentScope, char *name)
{
    printf("##SCOPE OF: %s\n", name);
    // create a new scope
    ScopeStack *newScope = malloc(sizeof(ScopeStack));
    newScope->name = name;
    push(currentScope, newScope);
}

void insertVar(ScopeStack **currentScope, char * type, char * name){
    // we still need to type check
    SymbEntry * newEntry = malloc(sizeof(SymbEntry));
    newEntry->var_type = type;
    newEntry->name = name;
    insert(*currentScope, newEntry);
}

void handleMultiVarsDecl(ScopeStack **currentScope, char * type, Node *ids){
    if(strcmp("MULTIPLE IDENTIFIERS",ids->left->data)==0) 
        handleMultiVarsDecl(currentScope, type, ids->left);
    else {
        // we still need to type check
        insertVar(currentScope,type, ids->left->data);
        insertVar(currentScope,type, ids->right->data);
    }
}

void handleVarDecl(ScopeStack **currentScope, Node *ast){
    char * type = ast->left->data;
    Node * ids = ast->right;
    if(strcmp("MULTIPLE IDENTIFIERS",ids->data)==0) 
        handleMultiVarsDecl(currentScope, type, ids);
    else{
        // we still need to type check
        insertVar(currentScope,type, ids->data);
    }
}

args * getFuncArgs(Node * ast){
    args * argument = malloc(sizeof(args));
    if (strcmp("PARAMETERS", ast->data) == 0) {
        args * largs = getFuncArgs(ast->left->left);
        largs->next = getFuncArgs(ast->left->right);
        argument->next = largs;
    } else {
        printInOrder(ast,0);
        argument->type = ast->left->data;
        argument->name = ast->right->data;

        printf("P: T:%s N:%s\n", argument->type, argument->name);
    }
    return argument;
}

void handleFunctionInfo(ScopeStack **currentScope, Node* ast){
    char * name = ast->middle->data;
    function * data = malloc(sizeof(function));
    data->r_value = ast->left->data;
    data->args = getFuncArgs(ast->right);
    
    // push function arguments into args scope
    args * runner = data->args;
    while(runner){
        insertVar(currentScope,runner->type,runner->name);
        runner = runner->next;
    }

    // push function data into upper scope for future search and validation
    // we do that only after we validate the function itself!
    SymbEntry * newEntry = malloc(sizeof(SymbEntry));
    newEntry->name = name;
    newEntry->data = data;
    insert((*currentScope)->next_scope, newEntry);   
}

void typecheck(ScopeStack **currentScope, Node *ast)
{
    if (!ast) return;
    
    if(strcmp("CODE" , ast->data)==0)
    {
        typecheck(currentScope,ast->left);
        typecheck(currentScope,ast->right);
    }

    else if (strcmp("FUNCTION", ast->data) == 0)
    {
       
        createScope(currentScope, ast->data);
        handleFunctionInfo(currentScope, ast->left);
        typecheck(currentScope, ast->right);
    
    }

    else if(strcmp("BLOCK", ast->data)==0)
    {
        createScope(currentScope, ast->data);
        typecheck(currentScope,ast->left);
        typecheck(currentScope,ast->right);
    }

    ////////////////////////////////////////////
    /* STOP REC */   

    if(strcmp("VARIABLES DECLERATION",ast->data)==0)
    {
      handleVarDecl(currentScope, ast);
    }

}