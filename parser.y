%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern int yylex();
	extern int yylineno;
	extern char *yytext;
	// binary tree struct
	typedef struct Node{
		char* data;
		struct Node *left, *right;
	} Node;
	// binary tree functions declerations
	Node* makeNode(char*, Node*, Node*);
	void printInOrder(Node*, int);
	int yyerror(const char *msg);
%}
%union {
  char* string; // node head
  struct Node* Node; // leafs
}

/* TOKENS */
/* TERMINALS */
%token <string> CHAR_LITERAL STRING_LITERAL INTEGER
%token <string> IF ELSE FOR WHILE DOWHILE BOOL_TRUE BOOL_FALSE
%token <string> PLUS MINUS MUL DIV 
%token <string> AND OR EQUAL GT GTE LT LTE NOT NOTEQUAL
%token <string> ADDRESS CONTENT TYPE RETURN IDEN
%token <string> SEMICOLON COLON COMMA O_CURL C_CURL O_PAREN C_PAREN VERT_LINE C_BRACK O_BRACK ASS
/* NODES */
%type <Node> t ident
/* SPECS */
%left PLUS MINUS MUL DIV
%left AND OR GT GTE LT LTE EQUAL NOTEQUAL
%right ADDRESS CONTENT NOT
/* definitions */
%%
start:	t {printInOrder($1, 0);};
t:	 	ident {makeNode("IDENTIFIER", $1, NULL);};
ident: IDEN {$$ = makeNode($1, NULL,NULL);};
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
Node* makeNode(char* token, Node* left, Node* right){
	printf("makeNode: %s\n", token);

	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	new_node->left = left;
	new_node->right = right;
	printf("out tree %p:\n", new_node);
	return new_node;
}
void printInOrder(Node* tree,int indent)
{
	printf("PARSE TREE:\n");
    printf("input tree %p:\n", tree);
    
	int i;
    if (tree)
    {
        for(i = 0; i < indent; i++)
          printf("\t");
        
        printf("(%s)\n",tree->data);
        printInOrder(tree->left, indent+1);
        printInOrder(tree->right, indent+1);
    }
}