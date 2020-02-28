#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ASSIGN "ASSIGN"
#define KEYWORD "KEYWORD"
#define SS "SS"
#define ARITHOP "ARITHOP"
#define RELOP "RELOP"
#define LOGOP "LOGOP"
#define ID "ID"
#define OP "OP"
#define CP "CP"
#define OB "OB"
#define CB "CB"
#define OSB "OSB"
#define CSB "CSB"
#define NUMBER "NUMBER"
#define STRLIT "STRLIT"

typedef struct{
	char lexname[50];
	unsigned int row, col;
	char tokenType[50];
} Token;

void normalizeSpaces(char filename[]){
	FILE * in_ptr = fopen(filename, "r");
	FILE * out_ptr = fopen("spaceless.c", "w");
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

int search(char substr[], char str[]){
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

void discardDirectives(char filename[]){
	FILE * in_ptr = fopen(filename, "r");
	FILE * out_ptr = fopen("directiveless.c", "w");
	char * line;
	int read; 
	size_t len = 0;
	while((read = getline(&line, &len, in_ptr)) != -1){
		char spaceLessLine[1000];
		int j = 0;
		for(int i=0; i<strlen(line); i++){
			if(line[i] != ' ' && line[i] != '\t')
				spaceLessLine[j++] = line[i]; 
		}
		if(search("#include", spaceLessLine) == 0 || search("#define", spaceLessLine) == 0){
			continue;
		}else{
			fprintf(out_ptr, "%s", line);
		}
	}
	fclose(in_ptr);
	fclose(out_ptr);
}

void printToken(Token token){
	printf("<%d, %s, %s>\n", token.row, token.lexname, token.tokenType);
}

int isKeyword(char lex[]){
	int n = 13;
	char keywords[][10] = {
		"int",
		"void",
		"float",
		"char",
		"typedef",
		"struct",
		"while",
		"for",
		"unsigned",
		"if",
		"else",
		"const",
		"return"
	};
	for(int i=0; i<n; i++)
		if(strcmp(keywords[i], lex) == 0) return 1;
	return 0;
}

int isArithOp(char ch){
	char operators[] = "+-*/%";
	for(int i=0; i<5; i++)
		if(operators[i] == ch)
			return 1;
	return 0;
}

Token createToken(char lexname[], char tokenType[], int r){
	Token token;
	strcpy(token.lexname, lexname);
	strcpy(token.tokenType, tokenType);
	token.row = r;
	return token;
}

Token getNextToken(char code[], int * start, int * end, int *row){
	int s = *start, e = *end, in = 0;
	int r = *row;
	char ch;
	char buf[25];
	Token token;
	while(e < strlen(code)){
		ch = code[e++];
		if(ch == '\n'){ 
			r++;
			break;
		}
		if(ch == '\'') continue;
		char a = ch;
		if(a == '=' || a == '<' || a == '>'){
			ch = code[e];
			if(ch == '='){
				// == <= >=
				buf[in++] = a;
				buf[in++] = ch;
				buf[in] = '\0';
				e++;
				token = createToken(buf, RELOP, r);
			}else{
				// < > =
				buf[in++] = a;
				buf[in] = '\0';
				if(a == '='){
					token = createToken(buf, ASSIGN, r);
				}else{
					token = createToken(buf, RELOP, r);
				}
			}
			break;
		}
		if(ch == '|' || ch == '&' || ch == '!'){
			buf[in++] = ch;
			if(ch != '!'){
				ch = code[e++];
				if(ch == a)
					buf[in++] = ch;
			}
			buf[in] = '\0';
			token = createToken(buf, LOGOP, r);
			break;
		}
		if(ch == '\"'){
			// STRLIT
			buf[in++] = ch;
			while(e < strlen(code)){
				ch = code[e++];
				buf[in++] = ch;
				if(ch == '\"') break;
			}
			buf[in] = '\0';
			token = createToken(buf, STRLIT, r);
			break;
		}
		if(isalpha(ch) || ch == '_')
			buf[in++] = ch;
		else if(in != 0){
			buf[in] = '\0';
			if(isKeyword(buf))
				token = createToken(buf, KEYWORD, r);
			else token = createToken(buf, ID, r);
			e--;
			break;
		}else if(isArithOp(ch)){
			buf[in++] = ch;
			buf[in] = '\0';
			token = createToken(buf, ARITHOP, r);
			break;
		}else if(ch != ' ' && ch != '\n'){
			// Special symbols or digits
			char tokenType[25];
			int isDigit = 0;
			while(e < strlen(code) && isdigit(ch)){
				buf[in++] = ch;
				isDigit = 1;
				ch = code[e++];
			}
			if(isDigit){
				buf[in] = '\0';
				token = createToken(buf, NUMBER, r);
				e--;
			}else{
				switch(ch){
					case '(':
						strcpy(tokenType, OP);
						break;
					case ')':
						strcpy(tokenType, CP);
						break;
					case '{':
						strcpy(tokenType, OB);
						break;
					case '}':
						strcpy(tokenType, CB);
						break;
					case '[':
						strcpy(tokenType, OSB);
						break;
					case ']':
						strcpy(tokenType, CSB);
						break;
					case '.':
					case ',':
					case ';':
						strcpy(tokenType, SS);
						break;
				}
				buf[in++] = ch;
				buf[in] = '\0';
				token = createToken(buf, tokenType, r);
			}
			break;
		}
	}
	*start = e, *end = e;
	*row = r;
	return token;
}

int main(int argc, char const *argv[])
{
	printf("Enter file name : ");
	char filename[25];
	scanf("%s", filename);
	FILE * fptr = fopen(filename, "r");
	// Preprocessing
	normalizeSpaces(filename);
	fclose(fptr);
	discardDirectives("spaceless.c");
	fptr = fopen("directiveless.c", "r");
	// Remove comments and get text
	char code[10000], c;
	int index = 0;
	while(1){
		c = getc(fptr);
		if(c == EOF) break;
		if(c == '/'){
			c = getc(fptr);
			if(c == '/')
				while((c = getc(fptr)) != '\n');
		}
		code[index++] = c;
	}
	fclose(fptr);
	code[index] = '\0';


	// Identify lexemes and map them to tokens
	int start = 0, end = 0, row = 1;
	Token next;
	// int i = 100;
	while(end < index){
		next = getNextToken(code, &start, &end, &row);
		// printf("\nstart = %d\tend = %d", start, end);
		printToken(next);
	}
	return 0;
}