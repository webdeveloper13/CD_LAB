#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define table_len 5000
struct token
{
	char lexeme[80];
	int row,col;
	char type[20];
};
typedef struct token token;


struct id_token
{
	char lexeme[100];
	char type[80];
	int size;
	int no_of_arg;
	char return_type[80];
	char scope;
	char arg[80];
};
typedef struct id_token id_token;


struct node
{
	id_token tok;
	struct node *next;
};
typedef struct node node;
node *table[table_len] = {NULL};
struct token prevtkn;
static int row=1,col=1;
char buf[3000];

const char specialsymbols[]  =   {'?',';',':',','};
const char *keywords[] = {"const", "char", "int",
                          "return","for", "while", "do",
						"switch", "if", "else","unsigned", 
						"case", "break" , "double" , "float" };

const char arithmeticsymbols[]={'*'};
int noa,after=0,infunc=0;


int hashf(char str[])
{
	int val=0;
	int c=2;
	for(int i=0;i<strlen(str);i++)
	{
		val+=c*str[i];
		c++;
	}
	return val;
}
int search(char str[])
{
	int pos=hashf(str)%table_len;
	node *temp=table[pos];
	while(temp)
	{
		id_token t=temp->tok;
		if(strcmp(t.lexeme,str)==0)
		{
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}
void insert(id_token t)
{
	if(search(t.lexeme)==1)
	  return;
	node *nd = (node*)malloc(sizeof(node));
	nd->tok=t;
	nd->next=NULL;
	int pos=hashf(t.lexeme)%table_len;
	if(table[pos]==NULL)
	{
		table[pos]=nd;
		return;
	}
	else
	{
		node *temp=table[pos];
		while(temp->next)
		{
			temp=temp->next;
		}
		temp->next=nd;
	}
}

void print(id_token tok)
{
	printf("%s\t%s\t%d\t%c\t%d\t\t%s\t\t\t%s\n",tok.lexeme,tok.type,tok.size,tok.scope,tok.no_of_arg,tok.arg,tok.return_type);
}
int count_no(char str[])
{
	int count=0,a=0;
	for(int i=0;i<table_len;i++)
	{
		node *temp=table[i];
		if(temp!=NULL)
		{
			while(temp->next!=NULL)
			{
				if(strcmp((temp->tok).lexeme,str)==0)
					{
						break;
						a=1;
					}
				count++;
				temp=temp->next;
			}
			if(a==1)
				break;
			count++;
		}
	}
	return count;
}
void display()
{
	printf("Name\tType\tSize\tScope\tNo_of_arg\tArguments\t\tReturn Type\n");
	for(int i=0;i<table_len;i++)
	{
		
		node *temp=table[i];
		if(temp!=NULL)
		{
			while(temp->next!=NULL)
			{
				print(temp->tok);
				temp=temp->next;
			}
			print(temp->tok);
		}
		
	}
}

int isKeyword(const char *str)
{
	for(int i=0;i<sizeof(keywords)/sizeof(char*);i++)
	   {
	   	if(strcmp(str,keywords[i])==0)
	   	  {
	   	  	return 1;
	   	  }
	   }
	return 0;
}

int array_type(int c,const char *arr)
{
	int len;
	if(arr==specialsymbols)
	  {
	  	len=sizeof(specialsymbols)/sizeof(char);
	  }
	else if(arr==arithmeticsymbols)
	       {
	       	len=sizeof(arithmeticsymbols)/sizeof(char);
	       }
	for(int i=0;i<len;i++)
	   {
	   	if(c==arr[i])
	   	  {
	   	  	return 1;
	   	  }
	   }
	return 0;
}

void build_token(struct token *tkn,char c,int row,int col, char *type)
{
	tkn->row=row;
	tkn->col=col;
	strcpy(tkn->type,type);
	tkn->lexeme[0]=c;
	tkn->lexeme[1]='\0';
}

void newLine()
{
	++row;
	col=1;
}

struct token getNextToken(FILE *f1)
{
	int c;
	struct token tkn= 
	{
		.row=-1
	};
	int gotToken=0;
	while(!gotToken && (c=fgetc(f1))!=EOF)
		 {
		 	if(array_type(c,specialsymbols))
		 	  {
		 	  	build_token(&tkn,c,row,col,"SS");
		 	  	gotToken=1;
		 	  	++col;
		 	  }
		 	else if(array_type(c,arithmeticsymbols))
		 		   {
		 		   	build_token(&tkn,c,row,col,"ARITHMETIC OPERATOR");
		 		   	gotToken=1;
		 		   	++col;
		 		   }
		 	else if(c=='(')
		 		   {
		 		   	build_token(&tkn,c,row,col,"LB");
		 		   	gotToken=1;
		 		   	++col;
		 		   	infunc=1;
		 		   }
		 	else if(c==')')
		 		   {
		 		   	build_token(&tkn,c,row,col,"RB");
		 		   	gotToken=1;
		 		   	++col;
		 		   	infunc=0;
		 		   }
		 	else if(c=='{')
		 		   {
		 		   	build_token(&tkn,c,row,col,"LC");
		 		   	gotToken=1;
		 		   	++col;
		 		   	after=1;
		 		   }
		 	else if(c=='}')
		 		   {
		 		   	build_token(&tkn,c,row,col,"RC");
		 		   	gotToken=1;
		 		   	++col;
		 		   	after=0;
		 		   }
		    else if(c=='+')
		 		   {
		 		   	int d=fgetc(f1);
		 		   	if(d!='+')
		 		   	  {
		 		   	  	build_token(&tkn,c,row,col,"ARITHMETIC OPERATOR");
		 		   	  	gotToken=1;
		 		   	  	++col;
		 		   	  	fseek(f1,-1,SEEK_CUR);
		 		   	  }
		 		   	else
		 		   	  {
		 		   	  	build_token(&tkn,c,row,col,"UNARY OPERATOR");
		 		   	  	strcpy(tkn.lexeme,"++");
		 		   	  	gotToken=1;
		 		   	  	col+=2;
		 		   	  }
		 		   }
		 	else if(c=='-')
		 		   {
		 		   	int d=fgetc(f1);
		 		   	if(d!='-')
		 		   	  {
		 		   	  	build_token(&tkn,c,row,col,"ARITHMETIC OPERATOR");
		 		   	  	gotToken=1;
		 		   	  	++col;
		 		   	  	fseek(f1,-1,SEEK_CUR);
		 		   	  }
		 		   	else
		 		   	  {
		 		   	  	build_token(&tkn,c,row,col,"UNARY OPERATOR");
		 		   	  	strcpy(tkn.lexeme,"--");
		 		   	  	gotToken=1;
		 		   	  	col+=2;
		 		   	  }
		 		   }
		 	else if(c=='=')
		 		   {
		 		   	int d=fgetc(f1);
		 		   	if(d!='=')
		 		   	  {
		 		   	  	build_token(&tkn,c,row,col,"ASSIGNMENT OPERATOR");
		 		   	  	gotToken=1;
		 		   	  	++col;
		 		   	  	fseek(f1,-1,SEEK_CUR);
		 		   	  }
		 		   	else
		 		   	  {
		 		   	  	build_token(&tkn,c,row,col,"RELATIONAL OPERATOR");
		 		   	  	strcpy(tkn.lexeme,"==");
		 		   	  	gotToken=1;
		 		   	  	col+=2;
		 		   	  }
		 		   }
		else if(isdigit(c))
			   {
			   	tkn.row=row;
			   	tkn.col=col++;
			   	tkn.lexeme[0]=c;
			   	int k=1;
			   	while((c=fgetc(f1))!=EOF && isdigit(c))
			   		 {
			   		 	tkn.lexeme[k++]=c;	
			   		 	col++;
			   		 }
			   	tkn.lexeme[k]='\0';
			   	strcpy(tkn.type,"NUMBER");
			   	gotToken=1;
			   	fseek(f1,-1,SEEK_CUR);
			   }
		else if(c == '#') 
			   {
					while((c = fgetc(f1)) != EOF && c != '\n');
					newLine();
			   }
		else if(c=='\n')
			   {
			   	newLine();
			   	c = fgetc(f1);
				if(c == '#') 
				  {
					while((c = fgetc(f1)) != EOF && c != '\n');
					newLine();
			      }
			    else if(c != EOF) 
			           {
						fseek(f1, -1, SEEK_CUR);
						}
			   }
		else if(isspace(c))
			   {
			   	++col;
			   }
		else if(isalpha(c)||c=='_')
			   {
			   	tkn.row=row;
			   	tkn.col=col++;
			   	tkn.lexeme[0]=c;
			   	int k=1;
			   	while((c=fgetc(f1))!= EOF && isalnum(c))
			   		 {
			   		 	tkn.lexeme[k++]=c;
			   		 	++col;
			   		 }
			   	tkn.lexeme[k]='\0';
			   	if(isKeyword(tkn.lexeme))
			   	  {
			   	  	strcpy(tkn.type,"KEYWORD");
			   	  	prevtkn=tkn;
			   	  }
			   	else
			   	  {
			   	  	strcpy(tkn.type,"IDENTIFIER");
			   	  	id_token idtok;
			   	  	strcpy(idtok.lexeme,tkn.lexeme);
			   	  	if(c=='(')
			   	  	{
			   	  		char args[50]="";
			   	  		strcpy(idtok.type,"FUNC");
			   	  		idtok.size=0;
			   	  		strcpy(idtok.return_type,prevtkn.lexeme);
			   	  		
			   	  		fseek(f1,-1,SEEK_CUR);
			   	  		noa=0;
			   	  		token nexttkn=getNextToken(f1);
			   	  		while(1)
			   	  		{
			   	  			if(nexttkn.lexeme[0]==')')
			   	  				break;
			   	  			printf("<%s, %d, %d, %s>\n",nexttkn.lexeme,nexttkn.row,nexttkn.col,nexttkn.type);
			   	  			if(strcmp(nexttkn.type,"KEYWORD")==0)
			   	  			{
			   	  				noa++;
			   	  				int a=count_no(nexttkn.lexeme);
			   	  			    char pos[50]="<id,";
			   	  				pos[4]=48+a;
			   	  				pos[5]='>';
			   	  				printf("%s\n",pos);
			   	  				strcat(args,pos);
			   	  				printf("%s\n",args);
			   	  			}
			   	  			nexttkn=getNextToken(f1);
			   	  		}
			   	  		printf("<%s, %d, %d, %s>\n",nexttkn.lexeme,nexttkn.row,nexttkn.col,nexttkn.type);
			   	  		idtok.no_of_arg=noa;
			   	  		if(noa==0)
			   	  			strcpy(idtok.arg,"NULL");
			   	  		else
			   	  			strcpy(idtok.arg,args);
			   	  	}
			   	  	else
			   	  	{
			   	  		strcpy(idtok.type,prevtkn.lexeme);
			   	  		if(strcmp(idtok.type,"int")==0)
			   	  			idtok.size=4;
			   	  		else if(strcmp(idtok.type,"double"))
			   	  			idtok.size=8;
			   	  		else
			   	  			idtok.size=6;
			   	  		strcpy(idtok.return_type,"");
			   	  		idtok.no_of_arg=0;
			   	  		strcpy(idtok.arg,"");
			   	  	}
			   	  	if(after==0 && infunc==0)
			   	  		idtok.scope='G';
			   	  	else if(infunc==1)
			   	  		idtok.scope=' ';
			   	  	else
			   	  		idtok.scope='L';
			   	  	insert(idtok);
			   	  }
			   	gotToken=1;
			   	fseek(f1,-1,SEEK_CUR);
			   }
		else if(c=='/')
			   {
			   	int d=fgetc(f1);
			   	++col;
			   	if(d=='/')
			   	  {
			   	  	while((c=fgetc(f1))!= EOF && c!='\n')
			   	  		 {
			   	  		 	++col;
			   	  		 }
			   	    if(c=='\n')
			   	      {
			   	      	newLine();
			   	      }
			   	  }
			   	else if(d=='*')
			   		   {
			   		   	do
			   		   	 {
			   		   	 	if(d=='\n')
			   		   	 	  {
			   		   	 	  	newLine();
			   		   	 	  }
			   		   	 	while((c==fgetc(f1))!= EOF && c!='*')
			   		   	 		 {
			   		   	 		 	++col;
			   		   	 		 	if(c=='\n')
			   		   	 		 	  {
			   		   	 		 	  	newLine();
			   		   	 		 	  }
			   		   	 		 }
			   		   	 	++col;
			   		   	 }while((d==fgetc(f1))!= EOF && d!='/' && (++col));
			   		   	++col;
			   		   }
			   	else
			   	  {
			   	  	build_token(&tkn,c,row,--col,"ARITHMETIC OPERATOR");
			   	  	gotToken=1;
			   	  	fseek(f1,-1,SEEK_CUR);
			   	  }
			   }
	  	else if(c == '"') 
	  	      {
				tkn.row = row; 
				tkn.col = col; 
				strcpy(tkn.type, "STRING LITERAL");
				int k = 1; 
				tkn.lexeme[0] = '"';
				while((c = fgetc(f1)) != EOF && c != '"') 
					{
					tkn.lexeme[k++] = c;
					++col;
					}
				tkn.lexeme[k] = '"';
				gotToken = 1;
			   } 
	 	else if(c == '<' || c == '>' || c == '!') 
	 		   {
				build_token(&tkn, c, row, col, "RELATIONAL OPERATOR");
				++col;
				int d = fgetc(f1);
				if(d == '=') 
				  {
					++col;
					strcat(tkn.lexeme, "=");
				  } 
				else 
				  {
					if(c == '!')
					  {
						strcpy(tkn.type, "LOGICAL OPERATOR");
					  }
					fseek(f1, -1, SEEK_CUR);
				  }
				gotToken = 1;
			   } 
		else if(c == '&' || c == '|') 
		 	  {
				int d = fgetc(f1);
				if(c == d) 
				  {
					tkn.lexeme[0] = tkn.lexeme[1] = c;
					tkn.lexeme[2] = '\0';
					tkn.row = row;
					tkn.col = col; 
					++col; 
					gotToken = 1;
					strcpy(tkn.type, "LOGICAL OPERATOR");
				  } 
				else 
				  {
					fseek(f1, -1, SEEK_CUR);
				  }
				++col;
			  } 
		else 
		  {
			++col;
		  }
	}
	return tkn;
}

void removeSpaces(char filename[]){
	FILE * in_ptr = fopen(filename, "r");
	FILE * out_ptr = fopen("no_space.c", "w");
	char c;
	while((c = getc(in_ptr)) != EOF){
		if(c == ' ' || c == '\t'){
			putc(' ', out_ptr);
			while((c = getc(in_ptr)) == ' ' || c == '\t');
			if(c == EOF)
				break;
		}
		putc(c, out_ptr);
	}
	fclose(in_ptr);
	fclose(out_ptr);
}

int searchsub(char substr[], char str[]){
	int m = strlen(substr);
	int n = strlen(str);
	for(int i = 0; i <= n-m; i++){
		int j;
		for(j=0; j<m; j++)
			if(substr[j] != str[i+j])
				break;
		if(j == m)
			return i;
	}
	return -1;
}

void removeDirectives(char filename[]){
	FILE * in_ptr = fopen(filename, "r");
	FILE * out_ptr = fopen("no_directive.c", "w");
	char * line;
	int read; 
	size_t len = 0;
	while((read = getline(&line, &len, in_ptr)) != -1){
		char no_spaceLine[1000];
		int j = 0;
		for(int i=0; i<strlen(line); i++){
			if(line[i] != ' ' && line[i] != '\t')
				no_spaceLine[j++] = line[i]; 
		}
		if(searchsub("#include", no_spaceLine) == 0 || searchsub("#define", no_spaceLine) == 0){
			continue;
		}else{
			fprintf(out_ptr, "%s", line);
		}
	}
	fclose(in_ptr);
	fclose(out_ptr);
}


void removeComments(char filename[]){

	FILE *fa,*fb;
	int ca,cb;
	fa = fopen(filename,"r");
	if(fa==NULL){
		printf("Cannot open file\n");
		exit(0);
	}

	fb = fopen("remove_comm.c","w");
	ca = getc(fa);
	while(ca!=EOF)
	{
		if(ca=='/')
		{
			cb=getc(fa);
			if(cb=='/')
			{
			while(ca!='\n')
				ca=getc(fa);
		    }

		else if(cb=='*')
		{
			do{
				while(ca!='*')
					ca =getc(fa);
				ca = getc(fa);
			}while(ca!='/');
		}

		else
		{
			putc(ca,fb);
			putc(cb,fb);
		}

	}

	else
	{
		putc(ca,fb);

	}

	ca = getc(fa);

}

	fclose(fa);
	fclose(fb);

}


int main()
{
	FILE *f1;
	printf("Please enter your file name in same directory: ");
	char file_name[25];
	scanf("%s", file_name);
	f1=fopen(file_name,"r");
	if(f1==NULL)
	  {
	  	printf("Error,opening the file\n");
	  	return 0;
	  }
	FILE * fptr = fopen(file_name, "r");
	removeSpaces(file_name);
	fclose(fptr);
	removeDirectives("no_space.c");
	removeComments("no_directive.c");  
	struct token tkn;
	while((tkn=getNextToken(f1)).row!=-1)
		 {
		 	printf("<%s, %d, %d, %s>\n",tkn.lexeme,tkn.row,tkn.col,tkn.type);
		 }
    fclose(f1);
    display();
}
