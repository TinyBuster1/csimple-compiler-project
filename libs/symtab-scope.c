#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./symtab-scope.h"

bool searchScope(ScopeStack **currentScope, char *name)
{
    SymbEntry *symb_walker;
    symb_walker = (*currentScope)->table_ptr;
    while (symb_walker)
    {
        if (strcmp(name, symb_walker->name) == 0)
        {
            fprintf(stderr, "previous declaration of ‘%s’ was found in this scope!\n", name);
            return true;
        }
        symb_walker = symb_walker->nextEntry;
    }

    // searched all tables in all scopes, did not found, returns NULL
    return false;
}

/**
 * This function will only look for the right name
 * if it find a function with a name
 * it's up to the reciving function to validate integrity
 * 
 * return the symbol itself if found by name
 * return NULL if no result found
 * 
 */
SymbEntry *find(ScopeStack **currentScope, char *name)
{
    SymbEntry *symb_walker;
    ScopeStack *scope_walker = *currentScope;
    while (scope_walker)
    {
        symb_walker = scope_walker->table_ptr;
        while (symb_walker)
        {
            if (strcmp(name, symb_walker->name) == 0)
                return symb_walker;
            symb_walker = symb_walker->nextEntry;
        }
        scope_walker = scope_walker->next_scope;
    }
    // searched all tables in all scopes, did not found, returns NULL
    return NULL;
}

void printTable(char *SCOPE_NAME, SymbEntry *head)
{
    if (!head)
        return;

    SymbEntry *walker = head;
    printf("\n---------\nSYMBOL TABLE OF SCOPE: %s\n", SCOPE_NAME);
    printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "NAME", "|", "TYPE", "|", "DATA", "|", "POINTS TO");
    do
    {
        if (walker->nextEntry)
            printf("%-15s%-15s%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker->var_type, "|", walker->data, "|", walker->nextEntry->name);
        else
        {
            printf("%-15s%-15s%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker->var_type, "|", walker->data, "|", "NULL");
        }
        walker = walker->nextEntry;
    } while (walker);
    printf("---------\n\n");
}

/**
 * inserts a new symbol to the current top scope in the stack
 */
void insert(ScopeStack *currentScope, SymbEntry *newEntry)
{
    // printf("INSERT: '%s' into symbol table of '%s'\n", newEntry->name, currentScope->name);
    SymbEntry *walker = currentScope->table_ptr;
    // if symbol table is empty
    if (!walker)
        currentScope->table_ptr = newEntry;
    // printf("\n####  %s  ####\n", newEntry->name);

    else
    {
        while (walker->nextEntry)
            walker = walker->nextEntry;
        walker->nextEntry = newEntry;
    }
    // printTable(currentScope->name, currentScope->table_ptr);
}

void printStack(ScopeStack **currentScope)
{
    ScopeStack *walker = *currentScope;
    printf("---------\n\nCurrent Scopes Stack\n");
    printf("%-15s%-15s%-15s%-15s%-15s\n", "NAME", "|", "ADDRESS", "|", "POINTS TO\n");

    if (!(*currentScope))
        return;

    do
    {
        if (walker->next_scope)
            printf("%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker, "|", walker->next_scope->name);
        else
            printf("%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker, "|", "NULL");
        walker = walker->next_scope;
    } while (walker);
    printf("---------\n\n");
}

/** 
 * inserts a new scope to the stack
 */
void push(ScopeStack **currentScope, ScopeStack *newScope)
{
    // printf("PUSH: '%s'\n", newScope->name);
    // first, make the new scope point to the current top scope
    newScope->next_scope = *currentScope;
    // now move the currentScope top the new scope
    *currentScope = newScope;
    // printStack(currentScope);
}

void pop(ScopeStack **currentScope)
{
    // printf("POP: '%s'\n", (*currentScope)->name);
    // free the symbole table of the current top
    free((*currentScope)->table_ptr);
    // save a tmp to free after moving the head 1 down
    ScopeStack *ptr = *currentScope;
    // move the head 1 down
    *currentScope = (*currentScope)->next_scope;
    // free the poped head
    // free(ptr);
    // printStack(currentScope);
}