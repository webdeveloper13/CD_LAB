#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
int compare(char arr[][10],char str[]){
	for (int i = 0; i < 7; ++i)
	{
		if(strcmp(arr[i],str)==0){
			return 1;
		}
	}
	return 0;
}

int main()
{
	FILE *fa,*fb;
	int ca,cb;
	char arr[7][10] = {"int","char","float","if","else","while","return"};
	fa = fopen("q2.c","r");
	if(fa==NULL){
		printf("File not found");
		exit(0);
	}
	fb=fopen("out3.c","w");
	ca=getc(fa);
	int l=1;
	int col=1;
	while(ca!=EOF){
		char buf[50];
		while(ca==' '){
			ca=getc(fa);
			col++;
		}
		int idx=0;
		if (isalnum(ca) || ca=='_')
		{
			while(isalnum(ca) || ca=='_'){
				buf[idx]=ca;
				idx++;
				ca=getc(fa);
				col++;
			}
			buf[idx]='\0';
			if(compare(arr,buf)==1){
				for(int i=0;i<idx;i++)
				{
					buf[i]=buf[i]-32;
				}
				printf("%s %d %d \n",(buf),l,col);
			}
		}

		if(ca=='\n') l++;col=0;
		ca=getc(fa);

	}
	return 0;
}