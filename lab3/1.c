#include "token.h"
//#include <unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct identifier{
	char name[30], type[30], retType[30];
	unsigned short int size, numArg, arg[100];
};

struct symbolTable{
	struct identifier id;
	struct symbolTable * next;
};

struct symbolTable* sTable = NULL;

int isDataType(char t[20]){
	if(!strcmp(t, "int") || !strcmp(t, "char") || !strcmp(t, "float") || !strcmp(t, "void") || !strcmp(t, "double")){
		return 1;
	}
	return 0;
}

int isPresent(struct identifier id){
	if(!sTable){
		return 0;
	}

	struct symbolTable *t;

	t = sTable;

	do{
		if(strcmp(t->id.name,id.name) == 0){
			return 1;
		}
	}while( (t = t->next));

	return 0;
}

int find(struct identifier id){
	if(!sTable){
		return 0;
	}

	struct symbolTable *t = sTable;

	int count = 1;
	
	do{
		if(strcmp(t->id.name,id.name) == 0){
			return count;
		}
		count++;
	}while( (t = t->next));

	return 0;
}

void addtoTable(struct identifier id){
	
	if(!sTable){
		sTable = (struct symbolTable*)malloc(sizeof(struct symbolTable));
		sTable->id = id;
		sTable->next =NULL;
		// printf("Table Created\n");
		return;
	}

	if(isPresent(id)){
		return;
	}

	struct symbolTable *t;

	t = sTable;

	while(t->next){
		t = t->next;
	}

	t->next = (struct symbolTable*)malloc(sizeof(struct symbolTable));
	t = t->next;
	t->id = id;
	t->next = NULL;
}

int getSize(char t[30]){
	if(strcmp(t, "int") == 0){
		return sizeof(int);
	}
	else if(strcmp(t, "char") == 0){
		return sizeof(char);
	}
	else if(strcmp(t, "float") == 0){
		return sizeof(float);
	}
	else if(strcmp(t, "double") == 0){
		return sizeof(double);
	}
	else{
		return sizeof(void);
	}
}

void displayTable(){
	if(!sTable){
		return;
	}

	struct symbolTable *t;

	t = sTable;
	int count = 1;
	printf("ID\tName\tType\tSize\tArgNum\tReturn\tArguments\n");
	while(t){
		if(strcmp(t->id.type, "FUNC") == 0){
			printf("%d\t%s\t%s\t%d\t%d\t%s\t", count++, t->id.name, t->id.type, t->id.size, t->id.numArg, t->id.retType);
			for(int i=0; i<t->id.numArg; i++){
				printf("<ID, %d>  ", t->id.arg[i]);
			}
			printf("\n");
			t = t->next;
			continue;
		}
		printf("%d\t%s\t%s\t%d\n", count++, t->id.name, t->id.type, t->id.size);
		t = t->next;
	}
}

int strToInt(char str[80]){
	int len = strlen(str);
	// printf("%d", str[0]);
	int num = (int)str[0] - 48;
	// printf("%d\n", num);
	int i = 1;
	while(i < len){
		num *= 10; 
		// printf("%d\n", num);
		num += (int)str[i] - 48;
		// printf("%d",  num);
		i++;
	}
	return num;
}	
	
int main(int argc, char const *argv[])
{
	int state, arrSize, numArg, cnt;
	char retType[30], arg[100];
	fa = fopen("sample.c", "r");
	int ca;

	if(!fa){
		printf("Cannot open file\n");
		exit(1);
	}

	removePreDir();
	fclose(fa);
	copyFile();
	removeComment();
	copyFile();

	struct token *t, *t_temp_1, *t_temp_2, *t_temp_3, *t_temp_4;

	// struct symbolTable* sT_temp;

	sTable = NULL;

	fa = fopen("temp1.c", "r");

	int row = 1, col = 0;

	// t = (getNextToken(&row, &col));

	while( t = (getNextToken(&row, &col))){
		printToken(t);
	}

	fclose(fa);
	
	fa = fopen("sample.c", "r");

	row = 1; col = 0;

	t = getNextToken(&row, &col);

	while(t){

		if(strcmp(t->lType, "KEYWORD") == 0){

			if(isDataType(t->lName)){
				
				int r = row, c = col;

				state = 1;

				while(1){
					t_temp_1 = getNextToken(&row, &col);

					if(strcmp(t_temp_1->lName,"main") == 0){
						struct identifier id;
						strcpy(id.type, "FUNC");
						strcpy(id.name, t_temp_1->lName);
						id.size = 0;
						id.numArg = 0;
						strcpy(id.retType, t->lName);
						// printf("Here\n");
						addtoTable(id);
						break;
					}

					if(state == 1){
						if(strcmp(t_temp_1->lType, "IDENTIFIER") == 0){
							state = 2;
							t_temp_2 = t_temp_1;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 2){
						if(strcmp(t_temp_1->lName, ";") == 0){
							struct identifier id;
							strcpy(id.type, t->lName);
							strcpy(id.name, t_temp_2->lName);
							id.size = (getSize(id.type));
							// printf("Here\n");
							addtoTable(id);
							break;
						}
						else if(strcmp(t_temp_1->lName, ",") == 0){
							struct identifier id;
							strcpy(id.type, t->lName);
							strcpy(id.name, t_temp_2->lName);
							id.size = (getSize(id.type));
							// printf("Here\n");
							addtoTable(id);
							state = 3;
							continue;
						}
						else if(strcmp(t_temp_1->lName, "=") == 0){
							struct identifier id;
							strcpy(id.type, t->lName);
							strcpy(id.name, t_temp_2->lName);
							id.size = (getSize(id.type));
							// printf("Here\n");
							addtoTable(id);
							state = 4;
							continue;
						}
						else if(strcmp(t_temp_1->lName, "[") == 0){
							state = 5;
							continue;
						}
						else if(strcmp(t_temp_1->lName, "(") == 0){
							numArg = 0;
							strcpy(retType, t->lName);
							cnt = 0;
							state = 11;
							continue;
						}
					}
					else if(state == 3){
						if(strcmp(t_temp_1->lType, "IDENTIFIER") == 0){
							t_temp_2 = t_temp_1;
							state = 2;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 4){
						if(strcmp(t_temp_1->lType, "NUMERICAL CONSTANT") == 0){
							state = 7;
							continue;
						}
						else if(strcmp(t_temp_1->lType, "NUMERICAL CONSTANT") == 0){
							state = 8;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 5){
						if(strcmp(t_temp_1->lType, "NUMERICAL CONSTANT") == 0){
							arrSize = strToInt(t_temp_1->lName);
							state = 6;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 6){
						if(strcmp(t_temp_1->lName, "]") == 0){
							// arrSize = strToInt(t_temp_1->lName);
							struct identifier id;
							strcpy(id.type, t->lName);
							strcpy(id.name, t_temp_2->lName);
							id.size = (getSize(id.type) * arrSize);
							// printf("Here\n");
							addtoTable(id);
							state = 2;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 7){
						if(strcmp(t_temp_1->lName, ",") == 0){
							state = 2;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 8){
						if(strcmp(t_temp_1->lName, ",") == 0){
							state = 2;
							continue;
						}
						else if(strcmp(t_temp_1->lName, "(") == 0){
							state = 9;
							break;
						}
						else{
							break;
						}
					}
					else if(state == 9){
						if(strcmp(t_temp_1->lName, ",") == 0 || strcmp(t_temp_1->lType, "IDENTIFIER") == 0){
							state = 9;
							continue;
						}
						else if(strcmp(t_temp_1->lName, ")") == 0){
							state = 10;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 10){
						if(strcmp(t_temp_1->lName, ",") == 0){
							state = 2;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 11){
						if(strcmp(t_temp_1->lName, ")") == 0){
							struct identifier id;
							strcpy(id.type, "FUNC");
							strcpy(id.name, t_temp_2->lName);
							strcpy(id.retType, retType);
							id.numArg = numArg;
							id.size = 0;
							for(int i=0; i<cnt; i++){
								id.arg[i] = arg[i];
							}
							addtoTable(id);
							break;
						}
						else if(isDataType(t_temp_1->lName)){
							state = 12;
							t_temp_3 = t_temp_1;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 12){
						if(strcmp(t_temp_1->lName, "*") == 0){
							state = 12;
							continue;
						}
						else if(strcmp(t_temp_1->lType, "IDENTIFIER") == 0){
							state = 13;
							numArg++;
							t_temp_4 = t_temp_1;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 13){
						if(strcmp(t_temp_1->lName, ",") == 0){
							state = 16;
							//create id and store - name is in t4 and type is in t3
							struct identifier id;
							strcpy(id.type, t_temp_3->lName);
							strcpy(id.name, t_temp_4->lName);
							id.size = getSize(id.type);
							id.numArg = numArg;
							addtoTable(id);
							arg[cnt++] = find(id);
							continue;
						}
						else if(strcmp(t_temp_1->lName, ")") == 0){
							struct identifier id;
							strcpy(id.type, t_temp_3->lName);
							strcpy(id.name, t_temp_4->lName);
							id.size = getSize(id.type);
							id.numArg = numArg;
							addtoTable(id);
							arg[cnt++] = find(id);
							strcpy(id.type, "FUNC");
							strcpy(id.name, t_temp_2->lName);
							strcpy(id.retType, retType);
							id.numArg = numArg;
							id.size = 0;
							for(int i=0; i<cnt; i++){
								id.arg[i] = arg[i];
							}
							addtoTable(id);
							break;
						}
						else if(strcmp(t_temp_1->lName, "[") == 0){
							state = 14;
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 14){
						if(strcmp(t_temp_1->lType, "NUMERICAL CONSTANT") == 0){
							state = 15;
							arrSize = strToInt(t_temp_1->lName);
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 15){
						if(strcmp(t_temp_1->lName, "]") == 0){
							state = 13;
							//store parameter id here - type is in t3 and name is in t4...
							struct identifier id;
							strcpy(id.type, t_temp_3->lName);
							strcpy(id.name, t_temp_4->lName);
							id.size = getSize(id.type) * arrSize;
							id.numArg = numArg;
							addtoTable(id);
							arg[cnt++] = find(id);
							continue;
						}
						else{
							break;
						}
					}
					else if(state == 16){
						if(isDataType(t_temp_1->lName)){
							state = 12;
							t_temp_3 = t_temp_1;	//for storing the datatype of the next identifier... 
							continue;
						}
						else{
							break;
						}
					}
				}
			}
		}
		t = getNextToken(&row, &col);
	}

	printf("\nSymbol Table will be displayed shortly...\n\n");

	displayTable();
	
	fclose(fa);
	
	return 0;
}