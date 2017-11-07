# CSimple Compiler
A C(Simple) Compiler written in Lex &amp; Yacc (and some C)

<details>
    <summary> ## Lexical Description </summary>
    <p>
        <details>
            <summary> ###  Keywords Lexemes </summary>
        </details>
        <p>
            * boolean
            * char
            * void
            * int
            * string
            * intp
            * charp
            * if
            * else
            * while
            * do
            * for
            * return
            * null
        </p>
    </p>


### Keywords Lexemes



### Operator Lexemes

We support the following operators, which follow the operator precedence table from the
language C:

* && 
* /
* =
* ==
* >
* >=
* <
* <=
* -
* !
* !=
* ||
* +
* *
* &
* ^

### Literal Lexemes 

* boolean: "true" or "false"
* char: A character literal is a single, printable character, enclosed in single quotes. 
    # Examples:
    * 'a' : lowercase a
    * 'A' : uppercase a
    * "a" : not a character; there are double quotes, and hence, this is a string
* int: An integer literal can be a decimal, hex, octal or binary number.
* ## Examples:
    