/* C code */
%option yylineno
%pointer
%{
    #include <stdlib.h>
    #include <string.h>
    /* #include "parser.h" */

    void yyerror(const char *);
%}

/* definitions */
%%
"boolean" printf("T_BOOLEAN");;
%%
/* subroutines */
int yywrap(void) {
    return 1;
}