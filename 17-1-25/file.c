#include<stdio.h>
void main()
{
	int num=100;
FILE * fp= fopen("data","w+");
if(fp==0)
{
	perror("fopen");
	return;
}

fwrite(&num,sizeof(num),1,fp);

fread(&num,sizeof(num),1,fp);
printf("read from file: %d\n",num);

fclose(fp);


}
