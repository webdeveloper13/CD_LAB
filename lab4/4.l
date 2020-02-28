%{
	int pc = 0, sc = 0;
%}

%%
	printf fprintf(yyout, "write"); pc++;
	scanf fprintf(yyout, "read"); sc++;
	ECHO;
%%

int yywrap(){
	return 1;
}

main(){
	yyin = fopen(a.c, "r");
	yyout = fopen(b.c, "w");
	yylex();
	printf("printf: %d\tscanf: %d\n", pc, sc);
}