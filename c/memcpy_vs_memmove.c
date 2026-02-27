#include<stdio.h>
#include<string.h>
int main()
{
	char str[20]="ABCDEFG";
	puts(str);
	memmove(str+3,str+1,5);
	printf("after memmove:%s\n",str);
	strcpy(str,"ABCDEFG");
	memcpy(str+3,str+1,5);
	printf("after memcpy:%s\n",str);

}
