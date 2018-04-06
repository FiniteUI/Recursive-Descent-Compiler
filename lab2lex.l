
%{
#include <stdio.h> 
#include "tokens.h"
int line = 0;
int wrap = 1;
%}
DELIM   [ \t\n]
WS      {DELIM}+
DIGIT 	[0-9]
NUMBER	{DIGIT}+
LETTER	[a-zA-z]
ID	{LETTER}({LETTER}|{DIGIT})*
%%
"\n"		{line++;}
{WS}		{}
BEGIN		{return(BEG);}
END		{return(END);}
:=		{return(ASSN);}
;		{return(SEMI);}
"+"		{return(ADD);}
"-"		{return(SUB);}
"*"		{return(MUL);}
DIV		{return(DIV);}
MOD		{return(MOD);}
{NUMBER}	{return(NUM);}
{ID}		{return(ID);}
"("		{return(OPENP);}
")"		{return(CLOSEP);}
.		{}
%%
int yywrap() {
	return wrap;
}



