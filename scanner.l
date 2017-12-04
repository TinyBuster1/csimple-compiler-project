/* pre defs */
L   [a-zA-Z]
D   [0-9]
/* C code */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
 	int yyerror(char *);
    extern int yyval, yylineno;
%}

/* definitions */
%%
    /* tokens */
    /* COMMENTS */
"//".*           { /* DO NOTHING */ }
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]       { /* DO NOTHING */ }               
    /* TYPES */
"void"|"boolean"|"char"|"string"|"int"|"intp"|"charp" { yylval.string = strdup(yytext); return TYPE; }
"null"   { yylval.string = strdup(yytext); return NULL_TYPE; }
    /* CONDITIONS */
"if"      return IF;
"else"    return ELSE;
    /* LOOPS */
"while"   return WHILE;
"do"      return DOWHILE;
"for"     return FOR;
    /* KEYWORDS */
"return"  return RETURN;

    /* OPERATORS */
"&&"    return AND;
"/"     return DIV;
"="     return ASS;
"=="    return EQUAL;
">"     return GT;
">="    return GTE;
"<"     return LT;
"<="    return LTE;
"-"     return MINUS;
"!"     return NOT;
"!="    return NOTEQUAL;
"||"    return OR;
"+"     return PLUS;
"*"     return MUL; 
"&"     return ADDRESS;
"^"     return CONTENT;

    /* Literal Lexemes */

"true"             return BOOL_TRUE;
"false"            return BOOL_FALSE;
'.'                { yylval.string = strdup(yytext); return CHAR_LITERAL; } 
{L}({L}|{D}|_)*    { yylval.string = strdup(yytext); return IDENTIFIER; }
\"([^\\\"]|\\.)*\" { yylval.string = strdup(yytext); return STRING_LITERAL; }

    /* ints types */

(0|[1-9][0-9]*|0[xX][0-9a-fA-F]+|0[0-7]+|[01]+b)  { yylval.string = strdup(yytext); return INTEGER; }

    /* others */

";" return SEMICOLON; 
":" return COLON; 
"," return COMMA; 
"{" return O_CURL; 
"}" return C_CURL; 
"(" return O_PAREN; 
")" return C_PAREN; 
"|" return VERT_LINE; 
"]" return C_BRACK; 
"[" return O_BRACK; 


[ \t\f\v]               ; /* ignore blank spaces */
[\n] 			         { yylineno = yylineno + 1; }
.   yyerror("invalid character");
%%
/* subroutines */
int yywrap(void) {
    return 1;
}