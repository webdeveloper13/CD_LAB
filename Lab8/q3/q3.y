%{
	#include<stdio.h>
	#include<stdlib.h>
%}

%token NUMBER ID NL
%left '+'
%left '*'

%%
stmt: exp NL { printf("The value calculated is %d",$$);exit(0); }
	;
exp:exp'+'term
	|term
	{$$=newast('+',$1,$3);}
	;
term:term'*'factor
	|factor
	{$$=newast('*',$1,$3);}
	;
factor:ID
	|NUMBER
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


