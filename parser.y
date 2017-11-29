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

s: 	functions {printer("s",1);};

functions: functions function_d {printer("functions",1);}
		   |function_d {printer("functions",2);}
		   ;

function_d: types IDEN O_PAREN params_list  C_PAREN O_CURL mul_body C_CURL;



params_list:params_list COMMA params
			  |params
			  |
			  ;

params:types IDEN;

types: INT
       |BOOLEAN
	   |CHAR
	   |STRING
       |PCHAR
	   |PINT
	   |VOID
	   ;

return: RETURN expre SEMICOLON;

expre:IDEN
	  |bool_types
	  |pointers
	  |literals
	  |function_u
	  |expre PLUS expre
	  |expre MINUS expre
	  |expre MUL expre
	  |expre DIVIDE expre
	  |expre EQUAL expre
	  |expre GT expre
	  |expre GTE expre
	  |expre LT expre
	  |expre LTE expre
	  |expre NOTEQUAL expre
	  |expre ASSIGMENT expre
	  |expre AND expre
	  |expre OR expre

	  
	  ;

pointers:PCHAR|PINT;

bool_types:BOOL_FALSE|BOOL_TRUE;

literals: INTEGER
	  	  |CHAR_LITERAL
	  	  |STRING_LITERAL 
		  ;

mul_body: mul_body body
          |body
		  ;

body:function_u
	 |function_d
	 |return
	 |
	 ;

function_u:IDEN O_PAREN C_PAREN SEMICOLON
		   |IDEN O_PAREN IDEN C_PAREN
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