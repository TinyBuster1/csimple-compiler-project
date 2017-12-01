#!/bin/bash
rm lex.yy.c out y.output y.tab.c y.tab.h -f
lex scanner.l
yacc parser.y -d -v # debug and verbus (ads output file)
gcc lex.yy.c y.tab.c -o out -Ly

# need to add tests