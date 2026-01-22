#include<stdio.h>
#include<string.h>
#include<stdio_ext.h>
int mystrcmp(const char *,const char *);

int main()
{
        char str1[100],str2[100];
        puts("enter the str1");
        scanf("%[^\n]s",str1);
        __fpurge(stdin);
        puts("enter the str2");
        scanf("%[^\n]s",str2);
        if(mystrcmp(str1,str2)==0)
                printf("strings are equal\n");
        else
                printf("strings are not equal\n");
        return 0;
}
int mystrcmp(const char *str1,const char *str2)
{
        int i=0;
        while(str1[i] && str2[i])
        {
                if(str1[i]!=str2[i])
                        return str1[i]-str2[i];
                i++;
        }
        return str1[i]-str2[i];
}
