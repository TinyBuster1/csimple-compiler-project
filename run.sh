#!/bin/bash
lex scanner.l
yacc parser.y -d -v
gcc lex.yy.c y.tab.c -o out -Ly

# need to add tests