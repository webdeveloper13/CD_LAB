#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int row=1,col=0;

typedef struct token
{
	char lexeme_name[50];
	int colno,rowno,type;
}Token;

bool isSpecialSymbol(char c,Token *t)
{
	switch(c)
	{
		case ';':strcpy(t->lexeme_name,";");
				 break;
		case ',':strcpy(t->lexeme_name,",");
				 break;
		case ':':strcpy(t->lexeme_name,":");
				 break;
		case '(':strcpy(t->lexeme_name,"(");
				 break;
		case ')':strcpy(t->lexeme_name,")");
				 break;
		case '{':strcpy(t->lexeme_name,"{");
				 break;
		case '}':strcpy(t->lexeme_name,"}");
				 break;
		case '[':strcpy(t->lexeme_name,"[");
				 break;
		case ']':strcpy(t->lexeme_name,"]");
				 break;
		default :return false;
	}
	return true;
}
bool isLogical(char c,Token *t,FILE *fp)
{
	fpos_t pos;
	char buf[5];
	int i=0,c1=col,r1=row;
	buf[i++] = c;
	if(c=='&' || c=='|')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		if(c==buf[i-1])
			buf[i++]=c;
		else
		{
			fsetpos(fp,&pos);
			col--;
		}
		buf[i] = '\0';
		t->rowno = r1;
		t->colno = c1;
		t->type = 3;
		strcpy(t->lexeme_name,buf);
		return true;
	}
	else if(c=='!')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		if(c=='=')
		{
			fsetpos(fp,&pos);
			col--;
			return false;
		}
		fsetpos(fp,&pos);
		col--;
		buf[i] = '\0';
		t->rowno = r1;
		t->colno = c1;
		t->type = 3;
		strcpy(t->lexeme_name,buf);
		return true;
	}
	return false;
}

bool isArithmetic(char c,Token *t,FILE* fp)
{
	fpos_t pos;
	char buf[5];
	int i=0,c1=col,r1=row;
	buf[i++] = c;
	if(c=='+' || c=='-')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		if(c==buf[i-1] || c=='=')
			buf[i++]=c;
		else
		{
			fsetpos(fp,&pos);
			col--;
		}
		buf[i] = '\0';
		t->rowno = r1;
		t->colno = c1;
		t->type = 2;
		strcpy(t->lexeme_name,buf);
		return true;
	}
	else if(c=='*' || c=='/' || c=='%')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		if(c=='=')
			buf[i++]=c;
		else
		{
			fsetpos(fp,&pos);
			col--;
		}
		buf[i] = '\0';
		t->rowno = r1;
		t->colno = c1;
		t->type = 2;
		strcpy(t->lexeme_name,buf);
		return true;
	}
	return false;
}

bool isNumerical(char c,Token *t,FILE *fp)
{
	fpos_t pos;
	char buf[20];
	int i=0,c1=++col,r1=row;
	buf[i++] = c;
	if(isdigit(c))
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		while(isdigit(c))
		{
			buf[i++] = c;
			fgetpos(fp,&pos);
			c = getc(fp);
			col++;
		}
		buf[i] = '\0';
		fsetpos(fp,&pos);
		strcpy(t->lexeme_name,buf);
		t->colno = c1;
		t->rowno = r1;
		t->type = 5;
		return true;
	}
	col--;
	return false;
}

bool isRelational(char c,Token *t,FILE *fp)
{
	fpos_t pos;
	char buf[5];
	int i=0,c1=col,r1=row;
	buf[i++] = c;
	if(c=='>' || c=='<')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		if(c=='=')
			buf[i++]=c;
		else
		{
			fsetpos(fp,&pos);
			col--;
		}
		buf[i] = '\0';
		t->rowno = r1;
		t->colno = c1;
		t->type = 0;
		strcpy(t->lexeme_name,buf);
		return true;
	}
	else if(c=='=' || c=='!')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		if(c=='=')
		{
			buf[i++]=c;
			t->type = 0;
		}
		else
		{
			fsetpos(fp,&pos);
			col--;
			if(buf[i-1]=='=')
				t->type = 1;
			else
				return false;
		}
		buf[i] = '\0';
		t->rowno = r1;
		t->colno = c1;
		strcpy(t->lexeme_name,buf);
		return true;
	}
	return false;
}

bool isIdentOrKeyw(char c,Token *t,FILE *fp)
{
	fpos_t pos;
	char buf[20];
	int i=0,c1=col,r1=row;
	buf[i++] = c;
	if(isalpha(c)!=0 || c=='_')
	{
		fgetpos(fp,&pos);
		c = getc(fp);
		col++;
		while(isalnum(c) || c=='_')
		{
			buf[i++] = c;
			fgetpos(fp,&pos);
			c = getc(fp);
			col++;
		}
		buf[i] = '\0';
		if(strcmp(buf,"int") == 0 || strcmp(buf,"float") == 0 || strcmp(buf,"double") == 0 || strcmp(buf,"if") == 0 || strcmp(buf,"else") == 0 || strcmp(buf,"for") == 0 || strcmp(buf,"while") == 0 || strcmp(buf,"void") == 0 || strcmp(buf,"do") == 0 || strcmp(buf,"switch") == 0 || strcmp(buf,"case") == 0 || strcmp(buf,"case") == 0 || strcmp(buf,"break") == 0)
			t->type = 8;
		else
			t->type = 7;
		fsetpos(fp,&pos);
		strcpy(t->lexeme_name,buf);
		t->colno = c1;
		t->rowno = r1;
		return true;
	}
	return false;
}

void startLiteral(char c,Token *t,FILE *fp)
{
	char buf[30];
	int k=0,r1=row,c1=col;
	c = getc(fp);
	col++;
	while(c!='"')
	{
		buf[k++] = c;
		col++;
		c = getc(fp);
	}
	col++;
	buf[k] = '\0';
	strcpy(t->lexeme_name,buf);
	t->colno = c1;
	t->rowno = r1;
	t->type = 6;
}

/*void placeComment(char c,FILE *fp)
{
	if(c=='/')
	{
		while(c!='\n')
		{
			c = getc(fp);
			col++;
		}
		row++;
		col=1;
	}
	else
	{
		do
		{
			while(c!='*')
			{
				if(c=='\n')
				{
					row++;
					col=1;
				}
				col++;
				c = getc(fp);
			}
			c = getc(fp);
			col++;
			if(c=='/')
				break;
		}while(1);
	}
}
*/

Token getNextToken(FILE *fp)
{
	Token t;
	char c = getc(fp);
	//col++;
	if(c==' ')
	{
		col++;
		t = getNextToken(fp);
		return t;
	}
	if(c=='\t')
	{
		col+=4;
		t = getNextToken(fp);
		return t;
	}
	if(c=='\n')
	{
		row++;
		col=1;
		t = getNextToken(fp);
		return t;
	}
	if(c=='$')
	{
		t.type = -3;
		return t;
		//printf("End of File encountered\n");
		//exit(0);
	}
	/*if(c=='/')
	{
		fpos_t pos;
		fgetpos(fp,&pos);
		c = getc(fp);
		if(c=='/'||c=='*')
			placeComment(c,fp);
		else
			fsetpos(fp,&pos);
		t.type = -1;
		return t;
	}
	*/
	if(c=='#')
	{
		while(c!='\n')
		{
			c = getc(fp);
			col++;
		}
		row++;
		col=1;
		t.type = -1;
		return t;
	}
	if(c=='"')
	{
		startLiteral(c,&t,fp);
		return t;
	}
	if(isSpecialSymbol(c,&t))
	{
		t.rowno = row;
		t.colno = col;
		t.type = 4;
		col++;
		return t;
	}
	if(isArithmetic(c,&t,fp))
		return t;
	if(isLogical(c,&t,fp))
		return t;
	if(isRelational(c,&t,fp))
		return t;
	if(isNumerical(c,&t,fp))
		return t;
	if(isIdentOrKeyw(c,&t,fp))
		return t;
	t.type = -1;
	return t;
}
