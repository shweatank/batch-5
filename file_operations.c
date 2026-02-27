#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	FILE*fptr=NULL;
	fptr=fopen("dummy.txt","w");
	int a=7,b=0;
	int arr[5]={6,7,8,9,1},*brr=calloc(1,sizeof(int));
	//fprintf(fptr,"%d",a);
	fwrite(arr,sizeof(int),5,fptr);
	fclose(fptr);
	fptr=fopen("dummy.txt","r");
	//fscanf(fptr,"%d",&b);
	while((fread(brr,sizeof(int),1,fptr))!=0)
	{
		printf("%d ",*brr);
	}

	fclose(fptr);
}
