#include<stdio.h>
#include<string.h>

void string_rev(char *str,int start,int end)
{
	if(start>=end)
		return;
	char temp=str[start];
	str[start]=str[end];
	str[end]=temp;
	
	string_rev(str,start+1,end-1);
}

int main()
{
	char str[20];
	printf("enter string : ");
	scanf("%[^\n]s",str);

	printf("actul string : %s\n",str);
	string_rev(str,0,strlen(str)-1);
	printf("after reverse : %s\n",str);
	return 0;	
}
