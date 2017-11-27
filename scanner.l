/* pre-defenictions */
D			[0-9]
L			[a-zA-Z]
H			[A-F0-9]

/* C code */
%{
    #include <stdio.h>
    #include "y.tab.h"
 	void yyerror(char *);
    extern int yylval;
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
{L}({L}|{D}|_)*    return LT_IDEN;
\"([^\\\"]|\\.)*\" return STRING_LITERAL;

    /* ints types */

0(x|X){H}+    return LT_HEX;
0[1-7][0-7]*  return LT_OCT;
0|{D}*        {yylval = atoi(yytext);return LT_INTEGER;}
1(0|1)+b      return LT_BIN;

    /* others */

";" return T_SEMICOLON; 
":" return T_COLON; 
"," return T_COMMA; 
"{" return T_OPENBRACKET; 
"}" return T_CLOSEBRACKET; 
"(" return T_OPENPAREN; 
")" return T_CLOSEPAREN; 
"|" return T_VERT_BAR; 
"]" return T_R_BRACKET; 
"[" return T_L_BRACKET; 


[ \t\r\n]               ; /* ignore blank spaces */

.   yyerror(yytext);
%%
/* subroutines */
int yywrap(void) {
    return 1;
}