#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

void fail(Token t1)
{
	printf("\nError while processing %s\n",t1.lexeme_name);
	exit(0);
}

void lexp(FILE *);
void list(FILE *);
void lexp_seq(FILE *);
void lexp_seq_prime(FILE *); 

void lexp(FILE *fp)
{
	Token t1 = getNextToken(fp);
	if(t1.type==7||t1.type==5)
	{
		printf("%s",t1.lexeme_name);
		return;
	}

	else if(strcmp(t1.lexeme_name,"(")==0)
	{
		printf("%s",t1.lexeme_name);
		list(fp);
		return;
	}
	else
	{
		fail(t1);
	}
}

void list(FILE *fp)
{
	lexp_seq(fp);
	Token t1 = getNextToken(fp);
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

void lexp_seq(FILE *fp)
{
	lexp(fp);
	lexp_seq_prime(fp);
	return;
}

void lexp_seq_prime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	Token t1 = getNextToken(fp);
	if(t1.type==7||t1.type==5||strcmp(t1.lexeme_name,"(")==0)
	{
		fsetpos(fp,&pos);
		lexp(fp);
		lexp_seq_prime(fp);
		return;
	}

	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

int main()
{
	FILE *fp;
	fp = fopen("test4.c","r");
	if(fp == NULL)
	{
		printf("\nCannot open the file\n");
		exit(0);
	}
	lexp(fp);
	printf("\nProcessed successfully\n");
	return 0;
}
