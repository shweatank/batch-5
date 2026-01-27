#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
	struct op{int a;int b;char c; int res;};
	struct op v;
	int r;
	//int d1,d2;
	//scanf("%d%d",&d1,&d2);
	//v.a=d1;v.b=d2;v.c='+';v.res=0;
	v.a=22;v.b=11;v.c='*';v.res=0;
	FILE *fptr=fopen("/dev/basic_char","w+");
	//perror("fopen");
	fwrite(&v,1,sizeof(struct op),fptr);
	if((fread(&r,1,sizeof(int),fptr))!=0)
	{
		printf("res:%d\n",r);
	}
	//perror("fread");
	fclose(fptr);
}
