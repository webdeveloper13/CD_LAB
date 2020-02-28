#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "token.h"

void fail(char ch)
{
	printf("\nError in %c\n",ch);
	exit(0);
}

void proc_s(FILE *);
void proc_a(FILE *);
void proc_a_prime(FILE *);
void proc_b(FILE *);

void proc_s(FILE *fp)
{
	char ch = getc(fp);
	if(ch=='a')
	{
		printf("%c",ch);
		proc_a(fp);
		ch = getc(fp);
		if(ch=='c')
		{
			printf("%c",ch);
			proc_b(fp);
			ch = getc(fp);
			if(ch=='e')
			{
				printf("%c",ch);
			}
			else
			{
				fail(ch);
			}
		}
		else
		{
			fail(ch);
		}
	}
	else
	{
		fail(ch);
	}
	return;
}

void proc_a(FILE *fp)
{
	char ch = getc(fp);
	if(ch=='b')
	{
		printf("%c",ch);
		proc_a_prime(fp);
	}
	else
	{
		fail(ch);
	}
}

void proc_a_prime(FILE *fp)
{
	fpos_t pos;
	fgetpos(fp,&pos);
	char ch = getc(fp);
	if(ch=='b')
	{
		printf("%c",ch);
		proc_a_prime(fp);
	}
	else
	{
		fsetpos(fp,&pos);
		return;
	}
}

void proc_b(FILE *fp)
{
	char ch = getc(fp);
	if(ch=='d')
	{
		printf("%c",ch);
		return;
	}
	else
	{
		fail(ch);
	}
}

int main()
{
	FILE *fp;
	fp = fopen("test3.c","r");
	if(fp == NULL)
	{
		printf("\nCannot open the file\n");
		exit(0);
	}
	proc_s(fp);
	printf("\nProcessed successfully\n");
	return 0;
}