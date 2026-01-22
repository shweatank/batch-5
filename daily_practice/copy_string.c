#include<stdio.h>
#include<string.h>
void mystrcpy(char *,const char *);
int main()
{
        char str[100];
        puts("enter the string");
        fgets(str,100,stdin);
        str[strlen(str)-1]='\0';
        char str2[100];
        mystrcpy(str2,str);
        puts("given string is");
        puts(str);
        puts("copied string is");
        puts(str2);
}
void mystrcpy(char *dst,const char *src)
{
        int i=0;
        while(src[i])
        {
                dst[i]=src[i];
                i++;
        }
        dst[i]='\0';
}
