#!/bin/bash
lex scanner.l
yacc parser.y -ll -v
gcc lex.yy.c y.tab.c -o out