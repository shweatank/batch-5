#include<stdio.h>
int add(int ,int );
void Print(int (*)(int,int),int,int);
int sub(int ,int );
int main()
{
	int a,b;
	printf("enter the a and b\n");
	scanf("%d%d",&a,&b);
	int (*ptr)(int,int);
	ptr=add;
	Print(ptr,a,b);
	ptr=sub;
	Print(ptr,a,b);

}
int add(int a,int b)
{
	return a+b;
}
void Print(int (*ptr)(int,int),int a,int b)
{
	printf("%d\n",ptr(a,b));
}
int sub(int a,int b)
{
	return a-b;
}
