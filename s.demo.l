/* pre-defenictions */
D			[0-9]
L			[a-zA-Z]
H			[A-F0-9]
OCT         [0-7]

/* C code */
%{
    #include <stdlib.h>
%}

/* definitions */
%%
    /* tokens */
    
    /* TYPES */
"boolean" printf("T_BOOLEAN ");
"char"    printf("T_CHAR ");
"void"    printf("T_VOID ");
"int"     printf("T_INTEGER ");
"string"  printf("T_STRING ");
"intp"    printf("T_P_INT ");
"charp"   printf("T_P_CHAR ");
"null"    printf("T_NULL ");

    /* CONDITIONS */
"if"      printf("C_IF ");
"else"    printf("C_ELSE ");
    /* LOOPS */
"while"   printf("L_WHILE ");
"do"      printf("L_DOWHILE ");
"for"     printf("L_FOR ");
    /* KEYWORDS */
"return" printf("K_RETURN ");

    /* OPERATORS */
"&&"    printf("OP_AND");
"/"     printf("OP_DIVIDE");
"="     printf("OP_ASSIGMENT ");
"=="    printf("OP_EQUAL ");
">"     printf("OP_GREATERT ");
">="    printf("OP_GREATERTE ");
"<"     printf("OP_LOWERT ");
"<="    printf("OP_LOWERTE ");
"-"     printf("OP_MINUS ");
"!"     printf("OP_NOT ");
"!="    printf("OP_NOTEQUAL ");
"||"    printf("OP_OR ");
"+"     printf("OP_PLUS ");
"*"     printf("OP_MUL "); 
"&"     printf("OP_ADDRESS ");
"^"     printf("OP_CONTENT ");

    /* Literal Lexemes */

"true"                      printf("LT_TRUE ");
"false"                     printf("T_FASLE ");
'.'                         printf("LT_CHAR "); 
{L}({L}|{D})*               printf("LT_IDEN ");
\"([^\\\"]|\\.)*\" 	        printf("STRING_LITERAL ");

    /* ints types */

0{D}+                       printf("LT_INTEGER ");
0(x|X){H}+                  printf("LT_HEX ");
0{OCT}*                     printf("LT_OCT ");
(0|1)(0|1)*b                printf("LT_BIN ");

    /* others */

";" printf("T_SEMICOLON "); 
":" printf("T_COLON "); 
"," printf("T_COMMA "); 
"{" printf("T_OPENBRACKET "); 
"}" printf("T_CLOSEBRACKET "); 
"(" printf("T_OPENPAREN "); 
")" printf("T_CLOSEPAREN "); 
"|" printf("T_VERT_BAR "); 
"]" printf("T_R_BRACKET "); 
"[" printf("T_L_BRACKET "); 

    /* TODO: move back into [ \t\v\f] */
"\n"                printf("\n"); 

[ \t\v\f]               /* ignore blank spaces */

.  printf(" ## ERROR ## ");
%%
/* subroutines */