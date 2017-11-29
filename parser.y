%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	// binary tree struct
	typedef struct TreeNode{
		char* data;
		struct TreeNode *left, *right;
	} TreeNode;
	// binary tree functions declerations
	TreeNode* makeNode(char*, TreeNode*, TreeNode*);
	void prinpreorder(TreeNode*, int);
	extern int yylex();
	extern int yylineno;
	extern char *yytext;
	int yyerror(const char *msg);
%}
%union {
  char* string;
  struct TreeNode* TreeNode;
}

/* TOKENS */
/* TYPES */
%token <string> BOOLEAN CHAR VOID INT STRING PINT PCHAR PNULL
/* OPERATORS */
%right ADDRESS CONTENT
%left MINUS PLUS 
%left MUL DIVIDE
%left ASSIGMENT EQUAL GT GTE LT LTE NOTEQUAL
%token <string> AND NOT OR
/* CONDITIONS */
%token <string> IF ELSE WHILE DOWHILE FOR RETURN
/* globals */
%token <string> BOOL_TRUE BOOL_FALSE CHAR_LITERAL IDEN STRING_LITERAL INTEGER 
/* others */
%token <string> SEMICOLON COLON COMMA O_CURL C_CURL O_PAREN C_PAREN VERT_LINE C_BRACK O_BRACK
/* types */
/* definitions */
%%

s: program;

program: functions;

functions: functions function_d
		   |function_d
		   ;

function_d: funtype IDEN O_PAREN params_list C_PAREN O_CURL  C_CURL;

params_list: params_list COMMA param
		     |param
			 ;

param: param_type IDEN
	   |IDEN
	   ;

funtype:VOID
	    |BOOLEAN
	    |CHAR
		|INT
		|PCHAR
		|PINT
		;

param_type:INT
           |BOOLEAN
		   |CHAR
		   |PCHAR
		   |PINT
		   ;






%%
/* subroutines */
int yyerror(const char *msg)
{
	fflush(stdout);
	fprintf(stderr, "Error: %s at line %d\n", msg, yylineno);
	fprintf(stderr, "Parser does not expect '%s'\n",yytext);
}
int main() {
  yyparse();
  return 0;
}
TreeNode* makeNode(char* token, TreeNode* left, TreeNode* right){
  TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
  new_node->data = strdup(token); // so we get a new pointer and not the original
  new_node->left = left;
  new_node->right = right;
  return new_node;
}
void prinpreorder(TreeNode* tree,int indent)
{
    int i;
    if (tree)
    {
        for(i = 0; i < indent; i++)
          printf("\t");
        
        printf("(%s)\n",tree->data);
        prinpreorder(tree->left, indent+1);
        prinpreorder(tree->right, indent+1);
    }
}