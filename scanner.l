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
"boolean" return T_BOOLEAN;
"char"    return T_CHAR;
"void"    return T_VOID;
"int"     return T_INTEGER;
"string"  return T_STRING;
"intp"    return T_P_INT;
"charp"   return T_P_CHAR;
"null"    return T_NULL;

    /* CONDITIONS */
"if"      return C_IF;
"else"    return C_ELSE;
    /* LOOPS */
"while"   return L_WHILE;
"do"      return L_DOWHILE;
"for"     return L_FOR;
    /* KEYWORDS */
"return"  return K_RETURN;

    /* OPERATORS */
"&&"    return OP_AND;
"/"     return OP_DIVIDE;
"="     return OP_ASSIGMENT;
"=="    return OP_EQUAL;
">"     return OP_GT;
">="    return OP_GTE;
"<"     return OP_LT;
"<="    return OP_LTE;
"-"     return OP_MINUS;
"!"     return OP_NOT;
"!="    return OP_NOTEQUAL;
"||"    return OP_OR;
"+"     return OP_PLUS;
"*"     return OP_MUL; 
"&"     return OP_ADDRESS;
"^"     return OP_CONTENT;

    /* Literal Lexemes */

"true"             return LT_TRUE;
"false"            return T_FASLE;
'.'                return LT_CHAR; 
{L}({L}|{D}|_)*    return IDEN;
\"([^\\\"]|\\.)*\" return STRING_LITERAL;

    /* ints types */

(0|[1-9][0-9]*|0[xX][0-9a-fA-F]+|0[0-7]+|[01]+b)  { yylval.string = strdup(yytext); return LT_INTEGER; }

    /* others */

";" return T_SEMICOLON; 
":" return T_COLON; 
"," return T_COMMA; 
"{" return CURL_O; 
"}" return CURL_R; 
"(" return T_OPENPAREN; 
")" return T_CLOSEPAREN; 
"|" return T_VERT_BAR; 
"]" return T_R_BRACKET; 
"[" return T_L_BRACKET; 


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