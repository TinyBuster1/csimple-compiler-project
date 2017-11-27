#!/bin/bash
lex scanner.l
yacc parser.y -y -d -ll -v
gcc lex.yy.c y.tab.c -o out

# need to add tests