%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct node
	{ char *token;
	  struct node *left;
	  struct node *right;
	} node;
node *mknode ( char *token , node *left, node *right);
void printtree ( node *tree);
#define YYSTYPE struct node *

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
%%
s: program {printf("ok\n"); printtree($1);}

program
	: LT_IDEN OP_ASSIGMENT LT_INTEGER T_SEMICOLON { $$ = mknode($2, $1,$3);}
	;

%%
/* subroutines */
int main(){
	return yyparse();
}
node * mknode(char * token, node *left, node *right){
	node * newnode 	= (node*) malloc (sizeof(node));
	char * newstr 	= (char*) malloc (sizeof(token)+1);
	strcpy (newstr,token);
	newnode->left 	=	left;
	newnode->right 	=	right;
	newnode->token	=	newstr;
	
	return newnode;
}
void printtree(node *tree){
	printf("%s\n",tree->token);
	if(tree->left){ 
		printtree(tree->left);
	}
	if(tree->right){ 
		printtree(tree->right);
	}
}
int yyerror()
{
	printf("ERROR! /* TODO MAKE ERROR HANDLING BETTER */");
	return 0;
}