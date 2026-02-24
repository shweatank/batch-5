#include<stdio.h>
#include<stdio_ext.h>
void printMenu(void);//declaration
int setBit(int data,int bit);
int clearBit(int data,int bit);
int complimentBit(int data,int bit);


void printBinary(int d)
{
	int b;
	printf("\nbinary:");
	for(b=31;b>=0;b--)
		printf("%d",(d>>b)&1);

	printf("\n");
}

int reverseBits(int var)
{
	int i;
	int rev=0;
	for(i=0;i<=31;i++)
	{
		if((var>>i) &1)
		{
			rev=rev|(1<<(31-i));

		}
	}
	///the var remains unchanged. and rev contains reverse equivalent of var.
	return rev;
}



int main()
{
	int data,bit;
	char choice;
	int flag=0;
	while(1)
	{
		printMenu();
		__fpurge(stdin);
		scanf("%c",&choice);
		switch(choice)
		{
			case 's': data=setBit(data,bit);
				  printf("%d",data);
				  break;
			case 'c': data=clearBit(data,bit);
				  printf("%d",data);
				  break;
			case 'm': data=complimentBit(data,bit);
				  printf("%d",data);
				  break;
			case 'p': printf("%d %d\n",data,bit);
				  printBinary(data);
				  break;
			case 'i': printf("enter data:");
				  scanf("%d",&data);
				  printf("enter bit:");
				  scanf("%d",&bit);
				  flag=1;
				  break;
			case 'r': data=reverseBits(data);
				  printf("%d",data);
				  break;
			case 'q': return 0;//exit(0);
			default : printf("invalid entry\n"); 
		}
		if(flag==0)
                {
                        printf("no data supplied\n");
                        choice='i';
                }
	}//while
}
int setBit(int data,int bit)
{
	return data|(1<<bit);
}
int clearBit(int data, int bit)
{
	return (data&= ~(1<<bit));
}
int complimentBit(int data,int bit)
{
	data^=(1<<bit);
	return data;
}
void printMenu(void)
{
	printf("\nMENU\n");
	printf("s:set bit\n");
	printf("c:clear bit\n");
	printf("m: compliment bit\n");
	printf("i:input again\n");
	printf("r:reverse bits\n");
	printf("p: print data\n");
	printf("q:quit\n");
	printf("enter your choice\n");
}

