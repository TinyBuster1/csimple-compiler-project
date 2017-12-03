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
	Node* makeBaseLeaf(char*);
	Node* makeParentNode(char*, Node*);
	Node* makePairNode(char*, Node*, Node*);
	Node* makeTripNode(char*, Node*, Node*, Node*);
	void printInOrder(Node*, int);
	int yyerror(const char *msg);

%}
%union {
  char* string; // node head
  struct Node* Node; // leafs
}

/* TOKENS */
/* TERMINALS */
%token <string> CHAR_LITERAL STRING_LITERAL INTEGER VOID NULL_TYPE
%token <string> IF ELSE FOR WHILE DOWHILE BOOL_TRUE BOOL_FALSE
%token <string> PLUS MINUS MUL DIV
%token <string> AND OR EQUAL GT GTE LT LTE NOT NOTEQUAL
%token <string> ADDRESS CONTENT TYPE RETURN IDEN
%token <string> SEMICOLON COLON COMMA O_CURL C_CURL O_PAREN C_PAREN VERT_LINE C_BRACK O_BRACK ASS
/* SPECS */
%left PLUS MINUS
%left MUL DIV
%left AND OR GT GTE LT LTE EQUAL NOTEQUAL NEG
%right ADDRESS CONTENT NOT

/* NODES */
/* basic leaf */
%type <Node> iden_name ident_type
%type <Node> bool_type
/* complex leafs */
%type <Node> var_dec vars_list parameters single_param
%type <Node> arr_index
%type <Node> expr func_call return_stmt
%type <Node> cond block block_return stmt body
%type <Node> assignment
%type <Node> program functions function f_parans

%nonassoc IFX
%nonassoc ELSE
/* definitions */
%%
s: program {printInOrder($1, 0);};

program: functions { $$ = makeParentNode("Program",$1); };

functions: functions function { $$ = makePairNode("Functions", $1, $2); }
        | function { $$ = $1; }
		;
function: 
		ident_type iden_name O_PAREN parameters C_PAREN block_return { 
				Node * input = makePairNode("INPUT",$2, $4);
				Node * output = makePairNode("OUTPUT", $1, $6);
				$$ = makePairNode("FUNCTION", input, output); 
			}
		| 
		ident_type iden_name O_PAREN C_PAREN block_return {
				Node * input = makeParentNode("INPUT",$2);
				Node * output = makePairNode("OUTPUT", $1, $5);
				$$ = makePairNode("FUNCTION NO PARAMS", input, output);  
			 }
		
		|
		VOID iden_name O_PAREN parameters C_PAREN block { 
				Node * input = makePairNode("INPUT",$2, $4);
				Node * output = makeParentNode("OUTPUT", $6);
				$$ = makePairNode("FUNCTION", input, output); 
			}
		| 
		VOID iden_name O_PAREN C_PAREN block {
				Node * input = makeParentNode("INPUT",$2);
				Node * output = makeParentNode("OUTPUT", $5);
				$$ = makePairNode("FUNCTION NO PARAMS", input, output);  
			 }
		;


f_parans: f_parans COMMA expr { $$ = makePairNode("FUNC INPUT PARAMS", $1, $3);}
		| expr {$$ = $1;}
		;

func_call: iden_name O_PAREN f_parans C_PAREN {$$ = makePairNode("FUNCTION CALL", $1, $3);}
		| iden_name O_PAREN C_PAREN {$$ = makeParentNode("FUNCTION CALL NO PARAMS", $1);}
		;

block: O_CURL body C_CURL {$$ = makeParentNode("BLOCK",$2);}
      | O_CURL C_CURL {$$ = makeBaseLeaf("EMPTY BLOCK");}
      ;

block_return: O_CURL body return_stmt C_CURL {$$ = makePairNode("BLOCK RETURN",$2, $3);}
			| O_CURL return_stmt C_CURL {$$ = makeParentNode("EMPTY BLOCK RETURN",$2);} 
      ;

body: body stmt { $$ = makePairNode("MULTI-LINE", $2 ,$1); }
	| stmt {$$ = $1;}

stmt: var_dec { $$ = $1; }
	| cond { $$ = $1; }
	| WHILE O_PAREN expr C_PAREN stmt {$$ = makePairNode("WHILE LOOP",$3,$5); }
	| DOWHILE stmt WHILE O_PAREN expr C_PAREN SEMICOLON {$$ = makePairNode("DO WHILE", $2, $5);}
	| FOR O_PAREN assignment SEMICOLON expr SEMICOLON assignment C_PAREN stmt {
			$$ = makePairNode("FOR LOOP", makeTripNode("FOR INPUT",$3, $5,$7), $9);  
		}
	| iden_name ASS expr SEMICOLON{ $$ = makePairNode("=", $1, $3); }
	| CONTENT iden_name ASS expr SEMICOLON{ $$ = makePairNode("PTR CONTENT", $2, $4); }
	| function  { $$ = $1; }
	| block { $$ =  $1; }
	;


assignment: iden_name ASS expr SEMICOLON{ $$ = makePairNode("=", $1, $3); }
		|	CONTENT	iden_name ASS expr SEMICOLON{ $$ = makePairNode("PTR CONTENT", $2, $4); }
		;

/* int x, char y */
parameters:parameters COMMA single_param { $$ = makePairNode("PARAMETERS", $1, $3); }
		| single_param {$$ = $1;}
		;
/* int x */
single_param: ident_type iden_name { $$ = makePairNode("PARAM", $1, $2); }
			;

/* int x, y, z; */
var_dec: ident_type vars_list SEMICOLON { $$ = makePairNode("VARIABLES DECLERATION",$1 ,$2); }
	;


/* x,y,z */
vars_list: vars_list COMMA iden_name { $$ = makePairNode("MULTIPLE IDENTIFIERS", $1, $3); }
		| iden_name { $$ = $1; }
		;

cond: IF expr stmt  %prec IFX {$$ = makePairNode("IF",$2,$3); }
	| IF expr stmt ELSE stmt {$$ = makePairNode("IF/ELSE",$2,makePairNode("boolean",$3, $5)); }
	;

expr: IDEN {$$ = makeParentNode("IDENT",makeBaseLeaf($1));}
	| IDEN arr_index {$$ = makeParentNode("ARRAY ACCESS",makeBaseLeaf($1));}
	| INTEGER  {$$ = makeParentNode("INT", makeBaseLeaf($1));}
	| CHAR_LITERAL {$$ = makeParentNode("char", makeBaseLeaf($1));}
	| STRING_LITERAL {$$ = makeParentNode("string", makeBaseLeaf($1));}
	| NULL_TYPE { $$ = makeBaseLeaf("NULL"); }
	| NOT expr { $$ = makeParentNode("!", $2); }
	| MINUS expr %prec NEG { $$ = makeParentNode("-", $2); }
    | expr PLUS expr { $$ = makePairNode("+",$1,$3); }
    | expr MUL expr { $$ = makePairNode("*",$1,$3); }
    | expr MINUS expr { $$ = makePairNode("-",$1,$3); }
    | expr DIV expr { $$ = makePairNode("/",$1,$3); }
	| CONTENT expr { $$ = makeParentNode("^",$2); }
	| ADDRESS expr { $$ = makeParentNode("&",$2); }
	| VERT_LINE iden_name VERT_LINE { $$ = makeParentNode("|",$2); }
	| func_call { $$ = $1; }
	| O_PAREN expr C_PAREN { $$ = $2; }
	| expr LT expr { $$ = makePairNode("<",$1,$3); }
    | expr GT expr { $$ = makePairNode(">",$1,$3); }
    | expr LTE expr { $$ = makePairNode("<=",$1,$3); }
    | expr GTE expr { $$ = makePairNode(">=",$1,$3); }
    | expr EQUAL expr { $$ = makePairNode("==",$1,$3); }
    | expr NOTEQUAL expr { $$ = makePairNode("!=",$1,$3); }
    | expr AND expr { $$ = makePairNode("&&",$1,$3); }
    | expr OR expr { $$ = makePairNode("||",$1,$3); }
	| bool_type { $$ = makeParentNode("boolean", $1); }
	;

ident_type:	TYPE {$$ = makeParentNode("TYPE", makeBaseLeaf($1));};
bool_type:	BOOL_TRUE { $$ = makeBaseLeaf("true"); }
          | BOOL_FALSE { $$ = makeBaseLeaf("false"); }
		  ;
iden_name:	IDEN {$$ = makeParentNode("IDENT",makeBaseLeaf($1));}
		| IDEN arr_index {$$ = makePairNode("ARRAY ACCESS",makeBaseLeaf($1), $2);}
		;
arr_index:  O_BRACK expr C_BRACK { $$ = $2 ;};
return_stmt: RETURN expr SEMICOLON { $$ = makeParentNode("RETURN",$2);}
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

Node* makeBaseLeaf(char* token){
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	return new_node;
}
Node* makeParentNode(char* token, Node* one){

	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	new_node->one = one;
	return new_node;
}
Node* makePairNode(char* token, Node* one, Node* two){

	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	new_node->one = one;
	new_node->two = two;
	return new_node;
}

Node* makeTripNode(char* token, Node* one, Node* two, Node * three){

	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	new_node->one = one;
	new_node->two = two;
	new_node->three = three;
	return new_node;
}

void printInOrder(Node* tree,int indent)
{    
	// printf("add: %p\n",tree);
	
	int i;
    if (tree)
    {
		for (i = 0; i < indent; i++)
			printf(" |");

    	if (tree->data)
	    	printf("-> %s\n",tree->data);

		if (tree->one){
	        printInOrder(tree->one, indent+1);
		}
    	if (tree->two){
			printInOrder(tree->two, indent+1);
		}

		if (tree->three){
			printInOrder(tree->three, indent+1);
		}
		
    }
}