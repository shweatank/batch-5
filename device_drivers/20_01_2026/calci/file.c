#include<stdio.h>
#include<stdlib.h>

int main(){
	char s[100]="33,7,-";
	char buf[100];
	
	FILE *fp=fopen("/dev/calci_dev","w");
	scanf("%[^\n]s",s);
	fputs(s,fp);
	fclose(fp);

	fp=fopen("/dev/calci_dev","r");
	fgets(buf,100,fp);
	printf("%s\n",buf);
	fclose(fp);
}


