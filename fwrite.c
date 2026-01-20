#include<stdio.h>
void main(){

FILE *fp=fopen("/dev/basic_char","w+");
char ch[20]="hello";
fwrite(ch,1,sizeof(ch),fp);
fclose(fp);
}
