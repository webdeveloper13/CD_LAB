#include<stdio.h>
#include<stdlib.h>

int fnc(int a,int b)
{

}

int main(int argc, char const *argv[])
{
	FILE *fa,*fb;
	int ca,cb,cc,cd;
	fa = fopen("ip.c","r");
	if(fa==NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	fb = fopen("out2.c","w");
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

	fclose(fa);
	fclose(fb);
	return 0;
}
