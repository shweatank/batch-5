#include"main.h"
int main()
{
	char ch;
	int ret;
	while(1)
	{
		__fpurge(stdin);
		printf("enter the choice\n");
		scanf("%c",&ch);
		switch(ch)
		{
			case 'a':ret=add(10,20);
				 printf("add is %d\n",ret);
				 break;
			case 's':ret=sub(20,10);
				 printf("sub is %d\n",ret);
				 break;
			case 'e':exit(0);
		}
	}
}

