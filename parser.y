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

	void printer(const char * rule, int id){
		fflush(stdout);
		fprintf(stdout, "rule: %s > id : %d\n", rule, id);
	}

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

s: 	functions {printer("s",1);}
	|
	STRING_LITERAL {printer("s",2);}
	;

functions: functions function_d {printer("functions",1);}
		   |function_d {printer("functions",2);}
		   ;

function_d: funtype IDEN O_PAREN params_list C_PAREN O_CURL param_d body C_CURL;

params_list: param | params_list COMMA param;

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
		   |STRING
		   |PCHAR
		   |PINT
		   ;


return: RETURN expre SEMICOLON;

expre:BOOL_TRUE
	  |BOOL_FALSE
	  |INTEGER
	  |CHAR_LITERAL
	  |STRING_LITERAL
	  |PCHAR
	  |PINT
	  |IDEN
	  ;
		
body:function_u
	 |function_d
	 |return;

param_d: params_list SEMICOLON;

function_u:IDEN O_BRACK expre C_BRACK SEMICOLON
		   |IDEN O_BRACK expre C_BRACK
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