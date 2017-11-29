/* pre defs */
L   [a-zA-Z]
D   [0-9]
/* C code */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "y.tab.h"
 	void yyerror(char *);
    extern int yyval, yylineno;
    int getIntValue(char* text, int type);
%}

/* definitions */
%%
    /* tokens */
    /* COMMENTS */
\/\/.*\n$ {}
\/\*(.|\n)*\*\/ {}
    /* TYPES */
"boolean" return BOOLEAN;
"char"    return CHAR;
"void"    return VOID;
"int"     return INT;
"string"  return STRING;
"intp"    return PINT;
"charp"   return PCHAR;
"null"    return PNULL;

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
"/"     return DIVIDE;
"="     return ASSIGMENT;
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
'.'                return CHAR; 
{L}({L}|{D}|_)*    return IDEN;
\"([^\\\"]|\\.)*\" return STRING_LITERAL;

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


[ \t\r]               ; /* ignore blank spaces */
[\n] 			         { yylineno = yylineno + 1; }
.   yyerror("invalid character");
%%
/* subroutines */
int yywrap(void) {
    return 1;
}

int getIntValue(char* text, int type) {
	if(strlen(text) > 10) {
		yyerror("integer too long");
		exit(0);
	}
	int value = (int) strtol(yytext, 0, type);
	return value;
}

/* TODO: add logical expr */
/* TODO: add unary expr */
/* TODO: not sure about yacc nubers handling */