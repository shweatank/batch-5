#include<stdio.h>
struct st{
int a;
int b;
char c;
int res;
};
void main(){

struct st v;
v.a=1;
v.b=2;
v.c='-';

FILE *fp=fopen("/dev/basic_char","w+");
fwrite(&v,1,sizeof(v),fp);
fread(&v,1,sizeof(v),fp);
perror("fread");
printf("result=%d\n",v.res);
fclose(fp);
}
