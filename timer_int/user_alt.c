#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
typedef struct result{
	int a;int b;int res;
}re;
int main(){
//	FILE *fp;
	int fd;
	re *r=malloc(sizeof(re));
	r->a=10;r->b=30;
	//fp=fopen("/dev/irq_bh","w+");
//	fwrite(r,sizeof(re),1,fp);
	fd=open("/dev/irq_bh",O_RDWR);
	//if(fd<1)
		perror("open");
	write(fd,r,sizeof(re));
	printf("wrote %d,%d\n",r->a,r->b);
	//fread(&w,sizeof(re),1,fp);
	//read(fd,&w,sizeof(re));
	//printf("read %d\n",w.res); 
	free(r);
	close(fd);
}


