#include<stdio.h>
#include<stdlib.h>
void main()
{
	struct st
	{
		int n1;
		char ch;
		int n2;
		int res;
	};
	FILE* fd;
	int res;
	struct st v={60,'-',20,0},v1;
	
	fd=fopen("/dev/basic_char","w+");
	fwrite(&v,sizeof(struct st),1,fd);
	fseek(fd,0,SEEK_SET);
		
	fread(&v1,sizeof(struct st),1,fd);
	printf("result=%d\n",v1.res);
	printf("%d %d %c\n",v1.n1,v1.n2,v1.ch);

	fclose(fd);
}
