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
	void print_preorder(TreeNode*, int);
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
%token T_BOOLEAN T_CHAR T_VOID T_INTEGER T_STRING T_P_INT T_P_CHAR T_NULL
/* OPERATORS */
%right OP_ADDRESS OP_CONTENT
%left OP_MINUS OP_PLUS 
%left OP_MUL OP_DIVIDE
%left OP_ASSIGMENT OP_EQUAL OP_GT OP_GTE OP_LT OP_LTE
%token OP_AND OP_NOT OP_NOTEQUAL OP_OR
/* CONDITIONS */
%token C_IF C_ELSE L_WHILE L_DOWHILE L_FOR K_RETURN
/* globals */
%token LT_TRUE T_FASLE LT_CHAR IDEN STRING_LITERAL LT_INTEGER 
/* others */
%token T_SEMICOLON T_COLON T_COMMA CURL_O CURL_R T_OPENPAREN T_CLOSEPAREN T_VERT_BAR T_R_BRACKET T_L_BRACKET
/* types */
/* definitions */
%%

s: functions;

functions: 	functions function
           	| function 
           	;

function: 	func_sign T_OPENPAREN params T_CLOSEPAREN CURL_O body CURL_R
			;


body: params;

type: T_BOOLEAN | T_INTEGER
	;

params: params T_COMMA param T_SEMICOLON
		| param T_SEMICOLON
		|
		;

param: type IDEN;

func_sign: type IDEN;
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
void print_preorder(TreeNode* tree,int indent)
{
    int i;
    if (tree)
    {
        for(i = 0; i < indent; i++)
          printf("\t");
        
        printf("(%s)\n",tree->data);
        print_preorder(tree->left, indent+1);
        print_preorder(tree->right, indent+1);
    }
}