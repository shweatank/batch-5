#include<stdio.h>
void main()
{
	FILE *fp=fopen("data","r+");
	int num;
	fread(&num,sizeof(int),1,fp);
	printf("%d\n",num);
	fclose(fp);
}
