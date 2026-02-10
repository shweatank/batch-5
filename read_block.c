#include<stdio.h>
#include<fcntl.h>
typedef struct details{
        char name[40];
        char place[40];
        int age;
        char prof[40];
}dts;


int main(){
	int fd;
	FILE *f;
	char ch;
	int l;
	dts p;
	fd=open("data.txt",O_RDONLY);
	while(read(fd,ch,1)==1){
		f=fopen("data.txt","r");
                fseek(f,l,SEEK_SET);
                fscanf(f,"%s%s%d%s",p.name,p.place,&p.age,p.prof);
                printf("%s, %s, %d, %s\n",p.name,p.place,p.age,p.prof);
                l=ftell(f);
                fclose(f);
        }
}
	

