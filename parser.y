%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct node
	{ char * token;
	  struct node * left;
	  struct node * right;
	} node;

node * make_node ( char * token , node * left, node * right);
void print_tree ( node * tree);
void yyerror(char * s);
int yylex(void);
#define YYDEBUG 1
#define YYSTYPE struct node *
extern int yylineon;
extern char * yytext;
%}
/* Enables verbose error messages */
%error-verbose

/* TOKENS */
/* TYPES */
%token T_BOOLEAN T_CHAR T_VOID T_INTEGER T_STRING T_P_INT T_P_CHAR T_NULL
/* OPERATORS */
%left OP_MINUS OP_PLUS 
%left OP_MUL OP_DIVIDE
%left OP_AND OP_ASSIGMENT OP_EQUAL OP_GT OP_GTE OP_LT OP_LTE OP_NOT OP_NOTEQUAL OP_OR OP_ADDRESS OP_CONTENT
/* CONDITIONS */
%token C_IF C_ELSE L_WHILE L_DOWHILE L_FOR K_RETURN
/* globals */
%token LT_TRUE T_FASLE LT_CHAR LT_IDEN STRING_LITERAL
/* ints types */
%token LT_HEX LT_OCT LT_INTEGER LT_BIN
/* others */
%token T_SEMICOLON T_COLON T_COMMA T_OPENBRACKET T_CLOSEBRACKET T_OPENPAREN T_CLOSEPAREN T_VERT_BAR T_R_BRACKET T_L_BRACKET
/* definitions */
%%
START	: { exit(0); }
		;

%%
/* subroutines */
node * make_node(char * token, node *left, node *right){
	node * newnode 	= (node*) malloc (sizeof(node));
	char * newstr 	= (char*) malloc (sizeof(token)+1);
	strcpy (newstr,token);
	newnode->left 	=	left;
	newnode->right 	=	right;
	newnode->token	=	newstr;
	
	return newnode;
}
void print_tree(node * tree){
	printf("%s\n",tree->token);
	if(tree->left){ 
		print_tree(tree->left);
	}
	if(tree->right){ 
		print_tree(tree->right);
	}
}
void yyerror(char * s) {
    fprintf(stderr, "%s\n", s);
}

int main(void) {
    yyparse();
}