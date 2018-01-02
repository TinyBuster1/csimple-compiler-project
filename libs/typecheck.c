
#include <stdlib.h>
#include <string.h>
#include "./typecheck.h"
#include "./ast.h"

// single main function flag
bool MAIN_FLAG = false;
/*****************************/
// CREATE THE SCOPES STACK LIST
ScopeStack **currentScope;
SymbEntry *currentFunction;

void printArgsList(args *list)
{
    while (list)
    {
        printf("%s %s --> ", list->name, list->type);
        list = list->next;
    }
    printf("NULL\n");
}

type handleEmptyFunctionCall(Node *func, int line)
{
    SymbEntry *entry = find(currentScope, func->data);
    if (!entry)
    {
        fprintf(stderr, "line: %d: Function '%s' was never declered!\n", line, func->data);
        return -1;
    }
    return charToType(entry->data->r_value);
}

args *getParamsTypes(Node *ast)
{

    if (!ast)
        return NULL;

    args *argument = malloc(sizeof(args));
    if (strcmp("INPUT PARAMS", ast->data) == 0)
    {
        argument->type = typeToChar(getType(ast->right));
        argument->next = getParamsTypes(ast->left);
    }
    else
        argument->type = typeToChar(getType(ast));

    return argument;
}

int countLen(args *args)
{
    int count = 0;
    while (args)
    {
        count++;
        args = args->next;
    }

    return count;
}

type handleFunctionCall(Node *ast)
{
    SymbEntry *function = find(currentScope, ast->left->left->data);
    if (!function)
    {
        fprintf(stderr, "line: %d: Function '%s' was never declered!\n", ast->line, ast->data);
        return -1;
    }

    args *runner = function->data->args;
    args *params = getParamsTypes(ast->right);
    if (countLen(runner) != countLen(params))
    {
        fprintf(stderr, "line: %d: args count is unmatched \n", ast->line);
        return charToType(function->data->r_value);
    }

    while (runner && params)
    {
        if (charToType(params->type) != charToType(runner->type))
            fprintf(stderr, "line: %d: type of '%s' is unmatched \n", ast->line, runner->type);

        runner = runner->next;
        params = params->next;
    }

    return charToType(function->data->r_value);
}

void createScope(char *name)
{
    // printf("##SCOPE OF: %s\n", (*currentScope)->name);
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
        if (strcmp("ARRAY ACCESS", ids->right->data) == 0)
        {
            if (strcmp(type, "string") != 0)
                fprintf(stderr, "line: %d: index brackets can only be apllied on 'string' type variable\n", ids->line);
            else if (getType(ids->right->right) != INTEGER)
                fprintf(stderr, "line: %d: expression in brackets must type of 'integer'\n", ids->line);
            else
                insertVar(type, ids->right->left->left->data);
        }
        else
            insertVar(type, ids->right->left->data);
    }
    else if (strcmp("ARRAY ACCESS", ids->left->data) == 0)
    {
        if (strcmp(type, "string") != 0)
            fprintf(stderr, "line: %d: index brackets can only be apllied on 'string' type variable\n", ids->line);
        else if (getType(ids->left->right) != INTEGER)
            fprintf(stderr, "line: %d: expression in brackets must type of 'integer'\n", ids->line);
        else
            insertVar(type, ids->left->left->left->data);

        if (strcmp(type, "string") != 0)
            fprintf(stderr, "line: %d: index brackets can only be apllied on 'string' type variable\n", ids->line);
        else if (getType(ids->right->right) != INTEGER)
            fprintf(stderr, "line: %d: expression in brackets must type of 'integer'\n", ids->line);
        else
            insertVar(type, ids->right->left->left->data);
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
        fprintf(stderr, "line: %d: varialbes of type 'void' can not be declered\n", ast->line);
        return;
    }
    Node *ids = ast->right;
    if (strcmp("MULTIPLE IDENTIFIERS", ids->data) == 0)
    {
        handleMultiVarsDecl(type, ids);
    }

    else if (strcmp("ARRAY ACCESS", ids->data) == 0)
    {
        if (strcmp(type, "string") != 0)
            fprintf(stderr, "line: %d: index brackets can only be apllied on 'string' type variable\n", ast->line);
        else if (getType(ids->right) != INTEGER)
            fprintf(stderr, "line: %d: expression in brackets must type of 'integer'\n", ast->line);
        else
        {
            ids = ids->left;
            insertVar(type, ids->left->data);
        }
    }
    else
        insertVar(type, ids->left->data);
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
        argument->name = ast->right->left->data;
    }
    return argument;
}

bool validateMain(function *data, int line)
{
    if (MAIN_FLAG)
    {
        fprintf(stderr, "line: %d: multiple definition of `main' was found\n", line);
        return false;
    }
    // now verify main is defind in global scope
    if ((*currentScope)->next_scope)
    {
        fprintf(stderr, "line: %d: 'main' was defind in inner scope\n", line);
        return false;
    }
    // check that args is empty
    if (data->args)
    {
        fprintf(stderr, "line: %d: 'main' have arguments\n", line);
        return false;
    }
    MAIN_FLAG = true;
    return true;
}

SymbEntry *handleFunctionInfo(Node *ast)
{
    char *name = ast->middle->left->data;

    if (searchScope(currentScope, name))
    {
        return NULL;
    }
    function *data = calloc(1, sizeof(function));
    data->r_value = ast->left->data;
    data->args = getFuncArgs(ast->right);

    if (strcmp("main", name) == 0 && !validateMain(data, ast->line))
        return NULL;

    // creating scope for function args
    createScope(ast->data);
    // printf("SETTING NEW FUNCTION PTR: %s\n", name);

    // push function arguments into args scope
    args *runner = data->args;
    while (runner)
    {
        insertVar(runner->type, runner->name);
        runner = runner->next;
    }

    // push function data into upper scope for future search and validation
    // we do that only after we validate the function block!
    SymbEntry *newEntry = calloc(1, sizeof(SymbEntry));
    newEntry->name = name;
    newEntry->data = data;

    insert((*currentScope)->next_scope, newEntry);
    currentFunction = newEntry;

    return newEntry;
}
bool validateIsString(Node *ast)
{

    if (getType(ast->left) != STRING)
    {
        fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
        return false;
    }
    if (!ast->right)
        return true;

    if (getType(ast->right) != STRING)
    {
        fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
        return false;
    }
    return true;
}

bool validateSameType(Node *ast)
{

    if (getType(ast->left) != getType(ast->right))
    {

        fprintf(stderr, "line: %d: Different types were found on '%s'\n", ast->line, ast->data);
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

    if (strcmp(expr->left->data, "STRING") == 0)
        return STRING;

    if (strcmp(expr->left->data, "CHAR") == 0)
        return CHAR;

    if (strcmp(expr->left->data, "FUNCTION CALL NO PARAMS") == 0)
        return handleEmptyFunctionCall(expr->left->left->left, expr->left->left->left->line);

    if (strcmp("FUNCTION CALL", expr->left->data) == 0)
        return handleFunctionCall(expr->left);

    if (strcmp(expr->left->data, "IDENT") == 0)
    {
        SymbEntry *entry = find(currentScope, expr->left->left->data);
        if (!entry)
        {
            fprintf(stderr, "line: %d: Variable '%s' was not declared!\n", expr->left->left->line, expr->left->left->data);
            return -1;
        }
        // printf("TYPE of %s: %s\n", entry->name, entry->var_type);
        return charToType(entry->var_type);
    }

    if (strcmp(expr->left->data, "ARRAY ACCESS") == 0)
    {

        SymbEntry *entry = find(currentScope, expr->left->left->left->data);
        if (!entry)
            fprintf(stderr, "line: %d: Variable '%s' is undefined!\n", expr->left->left->line, expr->left->left->data);
        return CHAR;
    }

    else
        return handleExpr(expr->left);
}

bool validateIsInt(Node *ast)
{

    if (getType(ast->left) != INTEGER)
    {
        fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
        return false;
    }
    if (!ast->right)
        return true;

    if (getType(ast->right) != INTEGER)
    {
        fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
        return false;
    }
    return true;
}

bool validateIsPtr(Node *ast)
{
    if (getType(ast) == INT_PTR || getType(ast) == CHAR_PTR)
        return true;
    fprintf(stderr, "line: %d: Bad type found on '^'\n", ast->line);
    return false;
}

bool validateIsSimple(Node *left)
{
    if (getType(left) == INTEGER || getType(left) == CHAR)
        return true;
    fprintf(stderr, "line: %d: Bad type found on '&'\n", left->line);
    return false;
}

bool validateABS(Node *ast)
{
    if (getType(ast->left) != INTEGER && getType(ast->left) != STRING)
    {
        fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
        return false;
    }
    if (!ast->right)
        return true;

    if (getType(ast->right) != INTEGER && getType(ast->right) != STRING)
    {
        fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
        return false;
    }
    return true;
}
bool validateIsBoolean(Node *ast)
{
    if (ast->right)
    {
        if (getType(ast->left) != BOOLEAN || getType(ast->right) != BOOLEAN)
        {
            fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
            return false;
        }
    }

    else
    {
        if (getType(ast) != BOOLEAN)
        {
            fprintf(stderr, "line: %d: Bad type found on '%s'\n", ast->line, ast->data);
            return false;
        }
    }

    return true;
}

type handleExpr(Node *ast)
{

    if (strcmp("EXPR", ast->data) == 0)
        return getType(ast);
    // same on 2 sides
    if (strcmp(ast->data, "==") == 0 && validateSameType(ast))
        return BOOLEAN;

    if (strcmp(ast->data, "!=") == 0 && validateSameType(ast))
        return BOOLEAN;

    if (strcmp(ast->data, "*") == 0 && validateIsInt(ast))
        return INTEGER;

    if (strcmp(ast->data, "/") == 0 && validateIsInt(ast))
        return INTEGER;

    if (strcmp(ast->data, "-") == 0 && validateIsInt(ast))
        return INTEGER;

    if (strcmp(ast->data, "+") == 0 && validateIsInt(ast))
        return INTEGER;

    if (strcmp(ast->data, "ABS") == 0 && validateABS(ast))
    {
        return INTEGER;
    }

    if (strcmp(ast->data, "^") == 0 && validateIsPtr(ast->left))
    {
        type t;
        t = getType(ast->left);
        if (t == INT_PTR)
            return INTEGER;
        else
            return CHAR;
    }

    if (strcmp(ast->data, "&") == 0 && validateIsSimple(ast->left))
    {
        type t;
        t = getType(ast->left);
        if (t == INTEGER)
            return INT_PTR;
        else
            return CHAR_PTR;
    }

    if (strcmp(ast->data, "!") == 0 && validateIsBoolean(ast->left))
        return BOOLEAN;

    if (strcmp(ast->data, "&&") == 0 && validateIsBoolean(ast))
        return BOOLEAN;

    if (strcmp(ast->data, "||") == 0 && validateIsBoolean(ast))
        return BOOLEAN;

    if (strcmp(ast->data, ">") == 0 && validateIsInt(ast))
        return BOOLEAN;

    if (strcmp(ast->data, ">=") == 0 && validateIsInt(ast))
        return BOOLEAN;

    if (strcmp(ast->data, "<") == 0 && validateIsInt(ast))
        return BOOLEAN;

    if (strcmp(ast->data, "<=") == 0 && validateIsInt(ast))
        return BOOLEAN;

    if (strcmp(ast->data, "<=") == 0 && validateIsInt(ast))
        return BOOLEAN;

    return -1;
}

bool handlePtrAssigment(Node *ast)
{
    if (getType(ast->left) == CHAR_PTR && getType(ast->right) == CHAR)
        return true;
    if (getType(ast->left) == INT_PTR && getType(ast->right) == INTEGER)
        return true;

    fprintf(stderr, "line: %d: Bad type on '^='\n", ast->line);
}

bool checkReturn(Node *ast)
{
    // ignore new functions nodes
    if (strcmp("RETURN", ast->data) == 0)
        return true;

    if (strcmp("RETURN VOID", ast->data) == 0)
        return true;

    if (ast->left && strcmp("FUNCTION", ast->left->data) != 0 && checkReturn(ast->left))
        return true;
    if (ast->middle && strcmp("function", ast->middle->data) != 0 && checkReturn(ast->middle))
        return true;
    if (ast->right && strcmp("FUNCTION", ast->right->data) != 0 && checkReturn(ast->right))
        return true;

    // no return on void functions
    if (strcmp("void", currentFunction->data->r_value) == 0)
        return true;

    return false;
}

void handleFunctionBlock(Node *block, SymbEntry *func_data)
{
    createScope("FUNCTION BLOCK");
    typecheck(block->left);
    typecheck(block->right);
    pop(currentScope); // pop block scope
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
            fprintf(stderr, "line: %d: 'main' was not found!\n", ast->line);
        pop(currentScope);
    }

    else if (strcmp("RETURN", ast->data) == 0)
    {
        if (!currentFunction)
            fprintf(stderr, "line: %d: 'return' unexpected\n", ast->line);
        else
        {

            if (getType(ast->left) != charToType(currentFunction->data->r_value))
                fprintf(stderr, "line: %d: Expected 'return' type to be %s\n", ast->line, currentFunction->data->r_value);
        }
    }

    else if (strcmp("RETURN VOID", ast->data) == 0)
    {
        if (!currentFunction)
            fprintf(stderr, "line: %d: 'return' unexpected\n", ast->line);
        else
        {

            if (VOID != charToType(currentFunction->data->r_value))
                fprintf(stderr, "line: %d: Expected 'return' type to be %s\n", ast->line, currentFunction->data->r_value);
        }
    }

    else if (strcmp("CODE", ast->data) == 0)
    {
        typecheck(ast->left);
        typecheck(ast->right);
    }

    else if (strcmp("FUNCTION", ast->data) == 0)
    {
        SymbEntry *function_data = handleFunctionInfo(ast->left);
        if (!searchScope(currentScope, ast->left->middle->data) && function_data)
        {
            if (!checkReturn(ast->right))
                fprintf(stderr, "line: %d: Return was expected\n", ast->line);
            handleFunctionBlock(ast->right, function_data);
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
            fprintf(stderr, "line: %d: Bad type found on 'if' statement\n", ast->line);
        typecheck(ast->right);
    }

    else if (strcmp("WHILE LOOP", ast->data) == 0)
    {
        if (typecheck(ast->left) != BOOLEAN)
            fprintf(stderr, "line: %d: Bad type found on 'while' statement\n", ast->line);
        typecheck(ast->right);
    }

    else if (strcmp("DO WHILE", ast->data) == 0)
    {
        if (handleExpr(ast->right) != BOOLEAN)
            fprintf(stderr, "line: %d: Bad type found on 'while' statement\n", ast->line);
        typecheck(ast->left);
    }

    else if (strcmp("FOR LOOP", ast->data) == 0)
    {
        Node *fordata = ast->left;
        typecheck(fordata->left);
        if (handleExpr(fordata->middle) != BOOLEAN)
            fprintf(stderr, "line: %d: Bad type found on 'for' statement\n", ast->line);
        typecheck(ast->right);
    }

    else if (strcmp("VARIABLES DECLERATION", ast->data) == 0)
        handleVarDecl(ast);

    else if (strcmp("FUNCTION CALL NO PARAMS", ast->data) == 0)
        return handleEmptyFunctionCall(ast->left->left, ast->left->left->line);

    else if (strcmp("FUNCTION CALL", ast->data) == 0)
        return handleFunctionCall(ast);

    else if (strcmp("EXPR", ast->data) == 0)
        return handleExpr(ast->left);
}
