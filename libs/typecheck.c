
#include <stdlib.h>
#include <string.h>
#include "./typecheck.h"
#include "./ast.h"

// single main function flag
bool MAIN_FLAG = false;
/*****************************/
// CREATE THE SCOPES STACK LIST
ScopeStack **currentScope;

void createScope(char *name)
{
    // printf("##SCOPE OF: %s\n", name);
    // create a new scope
    ScopeStack *newScope = malloc(sizeof(ScopeStack));
    newScope->name = name;
    push(currentScope, newScope);
}

bool insertVar(char *type, char *name)
{
    if (!searchScope(currentScope, name))
    {
        SymbEntry *newEntry = malloc(sizeof(SymbEntry));
        newEntry->var_type = type;
        newEntry->name = name;
        insert(*currentScope, newEntry);
        return true;
    }
    return false;
}

void handleMultiVarsDecl(char *type, Node *ids)
{
    if (strcmp("MULTIPLE IDENTIFIERS", ids->left->data) == 0)
    {
        handleMultiVarsDecl(type, ids->left);
        insertVar(type, ids->right->left->data);
    }
    else
    {
        insertVar(type, ids->left->left->data);
        insertVar(type, ids->right->left->data);
    }
}

void handleVarDecl(Node *ast)
{

    char *type = ast->left->data;
    if (strcmp("void", type) == 0)
    {
        fprintf(stderr, "varialbes of type 'void' can not be declered\n");
        return;
    }
    Node *ids = ast->right;
    if (strcmp("MULTIPLE IDENTIFIERS", ids->data) == 0)
        handleMultiVarsDecl(type, ids);
    else
        insertVar(type, ids->left->data);
}

void printArgsList(args *list)
{
    while (list)
    {
        printf("%s %s --> ", list->name, list->type);
        list = list->next;
    }
    printf("NULL\n");
}

args *getFuncArgs(Node *ast)
{
    if (!ast)
        return NULL;
    args *argument = malloc(sizeof(args));
    if (strcmp("PARAMETERS", ast->data) == 0)
    {
        argument = getFuncArgs(ast->right);
        argument->next = getFuncArgs(ast->left);
    }
    else
    {
        argument->type = ast->left->data;
        argument->name = ast->right->data;
    }
    return argument;
}

bool validateMain(function *data)
{
    if (MAIN_FLAG)
    {
        fprintf(stderr, "multiple definition of `main' was found\n");
        return false;
    }
    // now verify main is defind in global scope
    if ((*currentScope)->next_scope)
    {
        fprintf(stderr, "'main' was defind in inner scope\n");
        return false;
    }
    // check that args is empty
    if (data->args)
    {
        fprintf(stderr, "'main' have arguments\n");
        return false;
    }
    MAIN_FLAG = true;
    return true;
}

bool handleFunctionInfo(Node *ast)
{
    char *name = ast->middle->left->data;
    function *data = malloc(sizeof(function));
    data->r_value = ast->left->data;
    data->args = getFuncArgs(ast->right);

    if (strcmp("main", name) == 0 && !validateMain(data))
        return false;

    // creating scope for function args
    createScope(ast->data);
    // push function arguments into args scope
    args *runner = data->args;
    while (runner)
    {
        insertVar(runner->type, runner->name);
        runner = runner->next;
    }

    // push function data into upper scope for future search and validation
    // we do that only after we validate the function block!
    SymbEntry *newEntry = malloc(sizeof(SymbEntry));
    newEntry->name = name;
    newEntry->data = data;
    insert((*currentScope)->next_scope, newEntry);

    return true;
}

bool validateSameType(Node *ast)
{
    printf("%s:%s\n", ast->left->left->left->data, typeToChar(getType(ast->left)));
    printf("%s:%s\n", ast->right->left->left->data, typeToChar(getType(ast->right)));
    if (getType(ast->left) != getType(ast->right))
    {
        fprintf(stderr, "Different types were found on '%s'\n", ast->data);
        return false;
    }
    return true;
}

type charToType(char *type)
{
    if (strcmp(type, "int") == 0)
        return INTEGER;

    if (strcmp(type, "boolean") == 0)
        return BOOLEAN;

    if (strcmp(type, "char") == 0)
        return CHAR;

    if (strcmp(type, "string") == 0)
        return STRING;

    if (strcmp(type, "intp") == 0)
        return INT_PTR;

    if (strcmp(type, "charp") == 0)
        return CHAR_PTR;

    if (strcmp(type, "void") == 0)
        return VOID;

    return -1;
}

char *typeToChar(type t)
{
    if (t == INTEGER)
        return "int";

    if (t == CHAR)
        return "char";

    if (t == BOOLEAN)
        return "boolean";

    if (t == STRING)
        return "string";

    if (t == INT_PTR)
        return "intp";

    if (t == CHAR_PTR)
        return "charp";

    if (t == VOID)
        return "void";

    return "undefined";
}

type getType(Node *expr)
{

    printInOrder(expr, 0);

    if (strcmp(expr->left->data, "EXPR") == 0)
        return handleExpr(expr->left);

    if (strcmp(expr->left->data, "boolean") == 0)
        return BOOLEAN;

    if (strcmp(expr->left->data, "charp") == 0)
        return CHAR_PTR;

    if (strcmp(expr->left->data, "intp") == 0)
        return INT_PTR;

    if (strcmp(expr->left->data, "INTEGER") == 0)
        return INTEGER;

    if (strcmp(expr->left->data, "CHAR") == 0)
        return CHAR;

    if (strcmp(expr->left->data, "IDENT") == 0)
    {
        SymbEntry *entry = find(currentScope, expr->left->left->data);
        if (!entry)
        {
            fprintf(stderr, "Variable '%s' was not declared!\n", expr->left->left->data);
            return -1;
        }
        printf("TYPE of %s: %s\n", entry->name, entry->var_type);
        return charToType(entry->var_type);
    }

    if (strcmp(expr->left->data, "ARRAY ACCESS") == 0)
    {
        SymbEntry *entry = find(currentScope, expr->left->left->data);
        if (!entry)
            fprintf(stderr, "Variable '%s' is undefined!\n", expr->left->left->data);
        return CHAR;
    }

    else
        return handleExpr(expr);
}

bool validateIsInt(Node *ast)
{

    printInOrder(ast, 0);

    if (getType(ast->left) != INTEGER)
        fprintf(stderr, "Bad type found on '%s'\n", ast->data);
    if (getType(ast->right) != INTEGER)
    {
        fprintf(stderr, "Bad type found on '%s'\n", ast->data);
        return false;
    }
    return true;
}

bool validateIsPtr(Node *ast)
{
    if (getType(ast) == INT_PTR || getType(ast) == CHAR_PTR)
        return true;
    fprintf(stderr, "Bad type found on '^'\n");
    return false;
}

bool validateIsSimple(Node *left)
{
    if (getType(left) == INTEGER || getType(left) == CHAR)
        return true;
    fprintf(stderr, "Bad type found on '&'\n");
    return false;
}

type handleExpr(Node *ast)
{

    printInOrder(ast, 0);

    if (strcmp("EXPR", ast->data) == 0)
        return handleExpr(ast->left);
    // same on 2 sides
    else if (strcmp(ast->data, "==") == 0 && validateSameType(ast))
        return BOOLEAN;

    else if (strcmp(ast->data, "*") == 0 && validateIsInt(ast))
        return INTEGER;

    else if (strcmp(ast->data, "/") == 0 && validateIsInt(ast))
        return INTEGER;

    else if (strcmp(ast->data, "-") == 0 && validateIsInt(ast))
        return INTEGER;

    else if (strcmp(ast->data, "+") == 0 && validateIsInt(ast))
        return INTEGER;

    else if (strcmp(ast->data, "^") == 0 && validateIsPtr(ast->left))
        return getType(ast->left);

    else if (strcmp(ast->data, "&") == 0 && validateIsSimple(ast->left))
        return getType(ast->left);

    return -1;
}

bool handlePtrAssigment(Node *ast)
{
    if (getType(ast->left) == CHAR_PTR && getType(ast->right) == CHAR)
        return true;
    if (getType(ast->left) == INT_PTR && getType(ast->right) == INTEGER)
        return true;

    fprintf(stderr, "Bad type on '^='\n");
}

type typecheck(Node *ast)
{

    if (!ast)
        return -1;

    // Program start
    if (strcmp("PROGRAM", ast->data) == 0)
    {

        currentScope = malloc(sizeof(ScopeStack));
        ScopeStack *newScope = malloc(sizeof(ScopeStack));
        newScope->name = "GLOBAL";
        push(currentScope, newScope);
        typecheck(ast->left);
        typecheck(ast->right);

        if (!find(currentScope, "main"))
            fprintf(stderr, "'main' was not found!\n");
        pop(currentScope);
    }

    else if (strcmp("CODE", ast->data) == 0)
    {
        typecheck(ast->left);
        typecheck(ast->right);
    }

    else if (strcmp("FUNCTION", ast->data) == 0)
    {
        if (!searchScope(currentScope, ast->left->middle->data) && handleFunctionInfo(ast->left))
        {
            typecheck(ast->right);
            pop(currentScope); // pop function scope
        }
    }

    else if (strcmp("BLOCK", ast->data) == 0)
    {
        createScope(ast->data);
        typecheck(ast->left);
        typecheck(ast->right);
        pop(currentScope); // pop block scope
    }

    ////////////////////////////////////////////
    /* STOP REC */

    else if (strcmp("=", ast->data) == 0)
        validateSameType(ast);

    else if (strcmp("^=", ast->data) == 0)
        handlePtrAssigment(ast);

    else if (strcmp("IF", ast->data) == 0)
    {
        if (typecheck(ast->left) != BOOLEAN)
            fprintf(stderr, "Bad type found on 'if' statement\n");
        typecheck(ast->right);
    }

    else if (strcmp("VARIABLES DECLERATION", ast->data) == 0)
        handleVarDecl(ast);

    else if (strcmp("EXPR", ast->data) == 0)
        return handleExpr(ast->left);
}