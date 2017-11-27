#!/bin/bash
lex scanner.l
yacc parser.y -d -v # debug and verbus (ads output file)
gcc lex.yy.c y.tab.c -o out -Ly

# need to add tests