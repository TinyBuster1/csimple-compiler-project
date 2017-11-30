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
	Node* makeTripelNode(char*, Node*, Node*,Node*);
	Node* makeQuadNode(char*, Node*, Node*,Node*,Node*);
	void printInOrder(Node*, int);
	int yyerror(const char *msg);
%}
%union {
  char* string; // node head
  struct Node* Node; // leafs
}

/* TOKENS */
/* TERMINALS */
%token <string> CHAR_LITERAL STRING_LITERAL INTEGER STRING VOID NULL_TYPE
%token <string> IF ELSE FOR WHILE DOWHILE BOOL_TRUE BOOL_FALSE
%token <string> PLUS MINUS MUL DIV
%token <string> AND OR EQUAL GT GTE LT LTE NOT NOTEQUAL
%token <string> ADDRESS CONTENT TYPE RETURN IDEN
%token <string> SEMICOLON COLON COMMA O_CURL C_CURL O_PAREN C_PAREN VERT_LINE C_BRACK O_BRACK ASS
/* SPECS */
%left PLUS MINUS 
%left MUL DIV
%left AND OR GT GTE LT LTE EQUAL NOTEQUAL
%right ADDRESS CONTENT NOT

/* NODES */
/* basic leaf */
%type <Node> iden_name stmt_type
%type <Node> int_num bool_type var_char str
/* complex leafs */
%type <Node> var_dec vars_list parameters single_param
%type <Node> str_index
%type <Node> expr func_call return_stmt
%type <Node> cond block block_return stmt multi_stmt
%type <Node> assignment ass_tar
%type <Node> program functions function

/* negation--unary minus */
%precedence NEG 

/* definitions */
%%
s: program {printInOrder($1, 0);};

program: functions { $$ = makeParentNode("Program",$1); };

functions: functions function { $$ = makePairNode("Functions", $1, $2); }
        | function { $$ = $1; }
		;
function: 
		stmt_type iden_name O_PAREN parameters C_PAREN block { $$ = makeQuadNode("FUNCTION PARAMS", $1, $2, $4, $6); }
		| 
		stmt_type iden_name O_PAREN C_PAREN block_return { $$ = makeTripelNode("FUNCTION NO PARAMS", $1, $2, $5); }
		;

func_call: iden_name O_PAREN expr C_PAREN {$$ = makePairNode("FUNCTION CALL", $1, $3);}
		| iden_name O_PAREN C_PAREN {$$ = makeParentNode("FUNCTION CALL NO PARAMS", $1);}
		;

block: O_CURL multi_stmt C_CURL {$$ = makeParentNode("BLOCK",$2);}
      | O_CURL C_CURL {$$ = makeBaseLeaf("EMPTY BLOCK");}
      ;

block_return: O_CURL multi_stmt return_stmt C_CURL {$$ = makePairNode("BLOCK RETURN",$2, $3);}
      | O_CURL return_stmt C_CURL {$$ = makeParentNode("EMPTY BLOCK RETURN", $2);}
      ;

multi_stmt: multi_stmt stmt { $$ = makePairNode("MULTI STATEMENTS", $1,$2); }
		| stmt {$$ = $1;};

stmt: var_dec { $$ = makeParentNode("VAR DEC STMT", $1); }
	| cond { $$ = makeParentNode("COND STMT", $1); }
	| block { $$ = makeParentNode("BLOCK STMT", $1); }
	| assignment { $$ = makeParentNode("ASS STMT", $1); }
	| function  { $$ = makeParentNode("FUNC DEC STMT", $1); }
	;


assignment: ass_tar ASS expr SEMICOLON{ $$ = makePairNode("ASSIGNMENT", $1, $3); };

ass_tar: iden_name { $$ = makeParentNode("ASSIGNMENT TARGET: VARIABLE", $1); };

/* int x, char y */
parameters:parameters COMMA single_param { $$ = makePairNode("PARAMETERS", $1, $3); }
		| single_param {$$ = $1;}
		;
/* int x */
single_param: stmt_type iden_name { $$ = makePairNode("PARAM", $1, $2); }

/* int x, y, z; */
var_dec: stmt_type vars_list SEMICOLON { $$ = makePairNode("VARIABLES DECLERATION",$1 ,$2); };

/* x,y,z */
vars_list: vars_list COMMA iden_name { $$ = makePairNode("MULTIPLE IDENTIFIERS", $1, $3); }
		 | iden_name { $$ = $1; };

cond: IF O_PAREN expr C_PAREN block {$$ = makePairNode("IF",$3,$5); }
	| IF O_PAREN expr C_PAREN block ELSE block {$$ = makePairNode("IF/ELSE",$3,makePairNode("BOOLEAN",$5, $7)); }
	;

expr: iden_name { $$ = makeParentNode("IDENTIFIER", $1); }
	| int_num  { $$ = makeParentNode("INTEGER", $1); }
	| var_char { $$ = makeParentNode("CHARACTER", $1); }
	| str { $$ = makeParentNode("STRING", $1); }
	| NULL_TYPE { $$ = makeBaseLeaf("NULL"); }
	| bool_type { $$ = makeParentNode("BOOLEAN", $1); }
	| ADDRESS iden_name { $$ = makeParentNode("POINTER", $2); }
	| NOT expr { $$ = makeParentNode("!", $2); }
	| MINUS expr %prec NEG { $$ = makeParentNode("NEGATIVE", $2); }
    | expr PLUS expr { $$ = makePairNode("+",$1,$3); }
    | expr MUL expr { $$ = makePairNode("*",$1,$3); }
    | expr MINUS expr { $$ = makePairNode("-",$1,$3); }
    | expr DIV expr { $$ = makePairNode("/",$1,$3); }
	| expr LT expr { $$ = makePairNode("<",$1,$3); }
    | expr GT expr { $$ = makePairNode(">",$1,$3); }
    | expr LTE expr { $$ = makePairNode("<=",$1,$3); }
    | expr GTE expr { $$ = makePairNode(">=",$1,$3); }
    | expr EQUAL expr { $$ = makePairNode("==",$1,$3); }
    | expr NOTEQUAL expr { $$ = makePairNode("!=",$1,$3); }
    | expr AND expr { $$ = makePairNode("&&",$1,$3); }
    | expr OR expr { $$ = makePairNode("||",$1,$3); }
	| func_call { $$ = $1; }
	;

stmt_type:	TYPE {$$ = makeBaseLeaf($1);} 
			| VOID {$$ = makeBaseLeaf("VOID");}
		 	| STRING iden_name str_index { $$ = makePairNode("STRING DEC", $2, $3) ;};

bool_type:	BOOL_TRUE { $$ = makeBaseLeaf("true"); }
          | BOOL_FALSE { $$ = makeBaseLeaf("false"); };
iden_name:	IDEN {$$ = makeBaseLeaf($1);};
int_num:	INTEGER  {$$ = makeBaseLeaf($1);};
var_char:	CHAR_LITERAL {$$ = makeBaseLeaf($1);};
str:		STRING_LITERAL {$$ = makeBaseLeaf($1);};
str_index:  iden_name O_BRACK expr C_BRACK { $$ = makePairNode("STRING INDEX", $1, $3) ;};
return_stmt: RETURN expr SEMICOLON { $$ = makeParentNode("RETURN",$2);};
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
Node* makeTripelNode(char* token, Node* one, Node* two, Node* three){

	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = strdup(token); // so we get a new pointer and not the original
	new_node->one = one;
	new_node->two = two;
	new_node->three = three;
	return new_node;
}

Node* makeQuadNode(char* token, Node* one, Node* two, Node* three, Node* four){

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