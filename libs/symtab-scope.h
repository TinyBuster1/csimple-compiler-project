#ifndef SYMB_TAB_H_
#define SYMB_TAB_H_

typedef enum bool {
    false,
    true
} bool;

typedef struct args
{
    char *name;
    char *type;        // arg type
    struct args *next; // next arg in list
} args;

typedef struct function
{
    struct args *args; // function args
    char *r_value;     // return type
} function;

typedef struct SymbEntry
{
    char *name;                  // symbol name
    char *var_type;              // if it's a var
    struct function *data;       // if it's a func
    struct SymbEntry *nextEntry; // next Entry in "table"
} SymbEntry;

typedef struct ScopeStack
{
    char *name;
    struct SymbEntry *table_ptr;
    struct ScopeStack *next_scope;
} ScopeStack;

void pop(ScopeStack **);
void push(ScopeStack **, ScopeStack *);
void printStack(ScopeStack **);

SymbEntry *find(ScopeStack **, char *);
bool searchScope(ScopeStack **, char *);
void insert(ScopeStack *, SymbEntry *);
void printTable(char *, SymbEntry *);
#endif