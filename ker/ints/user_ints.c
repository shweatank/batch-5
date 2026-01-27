#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
typedef struct result{
	int a;
	int b;
	int res;
}re;

int main(){
	re *r=malloc(sizeof(re));
	r->a=30;
	r->b=10;
	FILE *fp;
	fp=fopen("/dev/ints","w+");
	fwrite(r,sizeof(re),1,fp);
	printf("wrote data %d, %d into kernle buffer\n",r->a,r->b);
	free(r);
}
