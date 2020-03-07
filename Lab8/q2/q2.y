%{
	#include<stdio.h>
	#include<stdlib.h>
%}

%token KEYWORD ID NL
%left '+'
%left ';'
%left '('
%left ')'
%left '{'
%left '}'

%%
decl:KEYWORD'('S')''{'B'}'
	{ printf("Valid Expression");exit(0); }
	;
S:ID
;
B:S'+'S
	|
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

