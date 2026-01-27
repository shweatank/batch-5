#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>
struct cal
{
	int n1;
	int n2;
	char ch;
};
int main()
{
	struct cal a;
	puts("enter the value a and b");
	scanf("%d %d",&a.n1,&a.n2);
	puts("enter the operand");
	__fpurge(stdin);
	scanf("%c",&a.ch);
	FILE *fp=fopen("/dev/char_device","w+");
	if(fp==NULL)
	{
		puts("file is not opened");
		return 0;
	}
	fwrite(&a,sizeof(a),1,fp);
	fseek(fp,sizeof(a),SEEK_CUR);
	fread(&a,sizeof(a),1,fp);
	printf("calculated is %d\n",a.n1+a.n2);
	return 0;

}
