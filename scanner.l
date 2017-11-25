/* C code */
%option yylineno
%pointer
%{
    #include <stdlib.h>
    #include <string.h>
    #include "parser.h"

    void yyerror(const char *);
%}

/* definitions */
%%
    /* tokens */
    
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

"true"                  return LT_TRUE;
"false"                 return LT_FASLE;
'[A-Za-z]'              return LT_CHAR; 
0(x|X)[A-F0-9][A-F0-9]* return LT_INTEGER_HEX;
0[0-7]*                 return LT_INTEGER_OCT;
(0|1)(0|1)*b            return LT_INTEGER_BIN;

[0-9][0-9]*             { yylval.base_int = atoi(yytext); return LT_INTEGER_INTEGER; }
[a-zA-Z][a-zA-Z0-9_]*   { yylval.base_char_ptr = yytext; return LT_IDEN; }
\"[a-zA-Z0-9_\\]*\"     { yylval.base_char_ptr = yytext; return LT_STRING; }

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

.   yyerror((char *) "invalid character");
%%
/* subroutines */
int yywrap(void) {
    return 1;
}