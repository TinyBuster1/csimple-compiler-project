%{
 	#include <stdio.h>
 	int yylex(void);
 	void yyerror(char *);
%}

/* TOKENS */
/* TYPES */
%token T_BOOLEAN T_CHAR T_VOID T_INTEGER T_STRING T_P_INT T_P_CHAR T_NULL
/* OPERATORS */
%token OP_AND OP_DIVIDE OP_ASSIGMENT OP_EQUAL OP_GT OP_GTE OP_LT OP_LTE OP_MINUS OP_NOT OP_NOTEQUAL OP_OR OP_PLUS OP_MUL OP_ADDRESS OP_CONTENT
/* CONDITIONS */
%token C_IF C_ELSE L_WHILE L_DOWHILE L_FOR K_RETURN
/* globals */
%token LT_TRUE T_FASLE LT_CHAR LT_IDEN STRING_LITERAL
/* ints types */
%token LT_HEX LT_OCT LT_INTEGER LT_BIN
/* others */
%token T_SEMICOLON T_COLON T_COMMA T_OPENBRACKET T_CLOSEBRACKET T_OPENPAREN T_CLOSEPAREN T_VERT_BAR T_R_BRACKET T_L_BRACKET
/* definitions */
%start var_dec
%%

var_dec
	: LT_INTEGER LT_IDEN T_SEMICOLON
	;

%%
/* subroutines */
#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
}

int main(void) {
	yyparse();
	return 0;
}