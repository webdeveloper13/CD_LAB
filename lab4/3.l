%{
	int pInt = 0, pFloat = 0, nInt = 0, nFloat = 0;
%}

%%
[+]?[0-9]+ {pInt++;}
[-]?[0-9]+ {nInt++;}
[+]?[0-9]*[.][0-9]+ {pFloat++;}
[-]?[0-9]*[.][0-9]+ {nFloat++;}
%%

int yywrap(){
	return 1;
}

main(){
	yylex();
	printf("pInt: %d\t\tnInt: %d\t\tpFloat: %d\tnFloat %d\n", pInt, nInt, pFloat, nFloat);
}