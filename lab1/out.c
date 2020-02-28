

#include<stdio.h>
int main()
{
	FILE *fopen(),*fp;
	int c;
	fp = fopen("prog.c","r"); 	c = getc(fp);
	while(c!=EOF)
	{
		putchar(c);
		c = getc(fp);

	}  
		fclose(fp);
	return 0;
}
