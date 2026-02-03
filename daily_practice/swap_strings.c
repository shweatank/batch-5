#include<stdio.h>
#include<string.h>
int main()
{
	char str1[100],str2[100];
	printf("enter the string1\n");
	scanf("%s",str1);
	printf("enter the string2\n");
	scanf("%s",str2);
	char temp[100];
	strcpy(temp,str1);
	strcpy(str1,str2);
	strcpy(str2,temp);
	printf("string 1 is %s\n",str1);
	printf("string 2 is %s\n",str2);
}
