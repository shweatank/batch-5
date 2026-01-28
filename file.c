#include<stdio.h>
void main()
{
	FILE *fp=fopen("data.txt","w");
	if(fp==0)
	{
		printf("File is not found\n");
		return;
	}

	int num=10;
	fprintf(fp,"%d",num);
	fclose(fp);
}
