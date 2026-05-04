#include<stdio.h>
void main()
{
char s[20]="sneha";
int i;
char ch;
FILE *fp,*fp1;
fp1=fopen("fputc","w");
perror("fp1");

fp=fopen("src","r");
if(fp==0)
{
printf("file not present\n");
return;
}

while((ch=fgetc(fp))!=EOF)
fputc(ch,fp1);
}
