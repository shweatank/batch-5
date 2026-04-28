#include<stdio.h>
#include<string.h>
#include<stdlib.h>


char* reverseWords(char* s) {
    int i,j;
    char *p=malloc(strlen(s)+1),*q=calloc(1,strlen(s)+1),temp;
    for(i=0,j=strlen(s)-1;s[i];i++,j--)
    {
        p[i]=s[j];
    }
    p[i]='\0';
    printf("str %s\n",p);
    while(p=strtok(p," "))
    {
        for(int i=0,j=strlen(p)-1;i<j;i++,j--)
        {
            temp=p[i];
            p[i]=p[j];
            p[j]=temp;
        }
        strcat(q,p);
        q[strlen(q)]=' ';
        p=NULL;
    }
    return q;
}

int main()
{
	char str[100],*p;
	printf("enter the string:");
	scanf("%[^\n]s",str);
	p=reverseWords(str);
	printf("after: %s\n",p);
}
