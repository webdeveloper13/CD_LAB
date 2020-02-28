#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void fail(Token t1)
{
	printf("Error in %s\n",t1.lexeme_name);
	exit(0);
}

void proc_e(FILE *);
void proc_e_prime(FILE *);
void proc_t(FILE *);
void proc_t_prime(FILE *);
void proc_f(FILE *);

void proc_e(FILE *fp)
{
	proc_t(fp);
	proc_e_prime(fp);
}

void proc_e_prime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"+")==0)
	{
		printf("%s",t1.lexeme_name);
		proc_t(fp);
		proc_e_prime(fp);
		return;
	}
	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

void proc_t(FILE *fp)
{
	proc_f(fp);
	proc_t_prime(fp);
	return;
}

void proc_t_prime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"*")==0)
	{
		printf("%s",t1.lexeme_name);
		proc_f(fp);
		proc_t_prime(fp);
		return;
	}
	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

void proc_f(FILE *fp)
{
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"(")==0)
	{
		printf("%s",t1.lexeme_name);
		proc_e(fp);
		t1 = getNextToken(fp);
		if(strcmp(t1.lexeme_name,")")==0)
		{
			printf("%s",t1.lexeme_name);
			return;
		}
	}
	else if(strcmp(t1.lexeme_name,"id")==0)
	{
		printf("%s",t1.lexeme_name);
		return;
	}
	else
	{
		fail(t1);
	}
}

int main()
{
	FILE *fp;
	fp = fopen("test2.c","r");
	if(fp == NULL)
	{
		printf("\nCannot open the file\n");
		exit(0);
	}
	proc_e(fp);
	printf("\nProcessed successfully\n");
	return 0;
}