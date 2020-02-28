






int arm(int n)
{
	int sum=0;
	int temp=n;

		while(temp)
	{
		sum+=pow(temp%10,3);
		temp = temp/10;
	}

	if(sum==n)
	  return 1;

	else
		return 0;
}

int main()
{
	int n,1;
	int x[]={1,2,13,7};
	printf("Enter a number\n");
	scanf("%d",&n);

	

	for(i=0;i<n;i++)
		if(arm(x[i]))
			printf("%d is Arm \n",x[i]);

		else
			printf("%d is not a Arm\n",x[i]);

		return 0;
	
	}