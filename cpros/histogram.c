#include"header.h"
#define size 128
void main()
{
int freq[size]={0},i;
char s[40];
printf("enter the string:\n");
scanf("%[^\n]",s);
for(i=0;s[i];i++)
freq[s[i]]++;

for(i=0;i<size;i++)
{
if(freq[i]>0)
{
printf("char %c-",i);
for(int j=0;j<freq[i];j++)
printf("*");
printf("\n");
}
}
}
