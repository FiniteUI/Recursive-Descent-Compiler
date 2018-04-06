# Recursive-Descent-Compiler, Spring 2017
This is a project for my Spring 2017 Compiler Theory class, a Recursive Descent Compiler written in C using Lex to generate a lexical analyzer.

Full details for the assignment are available in RecursiveDescentCompiler.pdf

lab2lex.l is the lexical analyzer written in Lex, which after being run through Lex generates lex.yy.c, the actual lexical analyzer written in C to be used in my compiler.

tokens.h is the C header file defining the tokens to be used.

rwslab2V2.c is the actual compiler written completely by me in C. It calls lex.yy.c for the next token, and recursively runs through the grammar to generate the simple output code. I also changed it from the grammar given to eliminate left recursion and potentially infinite loops. 
