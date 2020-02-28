%{
	int c = 0, w = 0, l = 0;

%}
%%
[a-zA-Z]+ {w++; c += strlen(yytext);}
\n {l++; c++;}
[.] {c++;}
%%

int yywrap(){
	return 1;
}

main(){
	yylex();
	printf("Characters: %d\tWords: %d\tLines: %d\n", c, w, l);
}