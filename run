#!/bin/bash
lex scanner.l
yacc parser.y -ll
gcc lex.yy.c y.tab.c -o out