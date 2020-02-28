#include<stdio.h>
#include<stdlib.h>


int fnc(int a,int b)
{

}

int main(int argc, char const *argv[])
{
	FILE *fa,*fb;
	int ca,cb;
	fa = fopen("ip.c","r");
	if(fa==NULL)
	{
		printf("File not found\n");
		exit(0);
	}
	fb = fopen("no_space.c","w");
	ca = getc(fa);
	while(ca!=EOF)
	{
		int flag=0;
		if(ca==' '||ca == '\t'){
			flag=1;
			cb = getc(fa);

			while((cb==' '||cb=='\t')&& cb!=EOF){
				cb = getc(fa);
			}
		}

		if(ca!='\t')
			putc(ca,fb);
		if(flag==1)
			putc(cb,fb);

		ca=getc(fa);
	}

	return 0;
}
