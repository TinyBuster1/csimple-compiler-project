#!/bin/bash
rm lex.yy.c out y.output y.tab.c y.tab.h -f
lex scanner.l
byacc -d -v parser.y # debug and verbus (ads output file)
find . -type f -iname '*.c' -exec gcc -o out -Ly {}  +

# need to add tests