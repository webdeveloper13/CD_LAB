#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include <stdbool.h>

/*void fail(Token t)
{
	printf("\nError!!!Cannot process the input for %s \n",t.lexeme_name);
	exit(0);
}
*/

void proc_prog(FILE *);
void proc_dec(FILE *);
void proc_datatype(FILE *);
void proc_stat_list(FILE *);
void proc_id_list(FILE *);
void proc_A (FILE *);
void proc_B (FILE *);
void proc_stat(FILE * );
void decision_stat(FILE *);
void assign_stat(FILE *);
void expn(FILE *);
void simple_expn(FILE *);
void eprime(FILE *);
void seprime(FILE *);
void tprime(FILE *);
void dprime(FILE *);
void looping_stat(FILE *,int p);
void term(FILE *);

void proc_prog(FILE *fp)
{
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"main")==0)
	{
		printf("%s ",t1.lexeme_name);
		Token t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,"(")==0)
		{
			printf("%s ",t1.lexeme_name);
			t1 = getNextToken(fp);

			if(strcmp(t1.lexeme_name,")")==0)
			{
				printf("%s ",t1.lexeme_name);
				Token t2 = t1;
				t1 = getNextToken(fp);
				if(strcmp(t1.lexeme_name,"{")==0)
				{
					printf("%s ",t1.lexeme_name);
					proc_dec(fp);
					proc_stat_list(fp);
					Token t3 = getNextToken(fp);
					t1 = getNextToken(fp);
					
				    if(strcmp(t1.lexeme_name,"}")==0)
				    {
				    	printf("%s ",t1.lexeme_name);
				    	printf("\nCompiled successfully!!You are a good programmer\n");
				    	return;
				    }
				    else
				    {
				    	printf("\n\nCompilation Error!!\nYou are missing a '}' before line number %d .\nYou are a bad programmer\n",t3.rowno);
				    	exit(0);
				    }
				}
				else
				{
					printf("\n\nCompilation Error!!\nYou are missing a '{' before line number %d .\nYou are a bad programmer\n",t2.rowno);
				    exit(0);
				}

			}
			else
			{
				printf("\n\nCompilation Error!!\nYou are missing a ')' before line number %d .\nYou are a bad programmer\n",t1.rowno);
				exit(0);
			}			
		
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a '(' before line number %d .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
	}			    	

	else
	{
		printf("\n\nCompilation Error!!\nYou are missing a main function before %s before line number %d.\nYou are a bad programmer\n",t1.lexeme_name,t1.rowno);
		exit(0);
	}
}



void proc_dec(FILE *fp) {

	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if((strcmp(t1.lexeme_name,"int")==0)||(strcmp(t1.lexeme_name,"char")==0))
	{
		printf("%s ",t1.lexeme_name);
		proc_id_list(fp);
		Token t2 = t1;
		t1 = getNextToken(fp);

		if(strcmp(t1.lexeme_name,";")==0)
		{
			printf("%s ",t1.lexeme_name);
			proc_dec(fp);
			return;
		}

		else
		{

			printf("\n\nCompilation Error!!\nYou are missing a ';' at line number %d .\nYou are a bad programmer\n",t2.rowno);
			exit(0);
		}
		
	}

	else if(t1.type==7)
	{
		printf("\n\nCompilation Error!!\nYou havent defined a datatype(int or char) for variable at line number %d .\nYou are a bad programmer\n",t1.rowno);
		exit(0);
	}

	else
	{
		fsetpos(fp,&pos);
		return;
	}
}


void proc_id_list(FILE *fp)
{
	Token t1 = getNextToken(fp);
	Token t2 =t1;
	if(t1.type==7)
	{
		printf("%s ",t1.lexeme_name);
		proc_A(fp);
		return;
	}

	else
	{
		printf("\n\nCompilation Error!!\nYou are missing an identifier at line number %d .\nYou are a bad programmer\n",t2.rowno);
		exit(0);
	}
	
}

void proc_A(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,",")==0)
	{
		printf("%s ",t1.lexeme_name);
		proc_id_list(fp);
	}
	else if(t1.type==7)
	{
		printf("\n\nCompilation Error!!\nYou are probably missing a ',' at line number %d\n",t1.rowno);
		exit(0);
	}

	else if(strcmp(t1.lexeme_name,"[")==0)
	{
		printf("%s ",t1.lexeme_name);
		t1 = getNextToken(fp);
		if(t1.type==5)
		{
			printf("%s ",t1.lexeme_name);
		}

		t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,"]")==0)
		{
			printf("%s ",t1.lexeme_name);
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a ']' at line number %d .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
		proc_B(fp);
		return;
	}

	else
	{
		fsetpos(fp,&pos);
		return;
	}

}

void proc_B(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,",")==0)
	{
		printf("%s ",t1.lexeme_name);
		proc_id_list(fp);
		return;
	}

	else if(t1.type==7)
	{
		printf("\n\nCompilation Error!!\nYou are probably missing a ',' at line number %d\n",t1.rowno);
		exit(0);
	}

	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

void proc_stat_list(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	Token t2 = t1;
	//Check with the follow set of statement List
	if(strcmp(t1.lexeme_name,"if")==0)
	{
		printf("%s ",t1.lexeme_name);
		decision_stat(fp);
		proc_stat_list(fp);
		return;
	}

	else if((strcmp(t1.lexeme_name,"for")==0) || (strcmp(t1.lexeme_name,"while")==0))
	{
		printf("%s ",t1.lexeme_name);
		int status;
		if(strcmp(t1.lexeme_name,"for")==0)
		{
			status = 1;
		}
		else
		{
			status = 2;
		}

		looping_stat(fp,status);
		proc_stat_list(fp);
		return;
	}

	else if(t1.type==7)
	{
		printf("%s ",t1.lexeme_name);
		assign_stat(fp);
		t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,";")==0)
		{
			printf("%s %d",t1.lexeme_name,t2.rowno);
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing ';' before line number %d  .\nYou are a bad programmer\n",t2.rowno);
			exit(0);
		}
		proc_stat_list(fp);
		return;
	}

	else
	{
		fsetpos(fp,&pos);
		return;
	}


}

void assign_stat(FILE *fp)
{
	Token t1 = getNextToken(fp);
	Token t2 = t1;
	if(t1.type==7)
	{
		printf("%s ",t1.lexeme_name);
		t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,"=")==0)
		{
			printf("%s ",t1.lexeme_name);
			expn(fp);
			return;
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a assignment operator before line number %d  .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
	}
	else if(strcmp(t1.lexeme_name,"=")==0)
	{
		printf("%s ",t1.lexeme_name);
		expn(fp);
		return;
	}

	else
	{
		printf("\n\nCompilation Error!!\nYou are missing a assignment operator before line number %d  .\nYou are a bad programmer\n",t2.rowno);
		exit(0);
	}

}

void expn(FILE *fp)
{
	simple_expn(fp);
	eprime(fp);
	return;
}

void eprime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if((strcmp(t1.lexeme_name,"==")==0) || (strcmp(t1.lexeme_name,">=")==0) || (strcmp(t1.lexeme_name,"<=")==0) || (strcmp(t1.lexeme_name,"!=")==0) || (strcmp(t1.lexeme_name,">")==0) || (strcmp(t1.lexeme_name,"<")==0))
	{
		printf("%s ",t1.lexeme_name);
		simple_expn(fp);
		return;
	}

	else if(strcmp(t1.lexeme_name,"=")==0)
	{
		printf("\n\nCompilation error!!\nYou are probably missing = here before line number %d\n",t1.rowno);
		exit(0);
	}


	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

void simple_expn(FILE *fp)
{
	term(fp);
	seprime(fp);
	return;
}

void seprime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if((strcmp(t1.lexeme_name,"+")==0) || (strcmp(t1.lexeme_name,"-")==0))
	{
		printf("%s ",t1.lexeme_name);
		term(fp);
		seprime(fp);
		return;
	}
	/*else if(t1.type==7||t1.type==5)
	{
		printf("\n\nCompilation error!!\nYou are probably missing an arithmetic or relop operator here before line number %d\n",t1.rowno);
		exit(0);
	}
	*/

	else
	{
		fsetpos(fp,&pos);
		return;
	}

}

void term(FILE *fp)
{
	Token t1 = getNextToken(fp);
	if(t1.type==7 || t1.type == 5)
	{
		printf("%s ",t1.lexeme_name);
		tprime(fp);
		return;
	}

	else
	{
		printf("\n\nCompilation Error!!\nYou are missing an identifier or a number before line number %d.\nYou are a bad programmer\n",t1.rowno);
		exit(0);
	}

}

void tprime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if((strcmp(t1.lexeme_name,"*")==0) || (strcmp(t1.lexeme_name,"/")==0) || (strcmp(t1.lexeme_name,"%")==0))
	{
		printf("%s ",t1.lexeme_name);
		t1 = getNextToken(fp);
		if(t1.type==7 || t1.type == 5)
		{
			printf("%s ",t1.lexeme_name);
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing an identifier or a number before line number %d .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
		tprime(fp);
		return;
	}

	else
	{
		fsetpos(fp,&pos);
		return;
	}
}


void decision_stat(FILE *fp)
{
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"(")==0)
	{
		printf("%s ",t1.lexeme_name);
		expn(fp);
		t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,")")==0)
		{
			printf("%s ",t1.lexeme_name);
			t1 = getNextToken(fp);
			if(strcmp(t1.lexeme_name,"{")==0)
			{
				printf("%s ",t1.lexeme_name);
				proc_stat_list(fp);
				t1 = getNextToken(fp);
				if(strcmp(t1.lexeme_name,"}")==0)
				{
					printf("%s ",t1.lexeme_name);
					dprime(fp);
					return;
				}
				else
				{
					printf("\n\nCompilation Error!!\nYou are missing a '}' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
					exit(0);
				}
			}
			else
			{
				printf("\n\nCompilation Error!!\nYou are missing a '{' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
				exit(0);	
			}
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a ')' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
	}
	else
	{
		printf("\n\nCompilation Error!!\nYou are missing a '(' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
		exit(0);
	}
}

void dprime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"else")==0)
	{
		printf("%s ",t1.lexeme_name);
		t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,"{")==0)
		{
			printf("%s ",t1.lexeme_name);
			proc_stat_list(fp);
			t1 = getNextToken(fp);
			if(strcmp(t1.lexeme_name,"}")==0)
			{
				printf("%s ",t1.lexeme_name);
				return;
			}
			else
			{
				printf("\n\nCompilation Error!!\nYou are missing a '}' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
				exit(0);
			}
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a '{' before line number %d .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}		

	}
	else
	{
		fsetpos(fp,&pos);
		return;
	}

}


void looping_stat(FILE *fp,int status)
{
	Token t1 = getNextToken(fp);
	if(status==2)
	{
		if(strcmp(t1.lexeme_name,"(")==0)
		{
			printf("%s ",t1.lexeme_name);
			expn(fp);
			t1 = getNextToken(fp);
			if(strcmp(t1.lexeme_name,")")==0)
			{
				printf("%s ",t1.lexeme_name);
				t1 = getNextToken(fp);
				if(strcmp(t1.lexeme_name,"{")==0)
				{
					printf("%s ",t1.lexeme_name);
					proc_stat_list(fp);
					t1 = getNextToken(fp);
					if(strcmp(t1.lexeme_name,"}")==0)
					{
						printf("%s ",t1.lexeme_name);
						return;
					}
					else
					{
						printf("\n\nCompilation Error!!\nYou are missing a '}' before line number %d .\nYou are a bad programmer\n",t1.rowno);
						exit(0);
					}
				}
				else
				{
					printf("\n\nCompilation Error!!\nYou are missing a '{' before line number %d .\nYou are a bad programmer\n",t1.rowno);
					exit(0);
				}
			}
			else
			{
				printf("\n\nCompilation Error!!\nYou are missing a ')' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
				exit(0);
			}
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a '(' before line number %d .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
	}
	else
	{
		if(strcmp(t1.lexeme_name,"(")==0)
		{
			printf("%s ",t1.lexeme_name);
			assign_stat(fp);
			t1=getNextToken(fp);
			if(strcmp(t1.lexeme_name,";")==0)
			{
				printf("%s ",t1.lexeme_name);
				expn(fp);
				t1=getNextToken(fp);
				if(strcmp(t1.lexeme_name,";")==0)
				{
					printf("%s ",t1.lexeme_name);
					assign_stat(fp);
					t1=getNextToken(fp);
					if(strcmp(t1.lexeme_name,")")==0)
					{
						printf("%s ",t1.lexeme_name);
						t1=getNextToken(fp);
						if(strcmp(t1.lexeme_name,"{")==0)
						{
							printf("%s ",t1.lexeme_name);
							proc_stat_list(fp);
							t1 = getNextToken(fp);
							if(strcmp(t1.lexeme_name,"}")==0)
							{
								printf("%s ",t1.lexeme_name);
								return;
							}
							else
							{
								printf("\n\nCompilation Error!!\nYou are missing a '}' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
								exit(0);
							}

					    }
					    else
					    {
					    	printf("\n\nCompilation Error!!\nYou are missing a '{' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
							exit(0);
					    }
					}
					else
					{
						printf("\n\nCompilation Error!!\nYou are missing a ')' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
						exit(0);
					}
				}
				else
				{
					printf("\n\nCompilation Error!!\nYou are missing a ';' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
					exit(0);
				}
			}
			else
			{
				printf("\n\nCompilation Error!!\nYou are missing a ';' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
				exit(0);
			}
		}
		else
		{
			printf("\n\nCompilation Error!!\nYou are missing a '(' before line number %d  .\nYou are a bad programmer\n",t1.rowno);
			exit(0);
		}
	}

}	

int main()
{
	FILE *fp;
	fp = fopen("test1.c","r");
	if(fp == NULL)
	{
		printf("\nCannot open the file\n");
		exit(0);
	}
	proc_prog(fp);
	return 0;
}



