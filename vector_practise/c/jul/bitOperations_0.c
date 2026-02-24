#include<stdio.h>
#include<stdio_ext.h>
void printMenu(void);//declaration
int main()
{
	int data,bit,b;
	char choice;
	int flag=0,left,right;
	while(1)
	{
		printMenu();
		__fpurge(stdin);
		scanf("%c",&choice);

		if(flag==0)
		{
			printf("no data supplied\n");
			    choice='i';
		}

		if((choice=='s')||(choice=='S'))
		                   data|=(1<<bit);
		else if((choice=='c')||(choice=='C'))
		                   data&= ~(1<<bit);
		else if((choice=='m')||(choice=='M'))
		                   data^=(1<<bit);
		else if((choice=='p')||(choice=='P'))
		{
			printf("data:%d   bitPosn:%d\n",data,bit);
			printf("bin Eq of data:");
			
			for(b=31;b>=0;b--)
				printf("%d",(data>>b)&1);
		}
		else if((choice=='r') ||(choice=='R'))
		{
			for(left=31,right=0;left>right;left--,right++)
			{
				if(((data>>left)&1) != ((data>>right)&1))
				{
					data^=(1<<left);
					data^=(1<<right);
				}
			}

		}

		else if(choice=='i')
		{
			printf("Enter data:");
			scanf("%d",&data);
			printf("Enter bit:");
			scanf("%d",&bit);
			flag=1;

		}
		else if((choice=='q')||(choice=='Q'))
		           break;
		else 
			printf("error:invalid entry\n");

		
	}//while
}//main
void printMenu(void)
{
		printf("\nMENU\n");
		printf("s:set bit\n");
		printf("c:clear bit\n");
		printf("m: compliment bit\n");
		printf("i:input  data and bit\n");
		printf("r:reverse bits\n");
		printf("p: print data\n");
		printf("q:quit\n");
		printf("enter your choice\n");
}

