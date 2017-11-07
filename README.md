# CSimple Compiler
A C(Simple) Compiler written in Lex & Yacc (and some C)

## Keywords Lexemes

* boolean
* char
* void
* int
* string
* intp (pointer)
* charp (pointer)
* if
* else
* while
* do
* for
* return
* null

## Operator Lexemes

We support the following operators, which follow the operator precedence table from the language C:

* `&&` 
* `/`
* `=`
* `==`
* `>`
* `>=`
* `<`
* `<=`
* `-`
* `!`
* `!=`
* `||`
* `+`
* `*`
* `&`
* `^`

## Literal Lexemes 

* *boolean*: "true" or "false"
* *char*: A character literal is a single, printable character, enclosed in single quotes. 
    * *Examples*:
    * 'a' : lowercase a
    * 'A' : uppercase a
    * "a" : not a character; there are double quotes, and hence, this is a string
* *int*: An integer literal can be a decimal, hex, octal or binary number.
    * *Examples*:
    * 100 : Decimal (cannot start with Zero if it is NOT zero)
    * 0x01F : Hex (any number beginning with 0x or 0X and digits 0-9,A,B,C,D,E,F)
    * 0X02A : Hex
    * 07234 : Octal (any number that starts with zero and has digits 0-7)
    * 0111b : Binary (any number ending with 'b' and having digits 0-1)
* *string*: A string is an array of characters.
    * *Examples*:
    * "this is a string" : simple string that contains 16 character
    * "this is \"invalid\"" : invalid string, double quotes cannot be escaped
    * "this is no newline\n" : string that contains 20 characters, including a backslash and a lowercase n
    * "" : empty strings are okay
* *identifier*: An identifier literal can be a variable or function name. Identifiers must start
with an alpha character (upper or lowercase letter), followed by zero or more digits, "_",
and/or other alpha characters.
* *pointers*: A pointer is a type that points to a value of its base type. 
    * We have *two* pointers in our language, a pointer to a *character* (of type `charp`), \n and a pointer to an *integer* (of type `intp`).