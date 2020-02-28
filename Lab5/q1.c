#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void fail(Token t)
{
	printf("\nCannot process the input for %s\n",t.lexeme_name);
	exit(0);
}

void proc_s(FILE *);
void proc_t(FILE *);
void proc_t_prime(FILE *);

void proc_s(FILE *fp)
{
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,"a")==0 || strcmp(t1.lexeme_name,">")==0)
	{
		printf("%s",t1.lexeme_name);
		return;
	}
	else if(strcmp(t1.lexeme_name,"(")==0)
	{
		printf("%s",t1.lexeme_name);
		proc_t(fp);
	}
	else
	{
		fail(t1);
	}
	t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,")")==0)
	{
		printf("%s",t1.lexeme_name);
		return;
	}
	else
	{
		fail(t1);
	}
}

void proc_t_prime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(strcmp(t1.lexeme_name,",")==0)
	{
		printf("%s",t1.lexeme_name);
		proc_s(fp);
		proc_t_prime(fp);
		return;
	}
	else if(strcmp(t1.lexeme_name,"$")==0)
	{
		printf("\nProcessed successfully\n");
		exit(0);
	}
	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

void proc_t(FILE *fp)
{
	proc_s(fp);
	proc_t_prime(fp);
	return;
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
	proc_s(fp);
	printf("\nProcessed successfully\n");
	return 0;
}