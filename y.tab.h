/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_BOOLEAN = 258,
    T_CHAR = 259,
    T_VOID = 260,
    T_INTEGER = 261,
    T_STRING = 262,
    T_P_INT = 263,
    T_P_CHAR = 264,
    T_NULL = 265,
    OP_AND = 266,
    OP_DIVIDE = 267,
    OP_ASSIGMENT = 268,
    OP_EQUAL = 269,
    OP_GT = 270,
    OP_GTE = 271,
    OP_LT = 272,
    OP_LTE = 273,
    OP_MINUS = 274,
    OP_NOT = 275,
    OP_NOTEQUAL = 276,
    OP_OR = 277,
    OP_PLUS = 278,
    OP_MUL = 279,
    OP_ADDRESS = 280,
    OP_CONTENT = 281,
    C_IF = 282,
    C_ELSE = 283,
    L_WHILE = 284,
    L_DOWHILE = 285,
    L_FOR = 286,
    K_RETURN = 287,
    LT_TRUE = 288,
    T_FASLE = 289,
    LT_CHAR = 290,
    LT_IDEN = 291,
    STRING_LITERAL = 292,
    LT_HEX = 293,
    LT_OCT = 294,
    LT_INTEGER = 295,
    LT_BIN = 296,
    T_SEMICOLON = 297,
    T_COLON = 298,
    T_COMMA = 299,
    T_OPENBRACKET = 300,
    T_CLOSEBRACKET = 301,
    T_OPENPAREN = 302,
    T_CLOSEPAREN = 303,
    T_VERT_BAR = 304,
    T_R_BRACKET = 305,
    T_L_BRACKET = 306
  };
#endif
/* Tokens.  */
#define T_BOOLEAN 258
#define T_CHAR 259
#define T_VOID 260
#define T_INTEGER 261
#define T_STRING 262
#define T_P_INT 263
#define T_P_CHAR 264
#define T_NULL 265
#define OP_AND 266
#define OP_DIVIDE 267
#define OP_ASSIGMENT 268
#define OP_EQUAL 269
#define OP_GT 270
#define OP_GTE 271
#define OP_LT 272
#define OP_LTE 273
#define OP_MINUS 274
#define OP_NOT 275
#define OP_NOTEQUAL 276
#define OP_OR 277
#define OP_PLUS 278
#define OP_MUL 279
#define OP_ADDRESS 280
#define OP_CONTENT 281
#define C_IF 282
#define C_ELSE 283
#define L_WHILE 284
#define L_DOWHILE 285
#define L_FOR 286
#define K_RETURN 287
#define LT_TRUE 288
#define T_FASLE 289
#define LT_CHAR 290
#define LT_IDEN 291
#define STRING_LITERAL 292
#define LT_HEX 293
#define LT_OCT 294
#define LT_INTEGER 295
#define LT_BIN 296
#define T_SEMICOLON 297
#define T_COLON 298
#define T_COMMA 299
#define T_OPENBRACKET 300
#define T_CLOSEBRACKET 301
#define T_OPENPAREN 302
#define T_CLOSEPAREN 303
#define T_VERT_BAR 304
#define T_R_BRACKET 305
#define T_L_BRACKET 306

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
