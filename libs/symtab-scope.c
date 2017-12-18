#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./symtab-scope.h"

/**
 * This function will only look for the right name
 * if it find a function with a name
 * it's up to the reciving function to validate integrity
 * 
 * return the symbol itself if found by name
 * return NULL if no result found
 * 
 */
SymbEntry *search(ScopeStack **SCOPES_STACK_TOP_PTR, char *name)
{
    SymbEntry *symb_walker;
    ScopeStack *scope_walker = *SCOPES_STACK_TOP_PTR;
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
    SymbEntry *walker = head;
    printf("\n---------\nSYMBOL TABLE OF SCOPE: %s\n", SCOPE_NAME);
    printf("%-15s%-15s%-15s%-15s%-15s\n", "NAME", "|", "ADDRESS", "|", "POINTS TO");
    do
    {
        if (walker->nextEntry)
            printf("%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker, "|", walker->nextEntry->name);
        else
            printf("%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker, "|", "NULL");
        walker = walker->nextEntry;
    } while (walker);
    printf("---------\n\n");
}

/**
 * inserts a new symbol to the current top scope in the stack
 */
void insert(ScopeStack *SCOPES_STACK_TOP_PTR, SymbEntry *newEntry)
{
    printf("\nINSERT: %s into SCOPE: %s\n", newEntry->name, SCOPES_STACK_TOP_PTR->name);
    SymbEntry *walker = SCOPES_STACK_TOP_PTR->table_ptr;
    // if symbol table is empty
    if (!walker)
        SCOPES_STACK_TOP_PTR->table_ptr = newEntry;
    // printf("\n####  %s  ####\n", newEntry->name);

    else
    {
        while (walker->nextEntry)
            walker = walker->nextEntry;
        walker->nextEntry = newEntry;
    }
    printTable(SCOPES_STACK_TOP_PTR->name, SCOPES_STACK_TOP_PTR->table_ptr);
}

void printStack(ScopeStack **SCOPES_STACK_TOP_PTR)
{
    ScopeStack *walker = *SCOPES_STACK_TOP_PTR;

    printf("---------\nCurrent Scopes Stack\n");
    printf("%-15s%-15s%-15s%-15s%-15s\n", "NAME", "|", "ADDRESS", "|", "POINTS TO");
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
void push(ScopeStack **SCOPES_STACK_TOP_PTR, ScopeStack *new_scope)
{
    printf("\nPUSH: %s\n", new_scope->name);
    // first, make the new scope point to the current top scope
    new_scope->next_scope = *SCOPES_STACK_TOP_PTR;
    // now move the SCOPES_STACK_TOP_PTR top the new scope
    *SCOPES_STACK_TOP_PTR = new_scope;
    printStack(SCOPES_STACK_TOP_PTR);
}

void pop(ScopeStack **SCOPES_STACK_TOP_PTR)
{
    printf("\nPOP: %s\n", (*SCOPES_STACK_TOP_PTR)->name);
    // free the symbole table of the current top
    // free((*SCOPES_STACK_TOP_PTR)->table_ptr);
    // save a tmp to free after moving the head 1 down
    ScopeStack *ptr = *SCOPES_STACK_TOP_PTR;
    // move the head 1 down
    *SCOPES_STACK_TOP_PTR = (*SCOPES_STACK_TOP_PTR)->next_scope;
    // free the poped head
    // free(ptr);
    printStack(SCOPES_STACK_TOP_PTR);
}