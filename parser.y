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
		struct Node *one, *two, *three, *four;
	} Node;
	// binary tree functions declerations
	Node* makeNode(char*, Node*, Node*,Node*,Node*);
	void printInOrder(Node*, int);
	int yyerror(const char *msg);
%}
%union {
  char* string; // node head
  struct Node* Node; // leafs
}

/* TOKENS */
/* TERMINALS */
%token <string> CHAR_LITERAL STRING_LITERAL INTEGER STRING
%token <string> IF ELSE FOR WHILE DOWHILE BOOL_TRUE BOOL_FALSE
%token <string> PLUS MINUS MUL DIV
%token <string> AND OR EQUAL GT GTE LT LTE NOT NOTEQUAL
%token <string> ADDRESS CONTENT TYPE RETURN IDEN
%token <string> SEMICOLON COLON COMMA O_CURL C_CURL O_PAREN C_PAREN VERT_LINE C_BRACK O_BRACK ASS
/* SPECS */
%left PLUS MINUS MUL DIV
%left AND OR GT GTE LT LTE EQUAL NOTEQUAL
%right ADDRESS CONTENT NOT

/* NODES */
/* basic leaf */
%type <Node> iden_name var_type
%type <Node> int_num bool_type var_char str
/* complex leafs */
%type <Node> var_dec vars_list 
%type <Node> str_index ptr
%type <Node> expr func_call return_stmt
%type <Node> cond block stmt multi_stmt

/* negation--unary minus */
%precedence NEG 

/* definitions */
%%
s: cond {printInOrder($1, 0);};

func_call: iden_name O_PAREN expr C_PAREN {$$ = makeNode("FUNCTION CALL", $1, $3, NULL, NULL);}
		| iden_name O_PAREN C_PAREN {$$ = makeNode("FUNCTION CALL NO PARAMS", $1, NULL, NULL, NULL);}
		;

block: O_CURL multi_stmt C_CURL {$$ = makeNode("BLOCK",$2,NULL, NULL, NULL);}
      | O_CURL C_CURL {$$ = makeNode("EMPTY BLOCK",NULL,NULL, NULL, NULL);}
      ;

multi_stmt: multi_stmt stmt { $$ = makeNode("MULTI STATEMENTS", $1,$2, NULL, NULL); }
		| stmt {$$ = $1;};

stmt: var_dec { $$ = makeNode("STATEMENT", $1, NULL, NULL, NULL); }
	| cond { $$ = makeNode("STATEMENT", $1, NULL, NULL, NULL); }
	| block { $$ = makeNode("STATEMENT", $1, NULL, NULL, NULL); }
	;


var_dec: var_type vars_list SEMICOLON { $$ = makeNode("PARAMETER",$1 ,$2, NULL, NULL); };
vars_list: vars_list COMMA iden_name { $$ = makeNode("MULTIPLE IDENTIFIERS", $1, $3, NULL, NULL); }
		 | iden_name { $$ = $1; };

cond: IF O_PAREN expr C_PAREN block {$$ = makeNode("IF",$3,$5,NULL, NULL); }
	| IF O_PAREN expr C_PAREN block ELSE block {$$ = makeNode("IF/ELSE",$3,makeNode("BOOLEAN",$5, $7, NULL, NULL), NULL, NULL); }
	;

expr: iden_name { $$ = makeNode("IDENTIFIER", $1, NULL, NULL,NULL); }
	| int_num  { $$ = makeNode("INTEGER", $1, NULL, NULL,NULL); }
	| var_char { $$ = makeNode("CHARACTER", $1, NULL, NULL,NULL); }
	| bool_type { $$ = makeNode("BOOLEAN", $1, NULL, NULL,NULL); }
	| NOT expr { $$ = makeNode("!", $2, NULL, NULL,NULL); }
	| MINUS expr %prec NEG { $$ = makeNode("NEGATIVE", $2, NULL,NULL,NULL); }
    | expr PLUS expr { $$ = makeNode("+",$1,$3,NULL,NULL); }
    | expr MINUS expr { $$ = makeNode("-",$1,$3,NULL,NULL); }
    | expr MUL expr { $$ = makeNode("*",$1,$3,NULL,NULL); }
    | expr expr expr { $$ = makeNode("/",$1,$3,NULL,NULL); }
	| expr LT expr { $$ = makeNode("<",$1,$3,NULL,NULL); }
    | expr GT expr { $$ = makeNode(">",$1,$3,NULL,NULL); }
    | expr LTE expr { $$ = makeNode("<=",$1,$3,NULL,NULL); }
    | expr GTE expr { $$ = makeNode(">=",$1,$3,NULL,NULL); }
    | expr EQUAL expr { $$ = makeNode("==",$1,$3,NULL,NULL); }
    | expr NOTEQUAL expr { $$ = makeNode("!=",$1,$3,NULL,NULL); }
    | expr AND expr { $$ = makeNode("&&",$1,$3,NULL,NULL); }
    | expr OR expr { $$ = makeNode("||",$1,$3,NULL,NULL); }
	| func_call { $$ = $1; }
	;

var_type:	TYPE {$$ = makeNode($1, NULL, NULL,NULL,NULL);};
bool_type:	BOOL_TRUE { $$ = makeNode("true", NULL, NULL,NULL,NULL); }
          | BOOL_FALSE { $$ = makeNode("false", NULL, NULL,NULL,NULL); };
iden_name:	IDEN {$$ = makeNode($1, NULL, NULL,NULL,NULL);};
int_num:	INTEGER  {$$ = makeNode($1, NULL, NULL,NULL,NULL);};
var_char:	CHAR_LITERAL {$$ = makeNode($1, NULL, NULL,NULL,NULL);};
str:		STRING_LITERAL {$$ = makeNode($1, NULL, NULL,NULL,NULL);};
str_index:  iden_name O_BRACK expr C_BRACK { $$ = makeNode("STRING INDEX", $1, $3, NULL, NULL) ;};

ptr: ADDRESS iden_name { $$ = makeNode("POINTER", $2, NULL,NULL,NULL); };
return_stmt: RETURN expr SEMICOLON { $$ = makeNode("RETURN",$2,NULL);};
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
Node* makeNode(char* token, Node* one, Node* two, Node* three, Node* four){

	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	new_node->one = one;
	new_node->two = two;
	new_node->three = three;
	new_node->four = four;
	return new_node;
}
void printInOrder(Node* tree,int indent)
{    
	int i;
    if (tree)
    {
        printf("%s\n",tree->data);
		if (tree->one)
	        printInOrder(tree->one, indent+1);
    	if (tree->two)
	        printInOrder(tree->two, indent+1);
    	if (tree->three)
	        printInOrder(tree->three, indent+1);
		if (tree->four)
	        printInOrder(tree->four, indent+1);
    }
}