#include<stdio.h>
void main()
{
	char s[20];
	int len;
	printf("enter the string..\n");
	scanf("%s",s);
	for(len=0;s[len];len++);
           printf("string userdefined length:%d\n",len);

}
/*#include<string.h>
//predeined
void main()
{
	char s[20];
	int len;
	printf("enter the string..\n");
	scanf("%s",s);
	len=strlen(s);
	printf("string length:%d",len);
}
*/
