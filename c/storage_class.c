#include<stdio.h>
int ext=9; //globalvar..extern
static int stat_g=5;
void fun()
{
	static int stat_loc=5;
	printf("stat_loc%d\n",stat_loc++);	
	printf("extern var=%d\n",ext);
	printf("static global:%d\n",stat_g);
}
int main()
{
	auto int a=4;
	register int reg;
	int temp;
	scanf("%d",&temp);
	reg=temp;
	printf("reg var=%d\n",reg);
	fun();
	fun();
	fun();
	printf("stat_g=%d\n",stat_g);
	printf("extern var:%d\n",ext);
}
