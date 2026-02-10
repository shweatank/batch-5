#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
typedef struct result{
	int a;
	int b;
	int res;
	int flag;
}re;

int main(){
	re *r=malloc(sizeof(re)),w;
	r->a=30;
	r->b=10;
	FILE *fp;
	fp=fopen("/dev/ints1","w+");
	fwrite(r,sizeof(re),1,fp);
	printf("wrote data %d, %d into kernle buffer\n",r->a,r->b);
	fread(&w,sizeof(re),1,fp);
	printf("Read data %d from kernel buffer\n",w.res);
	free(r);
}
