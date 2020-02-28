%{
	int c = 0, v = 0;

%}
%%
[aeiouAEIOU] {v++;}
[a-zA-Z] {c++;}
%%

int yywrap(){
	return 1;
}

main(){
	yylex();
	printf("Vowels: %d\tConsonants:%d\n", v, c);
}