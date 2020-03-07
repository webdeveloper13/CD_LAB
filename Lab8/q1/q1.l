%{
	#include "q1.tab.h"
%}

%%
int {return KEYWORD;}
\n {return NL;}
[a-zA-Z][a-zA-Z0-9_]* {return ID;}
[ ] { }
. {return yytext[0];}
%%

int yywrap(){
	return 1;
}