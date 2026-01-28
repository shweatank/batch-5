#include<stdio.h>
#include<stdlib.h>

struct st{
	int a;
	int b;
	char ch;
};

int main(){
	char s[100];
	struct st v={1,2,'+'};
	FILE*fp=fopen("/dev/calci_dev","w+");
	fwrite(&v,sizeof(v),1,fp);	
	fread(s,100,1,fp);
	fclose(fp);
}

