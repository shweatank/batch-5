#include <stdio.h>
#include <stdlib.h>

void writeToFile(int *arr,int len,char *filename) {
	FILE *fp = fopen(filename,"w");
	//fwrite(arr,sizeof(int),len,fp); (Writes the code in a RAW Format
	for(int i = 0;i<len;i++) {
		fprintf(fp,"%d",arr[i]);;
	}
	fclose(fp);
}
void input(int *arr,int len) {
	for(int i = 0;i<len;i++) {
		printf("Enter the %d element: ",i);
		scanf("%d",&arr[i]);
	}
}

int main() {
	char str[100];
	printf("Enter the filename to be created: ");
	scanf("%s",str);
	int len;
	printf("Enter the lenght of array: ");
	scanf("%d",&len);
	int arr[len];
	input(arr,len);
	writeToFile(arr,len,str);
}
