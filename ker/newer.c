#include<stdio.h>
#include<string.h>
int main(){
	FILE *fp;
	fp=fopen("/dev/open_close","w+");
	char buff[100]="5,20,*",buff2[50];
	//strcpy(buff,"device");
	fwrite(buff,strlen(buff)+1,1,fp);
	printf("wrote into buff %s\n",buff);
	fread(buff2,sizeof(buff2)+1,1,fp);
	printf("read from buff is %s\n",buff2);
	fclose(fp);
}
