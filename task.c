#include<stdio.h>
void main(){

FILE *fp=fopen("/dev/basic_char","w+");
char s[10];
printf("Enter the expression \n");
scanf("%s",s);
fwrite(s,1,sizeof(s),fp);
fclose(fp);
}
