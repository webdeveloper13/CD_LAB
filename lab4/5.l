%{
	int flag = 0;
%}

%%
.*[ ][aA][nN][dD][ ].* {flag = 1;}
%%

int yywrap(){
	return 1;
}

main(){
	yylex();
	if(flag == 1){
		printf("Compound\n");
	}
	else{
		printf("Simple\n");
	}
}