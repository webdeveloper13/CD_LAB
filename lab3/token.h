#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct token{
	char lType[40], lName[40];
	unsigned int row, col, size;
	// int val;	//in case of numeric constants
};

void removePreDir(); 	//Works...
void removeComment();	//Works...
void copyFile(); 		//works...
int isKeyWord(char t[]);
void printToken(struct token *t);
struct token * getNextToken(int *, int *);

FILE *fa, *fb;

int isKeyWord(char t[40]){

	if(!strcmp(t, "int") || !strcmp(t, "char") || !strcmp(t, "const") || !strcmp(t, "main")
		|| !strcmp(t, "return") || !strcmp(t, "if") || !strcmp(t, "else") || !strcmp(t, "while")
		|| !strcmp(t, "for") || !strcmp(t, "do") || !strcmp(t, "float") || !strcmp(t, "void") || !strcmp(t, "double")
		|| !strcmp(t, "printf") || !strcmp(t, "sqrt") || !strcmp(t, "scanf"))
	{
		return 1;
	}

	return 0;
}


void removePreDir(){ //Works...
	int ca,cb,cc,cd;
	fb = fopen("temp2.c","w");
	ca = getc(fa);
	while(ca!=EOF)
	{
		int flag=0;
		if(ca=='"')
		{
			putc(ca,fb);
			cc= getc(fa);
			while(cc!='"'){
				putc(cc,fb);
				cc = getc(fa);
			}

			putc(cc,fb);


		}

		else if(ca=='/')
		{
			//putc(ca,fb);
			cd=getc(fa);
			if(cd=='/')
			{
				putc(cd,fb);
			while(ca!='\n'){
				putc(ca,fb);
				ca=getc(fa);
			}
		    }

		    else if(cd=='*')
		{
			putc(ca,fb);

			putc(cd,fb);
			do{
				while(ca!='*'){
					putc(ca,fb);
					ca =getc(fa);
				}
				ca = getc(fa);
			}while(ca!='/');

			putc(cd,fb);
			putc(ca,fb);
		}

		else
		{
			putc(ca,fb);
			putc(cd,fb);
		}

	}
      else if(ca=='#')
		{
			flag=1;
			cb = getc(fa);
			while(cb!='\n')
				cb = getc(fa);

			putc(cb,fb);

		}

		else
		{
			putc(ca,fb);
		}

		ca = getc(fa);
	}

	fclose(fb);
}


void removeComment(){	//replaces all commented characters with white spaces...

	int ca,cb;
	fa = fopen("temp1.c","r");
	if(fa==NULL){
		printf("Cannot open file\n");
		exit(0);
	}

	fb = fopen("temp2.c","w");
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


void copyFile(){

	fa = fopen("temp2.c", "r");
	fb = fopen("temp1.c", "w");

	char *line = NULL;
    size_t len = 0;
    ssize_t read;

	while( (read = getline(&line, &len, fa)) != -1){
		fputs(line, fb);
	}

	fclose(fa);
	fclose(fb);
}

struct token * getNextToken(int *row, int *col){
	int ca;
	int r = *row, c = *col;
	struct token *t;
	int type = 0, quote = 0;
	ca = getc(fa); c++;
	
	while(ca != EOF){
		//string literals...
		if(quote){
			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "STRING LITERAL");
			ca = getc(fa);
			c++;
			int i = 0;
			while(ca != '\"'){
				t->lName[i++] = ca;
				ca = getc(fa);
				c++;
				if(ca == '\n'){
					c = 0;
					r++;
				}
				else if (ca == '\t'){
					c += 3;
				}
			}
			t->lName[i] = '\0';
			quote = 0;
			*row = r; *col = c;
			return t;
		}
		else if(ca == '\"'){
			quote = 1;
			continue;
		}

		else if(ca == ' '){
			ca = getc(fa);
			c++;
			continue;
		}
		else if(ca == '\t'){
			c+=4;
			ca = getc(fa);
			continue;
		}
		else if(ca == '\n'){
			r++;
			c = 1;
			ca = getc(fa);
			continue;
		}

		//arithmetic operators...
		else if(ca == '+' || ca == '-' || ca == '*' || ca == '/' || ca == '%'){
			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "ARITHMETIC OPERATOR");
			int temp = ca;
			ca = getc(fa); c++;
			if((ca != '+') && (ca != '-') && (ca != '*') && (ca != '/') && (ca != '%')){
			// if(ca == ' '){
				char temp1[2];
				temp1[0] = temp; temp1[1] = '\0';
				strcpy(t->lName, temp1);
				fseek(fa, -1, SEEK_CUR); c--;
				*row = r; *col = c; return t;
			}
			else if(ca == '='){
				if(temp == '+'){
					strcpy(t->lName, "+=");
				}
				else if(temp == '-'){
					strcpy(t->lName, "-=");
				}
				else if(temp == '*'){
					strcpy(t->lName, "*=");
				}
				else if(temp == '/'){
					strcpy(t->lName, "/=");
				}
				else if(temp == '%'){
					strcpy(t->lName, "%=");
				}
				*row = r; *col = c;
				return t;
			}
			else if( ca == '+' && temp == '+'){
				strcpy(t->lName, "++");
				*row = r; *col = c;
				return t;
			}
			else if(ca == '-' && temp == '-'){
				strcpy(t->lName, "--");
				*row = r; *col = c;
				return t;
			}
		}


		//relops...
		else if(ca == '>' || ca == '<' || ca == '=' || ca == '!'){

			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "RELATIONAL OPERATOR");
			
			if(ca == '>'){
				ca = getc(fa); c++;
				if(ca == '='){
					strcpy(t->lName, ">=");
					*row = r; *col = c; return t;
				}
				else{
					fseek(fa, -1, SEEK_CUR); c--;
					strcpy(t->lName, ">");
					*row = r; *col = c; return t;
				}
			}
			else if(ca == '<'){
				ca = getc(fa); c++;
				if(ca == '='){
					strcpy(t->lName, "<=");
					*row = r; *col = c; return t;
				}
				else{
					fseek(fa, -1, SEEK_CUR); c--;
					strcpy(t->lName, "<");
					*row = r; *col = c; return t;
				}
			}
			else if(ca == '!'){
				ca = getc(fa); c++;
				if(ca == '='){
					strcpy(t->lName, "!=");
					*row = r; *col = c; return t;
				}
				else{
					fseek(fa, -1, SEEK_CUR); c--;
					strcpy(t->lType, "LOGICAL OPERATOR");
					strcpy(t->lName, "NOT");
					*row = r; *col = c; return t;
				}
			}
			else{
				ca = getc(fa); c++;
				if(ca == '='){
					strcpy(t->lName, "==");
					*row = r; *col = c; return t;
				}
				else{
					fseek(fa, -1, SEEK_CUR); c--;
					strcpy(t->lType, "ASSIGNMENT OPERATOR");
					strcpy(t->lName, "=");
					*row = r; *col = c; return t;
				}
			}
		}

		//logops...
		else if(ca == '&' || ca == '|'){
			
			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "LOGICAL OPERATOR");// t->type = 2;
			
			if(ca == '&'){
				ca = getc(fa); c++;
				if(ca == '&'){
					strcpy(t->lName, "AND");
					*row = r; *col = c; return t;
				}
				else{ //Treating & as a special symbol... 
					fseek(fa, -1, SEEK_CUR); c--;
					strcpy(t->lType, "SPECIAL SYMBOL");// t->type = 3;
					strcpy(t->lName, "&");
					*row = r; *col = c; return t;
				}
			}
			else{
				ca = getc(fa); c++;
				if(ca == '|'){
					strcpy(t->lName, "OR");
					*row = r; *col = c; return t;
				}
				else{ //Treating | as a special symbol...
					fseek(fa, -1, SEEK_CUR); c--; 
					strcpy(t->lType, "SPECIAL SYMBOL");// t->type = 3;
					strcpy(t->lName, "|");
					*row = r; *col = c; return t;
				}
			}
		}

		//numeric constants...
		else if(isdigit((char)ca)){
			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "NUMERICAL CONSTANT");// t->type = 5;
			int x = 0;
			t->lName[x++] = (char)ca;
			int temp = ca;
			ca = getc(fa); c++;
			while((isdigit((char)ca)) || ca == '.'){
				t->lName[x++] = (char)ca;
				ca = getc(fa); c++;
			}
			t->lName[x] = '\0';
			fseek(fa, -1, SEEK_CUR); c--;
			*row = r; *col = c; return t;
		}

		//variables & keywords...
		else if(isalpha(ca)){
			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "IDENTIFIER");
			int x = 0;
			t->lName[x++] = (char)ca;
			ca = getc(fa); c++;
			while(isalnum(ca)  || ca == '_'){
				t->lName[x++] = (char)ca;
				ca = getc(fa); c++;
			}
			t->lName[x] = '\0';
			fseek(fa, -1, SEEK_CUR); c--;
			if(isKeyWord(t->lName)){
				strcpy(t->lType, "KEYWORD");
				*row = r; *col = c; return t;
			}
			else{
				*row = r; *col = c; return t;
			}
		}

		else{ //if nothing else, it must be a special symbol...
			t = (struct token*)malloc(sizeof(struct token));
			t->col = c; t->row = r;
			strcpy(t->lType, "SPECIAL SYMBOL");
			char temp[2];
			temp[0] = (char)ca;
			temp[1] = '\0';
			strcpy(t->lName, temp);
			*row = r; *col = c; return t;
		}
	}

	return NULL;
}

void printToken(struct token *t){
	if(strcmp(t->lType, "NUMERICAL CONSTANT") == 0 || strcmp(t->lType, "ARITHMETIC OPERATOR") == 0){
		
		printf("%s\t\t%d\t%d\t", t->lType, t->row, t->col);
		
		for(int i=0; i<strlen(t->lName); i++){
			printf("%c", t->lName[i]);
		}
		printf("\n");
	}
	else if(strcmp(t->lType, "KEYWORD") == 0){
		printf("%s\t\t\t\t%d\t%d\t%s\n", t->lType, t->row, t->col, t->lName);
	}
	else if(strcmp(t->lType, "ASSIGNMENT OPERATOR") == 0){
		printf("%s\t\t%d\t%d\t%s\n", t->lType, t->row, t->col, t->lName);
	}
	else{
		printf("%s\t\t\t%d\t%d\t%s\n", t->lType, t->row, t->col, t->lName);
	}
}
