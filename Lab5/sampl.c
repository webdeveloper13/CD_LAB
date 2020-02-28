#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int row=1,col=0;
int idno=1;
FILE *f1;

typedef struct token
{
	char lexeme_name[50],data_type[10],ret_type[10];
	int colno,rowno,type,id,size,argno;
	int arglist[10];
}Token;

struct list
{
	Token t;
	struct list *next;
};

typedef struct list List;
typedef struct list* listptr;

List TABLE[20];

int dataSize(char str[])
{
	if(strcmp(str,"int")==0)
		return sizeof(int);
	else if(strcmp(str,"float")==0)
		return sizeof(float);
	else if(strcmp(str,"double")==0)
		return sizeof(double);
	else if(strcmp(str,"char")==0)
		return sizeof(char);
}

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
		if(strcmp(buf,"sqrt") == 0 || strcmp(buf,"if") == 0 || strcmp(buf,"printf") == 0 || strcmp(buf,"scanf") == 0 || strcmp(buf,"else") == 0 || strcmp(buf,"for") == 0 || strcmp(buf,"while") == 0 || strcmp(buf,"void") == 0 || strcmp(buf,"do") == 0 || strcmp(buf,"switch") == 0 || strcmp(buf,"case") == 0 || strcmp(buf,"case") == 0 || strcmp(buf,"break") == 0 || strcmp(buf,"return") == 0)
			t->type = 8;
		else if(strcmp(buf,"int") == 0 || strcmp(buf,"float") == 0 || strcmp(buf,"double") == 0 || strcmp(buf,"char") == 0)\
			t->type = 9;
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

void placeComment(char c,FILE *fp)
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

int prime(int c)
{
	int f,i,j;
	for(i=2;;i++)
	{
		f=0;
		for(j=2;j<=i/2;j++)
			if(i%j==0)
			{
				f=1;
				break;
			}
		if(f==0)
			c--;
		if(c==0)
			break;
	}
	return i;
}

long int hash(char str[])
{
	int n = strlen(str);
	long int h=0;
	for(int i=0;i<n;i++)
		h += (int)str[i] * prime(i+1);
	return h;
}

void INSERT(Token tv,long int hv)
{
	List e1;
	e1.t = tv;
	e1.next = NULL;
	listptr temp;
	temp = TABLE[hv%20].next;
	if(temp!=NULL)
	{
		while(temp->next!=NULL)
			temp = temp->next;
		temp->next = malloc(sizeof(List));
		*(temp->next) = e1;
		return;
	}
	TABLE[hv%20].next = malloc(sizeof(List));
	*(TABLE[hv%20].next) = e1;
	return;
}

listptr SEARCH(Token t,long int hv)
{
	listptr e1;
	e1 = TABLE[hv%20].next;
	if(e1==NULL)
		return false;
	else
	{
		while(e1!=NULL)
		{
			if(strcmp(e1->t.lexeme_name,t.lexeme_name)==0)
				return e1;
			e1 = e1->next;
		}
		return false;
	}
}

void init()
{
	for(int i=0;i<20;i++)
		TABLE[i].next=NULL;
}

Token getNextToken(FILE *fp)
{
	Token t;
	char c = getc(fp);
	//col++;
	if(c==' ')
	{
		col++;
		t.type = -1;
		return t;
	}
	if(c=='\t')
	{
		col+=4;
		t.type = -1;
		return t;
	}
	if(c=='\n')
	{
		row++;
		col=1;
		t.type = -2;
		return t;
	}
	if(c==EOF)
	{
		//printf("End of File encountered\n");
		t.type = -3;
		return t;
	}
	if(c=='/')
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

void printTable()
{
	printf("Id\tName\t\tType\t\tSize\tNoOfArg\tRetType\tArguments\n");
	listptr e1;
	for(int i=0;i<20;i++)
	{
		e1 = TABLE[i].next;
		while(e1!=NULL)
		{
			printf("%d\t%s\t\t%s\t\t%d\t%d\t%s\t",e1->t.id,e1->t.lexeme_name,e1->t.data_type,e1->t.size,e1->t.argno,e1->t.ret_type);
			if(e1->t.argno>=0)
			{
				if(e1->t.argno==0)
					printf("NULL\n");
				else
				{
					for(int j=0;j<e1->t.argno;j++)
						printf("<id,%d> ",e1->t.arglist[i]);
					printf("\n");
				}
			}
			else
			{
				printf("\n");
			}
			e1 = e1->next;
		}
	}
}

Token lookahead;
int i=0;

void proc_t();

void proc_e()
{
	f1 = fopen("test.c","r");
	lookahead = getNextToken(f1);
	if(lookahead.type==5)//number
	{
		printf("%s",lookahead.lexeme_name);
		proc_t();
		
	}
	else
	{
		printf("\nError1\n");
	}
	if(lookahead.type==-2||lookahead.type==-3)//newline or eof
		printf("\nSuccessful\n");
	else
		printf("\nError2\n");
}

void proc_t()
{
	lookahead = getNextToken(f1);
	if(lookahead.type==2)
	{
		printf("%s",lookahead.lexeme_name);
		lookahead = getNextToken(f1);
		if(lookahead.type==5)//number
		{
			printf("%s",lookahead.lexeme_name);
			proc_t();
		}
	}
	else
		return;
	return;
}

int main()
{
	proc_e();
	// fpos_t pos;
	// long int hv;
	// int sel=0,f=0,arg=0,cnt=0;
	// char data[10];
	// Token t1,t2,t3;
	// init();
	// listptr temp = NULL;
	// FILE *fp = fopen("test.c","r");
	// if(fp==NULL)
	// {
	// 	printf("Cannot open file\n");
	// 	exit(0);
	// }
	// fgetpos(fp,&pos);
	// while(1)
	// {
	// 	if(f==0)
	// 		t1 = getNextToken(fp);
	// 	if(t1.type<0)
	// 	{
	// 		if(t1.type==-3)
	// 			break;
	// 		else continue;
	// 	}
	// 	f=0;
	// 	if(sel==0 && t1.type==9)
	// 	{
	// 		sel=1;
	// 		strcpy(data,t1.lexeme_name);
	// 		//printf("Datatype : %s\n",data);
	// 		continue;
	// 	}
	// 	if(sel==1 && t1.type==7)
	// 	{
	// 		strcpy(t1.data_type,data);
	// 		t1.id = idno;
	// 		//printf("Id %d : %s\n",idno,t1.lexeme_name);
	// 		idno+=1;
	// 		strcpy(t1.ret_type,"NULL");
	// 		t1.argno = -1;
	// 		t1.size = dataSize(data);
	// 		if(arg==1)
	// 		{
	// 			cnt++;
	// 			temp = SEARCH(t3,hash(t3.lexeme_name));
	// 			temp->t.arglist[cnt-1] = idno-1;
	// 		}
	// 		hv = hash(t1.lexeme_name);
	// 		INSERT(t1,hv);
	// 		do
	// 		{
	// 			t2 = getNextToken(fp);
	// 		}while(t2.type<0);
	// 		if(strcmp(t2.lexeme_name,",")==0)//Store in table,dt=1,continue,id = k++
	// 		{
	// 			sel=1;
	// 			if(arg==1)
	// 				sel=0;
	// 			//f=1;
	// 			continue;
	// 		}
	// 		else if(strcmp(t2.lexeme_name,"[")==0)
	// 			sel=2;
	// 		else if(strcmp(t2.lexeme_name,"(")==0)
	// 			sel=3;
	// 		else if(strcmp(t2.lexeme_name,";")==0)
	// 			sel=4;
	// 		else if(strcmp(t2.lexeme_name,"=")==0)
	// 			sel=5;
	// 		else if(strcmp(t2.lexeme_name,")")==0)
	// 		{
	// 			temp = SEARCH(t3,hash(t3.lexeme_name));
	// 			sel=0;
	// 			arg=0;
	// 			temp->t.argno = cnt;
	// 			cnt=0;
	// 		}
	// 	}
	// 	if(sel==2)
	// 	{
	// 		//printf("Entered sel =%d\n",sel);
	// 		do{
	// 		t2 = getNextToken(fp);
	// 		}while(t2.type<0);
	// 		if(t2.type==5)
	// 		{
	// 			temp = SEARCH(t1,hash(t1.lexeme_name));
	// 			temp->t.size = atoi(t2.lexeme_name)*dataSize(temp->t.data_type);
	// 			//printf("Char length changed\n");
	// 		}
	// 		do{
	// 		t2 = getNextToken(fp);
	// 		}while(t2.type<0);
	// 		if(strcmp(t2.lexeme_name,"]")==0)//check again--end of array
	// 		{
	// 			do
	// 			{
	// 				t2 = getNextToken(fp);
	// 			}while(t2.type<0);
	// 			t1 = t2;
	// 			f=1;
	// 			sel=1;
	// 			continue;
	// 		}
	// 	}
	// 	if(sel==3)
	// 	{
	// 		temp = SEARCH(t1,hash(t1.lexeme_name));
	// 		//printf("Setting %s as FUNC\n",temp->t.lexeme_name);
	// 		strcpy(temp->t.data_type,"FUNC");
	// 		strcpy(temp->t.ret_type,data);
	// 		temp->t.size = 0;
	// 		temp->t.argno = 0;
	// 		t3 = t1;
	// 		sel=0;
	// 		arg=1;
	// 		continue;
	// 	}
	// 	if(sel==4)
	// 	{
	// 		sel=0;
	// 		continue;
	// 	}
	// 	if(sel==5)
	// 	{
	// 		do{
	// 		t2 = getNextToken(fp);
	// 		}while(strcmp(t2.lexeme_name,",")!=0 && strcmp(t2.lexeme_name,";")!=0);
	// 		if(strcmp(t2.lexeme_name,",")==0)
	// 		{
	// 			sel=1;
	// 			continue;
	// 		}
	// 		else if(strcmp(t2.lexeme_name,";")==0)
	// 		{
	// 			sel=0;
	// 			continue;
	// 		}
	// 	}
	// 	else if(strcmp(t1.lexeme_name,";")==0)
	// 	{
	// 		sel=0;
	// 		continue;
	// 	}
	// }
	// printTable();
	// row=1,col=0;
	// fsetpos(f1,&pos);
	// while(1)
	// {
	// 	Token t1 = getNextToken(fp);
	// 	if(t1.type>=0 || t1.type==-2)
	// 	{
	// 		switch(t1.type)
	// 		{
	// 			case 7 :hv = hash(t1.lexeme_name);
	// 					temp = SEARCH(t1,hash(t1.lexeme_name));
	// 					printf("<id,%d>",temp->t.id);
	// 					break;
	// 			case -2:printf("\n");
	// 					break;
	// 			default:printf("<%s>",t1.lexeme_name);
	// 		}
	// 	}
	// 	else if(t1.type==-3)
	// 	{
	// 		//printf("\nEnd of file encountered\n");
	// 		exit(0);
	// 	}
	// }
	// return 0;
}

		// if(t1.type>=0)
		// {
		// 	switch(t1.type)
		// 	{
		// 		case 0:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Relational Op");
		// 			   break;
		// 		case 1:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Assignment Op");
		// 			   break;
		// 		case 2:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Arithmetic Op");
		// 			   break;
		// 		case 3:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Logical Op");
		// 			   break;
		// 		case 4:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Special Symbol");
		// 			   break;
		// 		case 5:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Numerical Constant");
		// 			   break;
		// 		case 6:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"String Literal");
		// 			   break;
		// 		case 7:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Identifiers");
		// 			   break;
		// 		case 8:printf("<%s,%d,%d,%s>\n",t1.lexeme_name,t1.rowno,t1.colno,"Keywords");
		// 			   break;
		// 	}
		// }