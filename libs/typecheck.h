#ifndef TYPECHECK_H_
#define TYPECHECK_H_
#include "./ast.h"
#include "./symtab-scope.h"
type typecheck(Node *);
type handleExpr(Node *);
type getType(Node *);
bool validateSameType(Node *);
type charToType(char *);
char *typeToChar(type);
#endif