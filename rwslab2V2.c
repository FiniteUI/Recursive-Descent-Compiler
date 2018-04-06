/*
Filename:	rwslab2V2.c
Associated files:	lex.yy.c, tokens.h
Author:	Richard Seddon
Date:	3-23-17
Compilation: gcc -o executablename lex.yy.c, rwsllab2V2
Run: executablename inputfile > outputfile
Currently input is handled by the program, output is redirected to file by OS
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"

#define nullStr ""

void openInput(int, char *[]);
void closeInput(FILE *);
//void openOutput(int, char *[]);
//void closeOutput(FILE *);
void emit(char *);
void error(char *);
void match(char *);
void num(void);
void id(void);
void program(void);
void stmt_list(void);
void stmt_list_rest(void);
void stmt(void);
void expr(void);
void expr_rest(void);
void term(void);
void term_rest(void);
void factor(void);

int yval = 0;
int outsuccess;
int yylex();
void yyrestart(FILE *);
extern char *yytext;
extern int yyleng;
extern int line;
extern int wrap;
FILE *yyin;
FILE *fpIn;
FILE *fpOut;

void openInput(int argc, char *argv[]) {
	fpIn = NULL;
	if (argc > 1) {
		if ( (fpIn = fopen(argv[1], "r")) == NULL ) {
			fprintf(stderr, "Error opening input file:  %s", argv[1]);
	    }
	  }
	else
    	fpIn = stdin;
}//end of openInput

void closeInput(FILE *in) {
	fclose(in);
}//end of close input

void emit(char *opcode) {
	printf(opcode);
}//end of emit

void error(char *errstr)
{
  printf(errstr);
  printf("%d", line);
} // end error

void match(char *token) {
	//puts("making match...");
	//puts(token);
	//puts(yytext);
    if (strcmp(token, yytext) == 0) {
		strcpy(yytext, nullStr);
		yval = yylex();
 	}
    else
	 error(strcat(token," expected in line "));
}//end match

void num() {
	//puts("num");
	emit("PUSH\t");
	emit(yytext);
	emit("\n");
	yval = yylex();
}//end of num

void id() {
	//puts("id");
	emit(yytext);
	emit("\n");
	yval = yylex();
}//end of id

void factor() {
	//puts("factor");
	if (yval == ID) {
		emit("RVALUE\t");
		id();
	}//enif
	else if (yval == NUM) {
		num();
	}//end else if
	else if (yval == OPENP) {
		match("(");
		expr();
		match(")");
	}//endelif
}//en of factor

void term_rest() {
	//puts("term_rest");
	if (yval == MUL) {
		match("*");
		factor();
		emit("MPY\n");
		term_rest();
	}//endif
	else if (yval == DIV) {
		match("DIV");
		factor();
		emit("DIV\n");
		term_rest();
	}//endelif
	else if (yval == MOD) {
		match("MOD");
		factor();
		emit("MOD\n");
		term_rest();
	}//endelif
}//end of term_rest

void term() {
	//puts("term");
	factor();
	term_rest();
}//end of term

void expr_rest() {
	//puts("expr_rest");
	if (yval == ADD) {
		match("+");
		term();
		expr_rest();
		emit("ADD\n");
	}//endif
	else if (yval == SUB) {
		match("-");
		term();
		expr_rest();
		emit("SUB\n");
	}//end elsif
}//end of expr_rest

void expr() {
	//puts("expr");
	term();
	expr_rest();
}//end of expr

void stmt() {
	//puts("stmt");
	if (yval == ID) {
		emit("LVALUE\t");
		id();
		match(":=");
		expr();
		emit("STO\n");
	}//endif
}//end of stmt

void stmt_list_rest() {
	//puts("stmt_list_rest");
	if (yval == SEMI) {
		match(";");
		stmt();
		stmt_list_rest();
	}//endif
}//end of stmt_list_rest

void stmt_list() {
	//puts("stmt_list");
	stmt();
	stmt_list_rest();
}//end of stmt_list

void program() {
	//puts("program");
	match("BEGIN");
	stmt_list();
	match("END");
}//end of program

void main(int argc, char *argv[]) {
	//printf("Opening input file...\n");
	openInput(argc, argv);
	//printf("Opening output file...\n");
	//openOutput(argc, argv);
	yyin = fpIn;
	yval = yylex();
	//printf("Building data section...\n");
	emit("Section	.data\n");
	wrap = 0;
	do {
		if (yval == ID) {
			emit(yytext);
			emit("\t:word\n");
		}//end if
		yval = yylex();
	} while (yval != END);
	closeInput(fpIn);
	openInput(argc, argv);
	yval = yylex();
	wrap = 1;
	emit("Section	.code\n");
	//printf("Beginning parse...\n");
	program();
	emit("HALT");
	//puts("Parse complete...\n");
	//printf("Closing output file...\n");
	//closeOutput(fpOut);

}
