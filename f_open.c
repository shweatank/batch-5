#include<stdio.h>
#include<limits.h>
void main(){

FILE *fp=fopen("/dev/basic_char","w+");
char ch[20]="Hello1234";
//int a=1234567890;
//fwrite(&a,1,sizeof(int),fp);
//fprintf(fp,"%d",a);
fwrite(ch,1,sizeof(ch),fp);
fclose(fp);

FILE *fp1=fopen("/dev/basic_char","r");
char ch2[20];
fread(ch2,1,sizeof(ch2),fp1);
printf("%s\n",ch2);
//fputc('A',fp);

fclose(fp);
}
