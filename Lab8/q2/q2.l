%{
	#include "q2.tab.h"
%}

%%
if {return KEYWORD;}
\n {return NL;}
[a-zA-Z][a-zA-Z0-9_]* {return ID;}
[ ] { }
. {return yytext[0];}
%%

int yywrap(){
	return 1;
}