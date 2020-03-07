%{
	#include<stdio.h>
	#include<stdlib.h>
%}

%token KEYWORD ID NL
%left ','
%left ';'

%%
decl:DT idlist ';'
	{ printf("Valid Expression");exit(0); }
	;
DT :KEYWORD
	;
idlist:ID','idlist
	|ID
	;
%%

int yyerror(char *msg)
{
	printf("Invalid Expression\n");
	exit(0);
}		

void main()
{
	printf("Enter the expression\n");
	yyparse();
}

